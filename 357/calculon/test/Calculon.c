
#include "Calculon.h"

static Suite TestSuite;

int main(int argc, char **argv) {
   char *suiteFile;
   int numProgram;
   
   suiteFile = *(argv + 1);
   TestSuite.programs = (Program **)calloc(MAX_FILES, sizeof(Program **));
   ParseSuite(suiteFile);
   for (numProgram = 0; numProgram < TestSuite.numPrograms; numProgram++) {
      RunProgram(numProgram);
   }
   return EXIT_SUCCESS;
}

void CopyToTemp(int numProg, char *tempDir) {
   int index; 
   Program *prog = TestSuite.programs[numProg];
   Test *test;
   
   for (index = 0; index < prog->numInputFiles; index++) {
      if (access(prog->inputFiles[index], F_OK)) {
         printf("Could not find required test file '%s'\n",
          prog->inputFiles[index]);
         exit(1);
      }
      else {
         if (fork()) {
            wait(NULL);
         }
         else {            
            execl(CP, CP, prog->inputFiles[index], tempDir, NULL);
         }
      }
   }
   for (index = 0; index < prog->numTests; index++) {
      test = prog->tests[index];
      if (access(test->inputFile, F_OK)) {
         printf("Could not find required test file '%s'\n", test->inputFile);
         exit(1);
      }
      else {
         if (fork()) {
            wait(NULL);
         }
         else {
            execl(CP, CP, test->inputFile, tempDir, NULL);
            exit(-1);  
         }
      }
      
      if (access(test->outputFile, F_OK)) {
         printf("Could not find required test file '%s'\n", test->outputFile);
         exit(1);
      }
      else {
         if (fork()) {
            wait(NULL);
         }
         else {
            execl(CP, CP, test->outputFile, tempDir, NULL);
            exit(-1);  
         }
      }   
   }   
}

int CompileFile(int numProg, char *tempDir) {

   char **argv = calloc(MAX_FILES, sizeof(char *)), *extension;
   int argc = 0, index, status, success = 0;
   Program *prog = TestSuite.programs[numProg];
   
   argv[argc++] = GCC;

   for (index = 0; index < prog->numInputFiles; index++) {
      extension = strrchr(prog->inputFiles[index], '.');
      if (extension && !(strcmp(extension, ".c"))) {
         argv[argc++] = prog->inputFiles[index];
      }
   }
   
   argv[argc++] = "-o";
   argv[argc++] = prog->execName;
   argv[argc] = NULL;   
   if (fork()) {
      wait(&status);
      if (WEXITSTATUS(status)) {
         printf("Failed: gcc");
         for (index = 1; index < argc; index++) {
            printf(" %s", argv[index]);
         }
         printf("\n");
         success = 0;
      }
      else {
         success = 1;
      }
   }
   else {
      chdir(tempDir);
      execv(GCC, argv);
      exit(-1); 
   }
   return success;      
}

int Makefile(int numProg, char *tempDir) {
   
   int success = 0, output, status;
   
   if (fork()) {
      wait(NULL);
   }
   else {
      execl(CP, CP, "Makefile", tempDir, NULL); 
      exit(-1);  
 
   }
   
   if (fork()) {
      wait(&status);
      if (WEXITSTATUS(status) == 0) {
         success = 1; 
      }
      else {
         printf("Failed: make %s\n", TestSuite.programs[numProg]->execName);
         success = 0;
      }
   }
   else {
      chdir(tempDir);
      output = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS);
      dup2(output, 1);
      close(output);
      execl(MAKE, MAKE, TestSuite.programs[numProg]->execName, NULL);
      exit(-1);  
   }
   return success;   
}

Report *RunExec(Test *test, char **argv, char *tempDir) {
   
   int inputFile = -1, outputFile = -1, errorFile = -1, status;
   Report *report = calloc(1, sizeof(Report));

   chdir(tempDir);
   umask(0);
   inputFile = open(test->inputFile, O_RDONLY);
   outputFile = open("./test.output.temp", O_WRONLY |
    O_CREAT | O_TRUNC, PERMISSIONS);

   errorFile = open("./test.output.temp", O_WRONLY |
    O_CREAT | O_TRUNC, PERMISSIONS);
   
   fchmod(inputFile, PERMISSIONS);
   fchmod(outputFile, PERMISSIONS);
   fchmod(errorFile, PERMISSIONS);
   
   report->runtime = 0;
   report->timeout = 0;
   report->diffFail = 0;
   
   if (fork()) {
      close(inputFile);
      close(outputFile);
      close(errorFile);
      wait(&status);
      if (WEXITSTATUS(status) == RUNTIME_ERROR_CODE ||
       WEXITSTATUS(status) == 1) {
         report->runtime = 1;
      }
      else if (WEXITSTATUS(status) == TIMEOUT_ERROR_CODE) {
         report->timeout = 1;
      }
      else if (WEXITSTATUS(status) == BOTH_ERROR_CODES) {
         report->timeout = 1;
         report->runtime = 1;
      }
   }
   else {
      chdir(tempDir);

      dup2(inputFile, 0);
      close(inputFile);

      dup2(outputFile, 1);
      close(outputFile);      
      
      dup2(errorFile, 2);
      close(errorFile);
 
      execv(SAFERUN, argv);
      exit(-1);
   }
   return report;
}

int RunTest(int numProgram, int testNumber, char *tempDir) {
   
   Report *report;
   int argc = 0, index, testsFailed = 0;
   char *tFlag = calloc(MAX_CHARS, sizeof(char)),
    *scaledTFlag = calloc(MAX_CHARS, sizeof(char)), 
    *execName = calloc(MAX_CHARS, sizeof(char)), 
    **argv = calloc(MAX_CHARS, sizeof(char *));
   Program *prog = TestSuite.programs[numProgram];
   Test *test = prog->tests[testNumber];
   
   sprintf(tFlag, "%s%d", "-t", test->time);
   sprintf(scaledTFlag, "%s%d", "-T", (test->time) * TSCALE);
   sprintf(execName, "%s%s", "./", prog->execName);
   
   argv[argc++] = SAFERUN;
   argv[argc++] = PFLAG;
   argv[argc++] = tFlag;
   argv[argc++] = scaledTFlag;
   argv[argc++] = execName;
   for (index = 0; index < test->argc; index++) {
      argv[argc++] = test->argv[index];
   }
   argv[argc] = NULL;
   report = RunExec(test, argv, tempDir);

   argc = 0;
   argv[argc++] = DIFF;
   argv[argc++] = test->outputFile;
   argv[argc++] = "test.output.temp";
   argv[argc] = NULL;
   
   RunDiff(argv, tempDir, report);
   testsFailed = CheckErrors(report, numProgram, testNumber);
   return testsFailed;
}

int CheckErrors(Report *report, int numProgram, int testNumber) {
   Program *prog = TestSuite.programs[numProgram];
   int fail = 0;
   
   if (report->diffFail) {
      fail++;
   }
   if (report->runtime) {
      fail++;
   }
   if (report->timeout) {
      fail++;
   }
   if (fail) {
      printf("%s test %d failed:", prog->execName, testNumber + 1);
      if (report->diffFail) {
         printf(" diff failure");
         if (--fail > 0) {
            printf(",");
         }
      }
      if (report->runtime) {
         printf(" runtime error");
         if (--fail > 0) {
            printf(",");
         }
      }
      if (report->timeout) {
         printf(" timeout");
      }
      printf("\n");
      fail = 1;
   }
   return fail;
}

void RunDiff(char **argv, char *tempDir, Report *report) {
   
   int status, outputFile;
   
   report->diffFail = 0;
   if (fork()) {
      wait(&status);
      if (WEXITSTATUS(status) != 0) { //Differences found
         report->diffFail = 1;
      }
   }
   else {
      chdir(tempDir);
      outputFile = open("/dev/null", O_WRONLY | O_CREAT
       | O_TRUNC, PERMISSIONS);
      dup2(outputFile, 1);
      close(outputFile);
      execv(DIFF, argv); 
   }
}

void RunProgram(int numProgram) {
   int processID, success, index, failed = 0;
   char *tempDirectory = calloc(MAX_CHARS, sizeof(char)),
    *sourceDirectory = calloc(MAX_CHARS, sizeof(char));
   Program *prog = TestSuite.programs[numProgram];
   
   processID = getpid();
   sprintf(tempDirectory, "%s%d", ".", processID);
   mkdir(tempDirectory, S_IRWXU | S_IRWXG | S_IRWXO);
   
   getcwd(sourceDirectory, MAX_CHARS);
   CopyToTemp(numProgram, tempDirectory);
   if (access("Makefile", F_OK)) {
      success = CompileFile(numProgram, tempDirectory);
   }
   else {
      success = Makefile(numProgram, tempDirectory);
   }
   if (success) {
      for (index = 0; index < prog->numTests; index++) {
         failed += RunTest(numProgram, index, tempDirectory);
      }
   }
   
   if (!(failed) && success) {
      printf("%s %d of %d tests passed.\n", prog->execName,
       prog->numTests, prog->numTests);
   }
   chdir(sourceDirectory);
   DeleteDirectory(tempDirectory);
}

void DeleteDirectory(char *name) {
   DIR *directory = opendir(name);
   struct dirent *directoryEntry;
   char *removeName = calloc(MAX_CHARS, sizeof(char));

   if (directory) {
      while ((directoryEntry = readdir(directory))) {
         sprintf(removeName, "%s/%s", name, directoryEntry->d_name);
         remove(removeName);
      }
      rmdir(name);
   }
   else {
      printf("that directory did not exsist\n");
   }
   
}

void ParseSuite(char *fileName) {
   
   FILE *file;
   char *line = calloc(MAX_CHARS, sizeof(char)),
    *letter = calloc(2, sizeof(char));
   int numTests = 0;
   Program *program;
   Test *test;
   
   file = fopen(fileName, "r");

   if (!file) {
      printf("failed to open file with name -%s-\n", fileName);
      exit(-1);
   }

   while (fgets(line, MAX_CHARS, file)) {
      sscanf(line, "%c", letter);
      if (strcmp(letter, "P") == 0) {
         program = ParseProgram(line);
         
         (TestSuite.programs)[TestSuite.numPrograms] =
          calloc(1, sizeof(Program *));
         (TestSuite.programs)[(TestSuite.numPrograms)] = program;
         (TestSuite.programs)[TestSuite.numPrograms++]->tests =
          (Test **)calloc(MAX_FILES, sizeof(Test **));

      }
      else if (strcmp(letter, "T") == 0) {
         test = ParseTest(line);
         numTests =
          ((TestSuite.programs)[TestSuite.numPrograms - 1])->numTests++;
         (((TestSuite.programs)[TestSuite.numPrograms - 1])->tests)[numTests]
          = (Test *)calloc(1, sizeof(Test *));
         ((TestSuite.programs)[TestSuite.numPrograms - 1])->tests[numTests]
          = test;
      }
   }
}

Program *ParseProgram(char *line) {
   char *token;
   int numInputFiles = 0;
   Program *program = calloc(1, sizeof(Program));
   
   program->inputFiles = calloc(MAX_FILES, sizeof(char *));
   token = strtok(line, " \n");
   token = strtok(NULL, " \n");
   program->execName = strdup(token);
   while ((token = strtok(NULL, " \n"))) {
      program->inputFiles[numInputFiles++] = strdup(token);
   }
   program->numInputFiles = numInputFiles;
   return program;
}

Test *ParseTest(char *line) {
   
   char **argv = calloc(MAX_FILES, sizeof(char *)), *token;
   int argc = 0;
   Test *test = calloc(1, sizeof(Test));
   
   test->argv = argv;
   
   token = strtok(line, " \n");
   token = strtok(NULL, " \n");
   test->inputFile = strdup(token);
   
   token = strtok(NULL, " \n");
   test->outputFile = strdup(token);

   token = strtok(NULL, " \n");
   test->time = atoi(token);
   
   while ((token = strtok(NULL, " \n"))) {
      test->argv[argc++] = strdup(token);
   }
   test->argc = argc;
   return test;
}

void PrintSuite() {
   int index, testIndex, fileIndex;
   Program *prog;
   Test *test;
   
   printf("printing suite with %d progs\n", TestSuite.numPrograms);
   for (index = 0; index < TestSuite.numPrograms; index++) {
      prog = TestSuite.programs[index];
      printf("\nprogram %d\n", index);
      printf("exec name = %s\n", prog->execName);
      printf("numInptFiles = %d\n", prog->numInputFiles);
      for (fileIndex = 0; fileIndex < prog->numInputFiles; fileIndex++) {
         printf("InptFiles %d = %s\n", fileIndex, prog->inputFiles[fileIndex]);
         
      }
      printf("numTests = %d\n", prog->numTests);
      printf("Printing tests:\n");
      for (testIndex = 0; testIndex < prog->numTests; testIndex++) {
         test = prog->tests[testIndex];
         printf("test #%d\n", testIndex);
         printf("input file = %s\n", test->inputFile);
         printf("output file = %s\n", test->outputFile);
         printf("argc = %d\n", test->argc);
         for (fileIndex = 0; fileIndex < test->argc; fileIndex++) {
            printf("argv%d  = %s\n", fileIndex, test->argv[fileIndex]);
         }
         printf("time = %d\n", test->time);
      }
   }
   printf("done printing\n");
}


#include "Calculon.h"

//static char *suiteFile;
static Suite suite;

int main(int argc, char **argv) {
   char *suiteFile;
   int numProgram;
   suiteFile = *(argv + 1);
   suite.programs = (Program **)calloc(MAX_FILES, sizeof(Program **));
   ParseSuite(suiteFile);
   PrintSuite();
   for (numProgram = 0; numProgram < suite.numPrograms; numProgram++) {
      //printf("running program #%d\n", numProgram);
      RunProgram(numProgram);
   }
   return EXIT_SUCCESS;
}

void CopyToTemp(int numProg, char *tempDir) {
   int index; 
   Program *prog = suite.programs[numProg];
   Test *test;
   
   for (index = 0; index < prog->numInputFiles; index++) {
      if (access(prog->inputFiles[index], F_OK)) {
         printf("Could not find required test file '%s'\n", prog->inputFiles[index]);
      }
      else {
         if (fork()) {
            wait(NULL);
         }
         else {
            
            execl(CP, CP, prog->inputFiles[index], tempDir, NULL);
            printf("BADFORK\n");  
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
                        printf("BADFORK2\n");
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
                        printf("BADFORK3\n");
                        exit(-1);  
  
         }
      }   
   }   
}

int Makefile(int numProg, char *tempDir) {
   
   int success = 0, output, status;
   
   if (fork()) {
      wait(NULL);
   }
   else {
      execl(CP, CP, "Makefile", tempDir, NULL);
                  printf("BADFORK4\n"); 
                                          exit(-1);  
 
  
   }
   
   if (fork()) {
      wait(&status);
      if (WEXITSTATUS(status) == 0) {
         success = 1; 
      }
      else {
         printf("Failed: make %s\n", suite.programs[numProg]->execName);
         success = 0;
      }
   }
   else {
      chdir(tempDir);
      output = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS);
      dup2(output, 1);
      close(output);
      execl(MAKE, MAKE, suite.programs[numProg]->execName, NULL);
                  printf("BADFORK5\n"); 
                                          exit(-1);  
 

   }
   printf("the make returned a status of %d\n", success);
   return success;   
}


void RunProgram(int numProgram) {
   int pid;
   char *tempDirectory = calloc(MAX_CHARS, sizeof(char)), *sourceDirectory = calloc(MAX_CHARS, sizeof(char));
   
   pid = getpid();
   sprintf(tempDirectory, "%s%d", ".", pid);
   mkdir(tempDirectory, S_IRWXU);
   
   getcwd(sourceDirectory, MAX_CHARS);
   CopyToTemp(numProgram, tempDirectory);
   if (access("Makefile", F_OK)) {
      //no makefile
   }
   else {
      Makefile(numProgram, tempDirectory);
   }
   printf("about to delete dir\n");
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
      printf("that directory did not exsist?\n");
   }
   
}

void ParseSuite(char *fileName) {
   FILE *file;
   
   file = fopen(fileName, "r");
   
   if (!file) {
      printf("failed to open file\n");
      exit(-1);
   }
   
   char *line = calloc(MAX_CHARS, sizeof(char)), *letter = calloc(2, sizeof(char));
   int numTests = 0;
   
   Program *program;
   Test *test;
   while (fgets(line, MAX_CHARS, file)) {
      sscanf(line, "%c", letter);
      if (strcmp(letter, "P") == 0) {
         program = ParseProgram(line);
         
         (suite.programs)[suite.numPrograms] = calloc(1, sizeof(Program *));
         (suite.programs)[(suite.numPrograms)] = program;
         (suite.programs)[suite.numPrograms++]->tests = (Test **)calloc(MAX_FILES, sizeof(Test **));

      }
      else if(strcmp(letter, "T") == 0) {
         test = ParseTest(line);
         numTests = ((suite.programs)[suite.numPrograms - 1])->numTests++;
         (((suite.programs)[suite.numPrograms - 1])->tests)[numTests] = (Test *)calloc(1, sizeof(Test *));
         ((suite.programs)[suite.numPrograms - 1])->tests[numTests] = test;         
      }
      //no argument? 
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
   int i, j, k;
   Program *p;
   Test *t;
   printf("printing suite with %d progs\n", suite.numPrograms);
   for (i = 0; i < suite.numPrograms; i++) {
      p = suite.programs[i];
      printf("\nprogram %d\n", i);
      printf("exec name = %s\n", p->execName);
      printf("numInptFiles = %d\n", p->numInputFiles);
      for (k = 0; k < p->numInputFiles; k++) {
         printf("InptFiles %d = %s\n", k, p->inputFiles[k]);
         
      }
      printf("numTests = %d\n", p->numTests);
      printf("Printing tests:\n");
      for (j = 0; j < p->numTests; j++) {
         t = p->tests[j];
         printf("test #%d\n", j);
         printf("input file = %s\n", t->inputFile);
         printf("output file = %s\n", t->outputFile);
         printf("argc = %d\n", t->argc);
         for (k = 0; k < t->argc; k++) {
            printf("argv%d  = %s\n", k, t->argv[k]);
         }
         printf("time = %d\n", t->time);
      }
   }
   printf("done printing\n");
}
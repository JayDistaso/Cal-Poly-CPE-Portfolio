#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "Report.h"
#define MAX_CHARS 100
#define EXEC_NAME "./Cell", "./Cell"
#define ONE_INNER_CELL 3
#define TWO_INNER_CELLS 4
#define MANY_CELLS 5
#define BASE 10
#define MAX_FD 12
#define READ 0
#define WRITE 1
#define STATUS_MULT 256

void InputErrorCheck(int numCells, int rightSet, int simTimeTaken,
 int numCellsTaken);

void BranchCase(int numCells, int simTime, double rightVal, 
 double leftVal, int parentPipe[2]);
 
void OneCell(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells, 
 int simTime);
 
void TwoCells(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS); 
 
void OneInnerCell(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS);

 
void TwoInnerCells(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS);
 
void ManyCells(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS);
 
void ManyCellsOdd(int index, int **pids, char *simTimeS, char
 *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, int 
 **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, char 
 **oldToNew1ReadS, char **oldToNew2WriteS, 
 char **oldToNew2ReadS, 
 char **newToOld1WriteS, char **newToOld1ReadS, char 
 **newToOld2WriteS, char **newToOld2ReadString, int *parentPipe);

void ManyCellsEven(int index, int **pids, char *simTimeS, char
 *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, int 
 **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, char 
 **oldToNew1ReadS, char **oldToNew2WriteS, 
 char **oldToNew2ReadS, char **newToOld1WriteS, 
 char **newToOld1ReadS, char **newToOld2WriteS, 
 char **newToOld2ReadString, int *parentPipe); 
 
void CleanupManyCells(int index, int **pids, char *simTimeS, char
 *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, int 
 **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, char 
 **oldToNew1ReadS, char **oldToNew2WriteS, 
 char **oldToNew2ReadS, char **newToOld1WriteS, 
 char **newToOld1ReadS, char **newToOld2WriteS, 
 char **newToOld2ReadString, int numCells, int simTime,
 int parentPipe[2], int **pipeEnd, char **endWriteString, 
 char **endReadString, char *rightValS);
 
void ManyCellsBranch(int index, int **pids, char *simTimeS, char
 *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, int 
 **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, char 
 **oldToNew1ReadS, char **oldToNew2WriteS, 
 char **oldToNew2ReadS, char **newToOld1WriteS, 
 char **newToOld1ReadS, char **newToOld2WriteS, 
 char **newToOld2ReadString, int *parentPipe);  
 
void MakeOneTimePipe(int **pipeName, char **writeString, char **readString); 
 
void ReadReports(int numCells, int simTime, int parentPipe[2], int *pids);

int CheckStatus(int status, int pids[], int processID, int numCells);

void MakePipe(int **pipeName, char **writeString, char **readString);

int main(int argc, char **argv) {
   
   int numCells = 0, numCellsTaken = 0, simTimeTaken = 0, simTime = 0, rightSet
    = 0, leftSet = 0, originalArgc, parentPipe[2];
   double value = 0, leftVal = 0, rightVal = 0;
   char *end;
   
   originalArgc = argc;
   while (--argc) {
      (argv)++;

      if (**argv == 'C' && numCellsTaken == 0) {
         value = strtol(*argv + 1, &end, BASE);
         if (*end == '\0' && (numCellsTaken = 1)) {
            numCells = value;
         }
      }
      else if (**argv == 'S' && simTimeTaken == 0) {
         value = strtol(*argv + 1, &end, BASE);
         if (*end == '\0' && (simTimeTaken = 1)) {
            simTime = value;
         }
      }
      else if (**argv == 'L' && leftSet == 0) {
         value = strtod(*argv + 1, &end);
         if (*end == '\0' && (leftSet = 1)) {
            leftVal = value;
         }
      }
      else if (**argv == 'R' && rightSet == 0) {
         value = strtod(*argv + 1, &end);
         if (*end == '\0' && (rightSet = 1)) {
            rightVal = value;
         }
      }
   }
   InputErrorCheck(numCells, rightSet, simTimeTaken,
    numCellsTaken);

   pipe(parentPipe); 
   BranchCase(numCells, simTime, rightVal, leftVal, parentPipe);
   return 0;   
}

void ReadReports(int numCells, int simTime, int parentPipe[2], int *pids) {
   
   int data, report, reports[numCells], tooManyReports = 0,
    tooFewReports = 0, status, cleanExit = 0, index, processID, retVal = 0;
   Report reportIn;
   
   for (report = 0; report < numCells; report++) {
      reports[report] = 0;
   }
   for (index = 0; index < numCells; index++) {

      processID = wait(&status);
      cleanExit = CheckStatus(status, pids, processID, numCells);
      if (cleanExit) {
         retVal = 1;
      }
      while ((data = read(parentPipe[READ], &reportIn, sizeof(Report)))) {

         if (data < 0) {
            exit(EXIT_FAILURE);
         }
         (reports[reportIn.id]) += 1;         
         printf("Result from %d, step %d: %.3lf\n", reportIn.id, 
          reportIn.step, reportIn.value);
      }
   }  

   report = 0;
   while ((data = read(parentPipe[READ], &reportIn, sizeof(Report)))) {

      if (data < 0) {
         exit(EXIT_FAILURE); //Read threw an error
      }
      if (data == 0) {
         continue;
      }
      (reports[reportIn.id]) += 1;         
      printf("Result from %d, step %d: %.3lf\n", reportIn.id, reportIn.step, 
       reportIn.value);
   }
   for (report = 0; report < numCells; report++) {
      if (reports[report] > simTime + 1) {
         tooManyReports++;
      }
      else if (reports[report] < simTime + 1) {
         tooFewReports++;
      }
   }
   if (tooManyReports) {
      fprintf(stderr, "Error: %d cells reported too many reports\n",
       tooManyReports);
      retVal = 1;
   }
   if (tooFewReports) {
      fprintf(stderr, "Error: %d cells reported too few reports\n", 
       tooFewReports);
      retVal = 1;
   }
   if (retVal) {
      exit(EXIT_FAILURE);
   }
}

void BranchCase(int numCells, int simTime, double rightVal, 
 double leftVal, int parentPipe[2]) {
   
   char *simTimeS, *parentPipeWrite, *leftValString, 
    *rightValS;

   sprintf(simTimeS = (char *)calloc(1, MAX_CHARS), "%s%d", "S", simTime);

   sprintf(parentPipeWrite = (char *)calloc(1, MAX_CHARS), "%s%d", "O", 
    parentPipe[WRITE]);

   sprintf(leftValString = (char *)calloc(1, MAX_CHARS), "%s%lf", "V", 
    leftVal);
   sprintf(rightValS = (char *)calloc(1, MAX_CHARS), "%s%lf", "V", 
    rightVal);
   if (numCells == 1) {
      OneCell(parentPipe, simTimeS, parentPipeWrite, leftValString,
       numCells, simTime);
   }
   else if (numCells == 2) {
      TwoCells(parentPipe, simTimeS, parentPipeWrite, leftValString,
       numCells, simTime, rightValS);
   }
   else if (numCells == ONE_INNER_CELL) {
      OneInnerCell(parentPipe, simTimeS, parentPipeWrite, 
       leftValString, numCells, simTime, rightValS);
   }
   else if (numCells == TWO_INNER_CELLS) {
      TwoInnerCells(parentPipe, simTimeS, parentPipeWrite,
       leftValString, numCells, simTime, rightValS);
   }
   else if (numCells >= MANY_CELLS) {
      ManyCells(parentPipe, simTimeS, parentPipeWrite, 
       leftValString, numCells, simTime, rightValS);
   }
}

void ManyCells(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS) {
 
   int childPID, *pipeEnd = calloc(2, sizeof(int)), *pipeOldToNew1 = calloc(2,
    sizeof(int)), *pids = calloc(numCells, sizeof(int)), *pipeNewToOld1 = 
    calloc(2, sizeof(int)),  *pipeOldToNew2 = calloc(2, sizeof(int)), 
    *pipeNewToOld2 = calloc(2, sizeof(int)), index, cellsMade = 0;
   
   char *endWriteString = (char *)calloc(1, MAX_CHARS), *endReadString = (char 
    *)calloc(1, MAX_CHARS), *oldToNew1WriteS = 
    (char *)calloc(1, MAX_CHARS), *oldToNew1ReadS = (char *)calloc(1, 
    MAX_CHARS), *oldToNew2WriteS = (char *)calloc(1, MAX_CHARS), 
    *oldToNew2ReadS = (char *)calloc(1, MAX_CHARS), *newToOld1WriteS 
    = (char *)calloc(1, MAX_CHARS), *newToOld1ReadS = (char *)calloc(1, 
    MAX_CHARS), *newToOld2WriteS = (char *)calloc(1, MAX_CHARS), 
    *newToOld2ReadString = (char *)calloc(1, MAX_CHARS);
   
   MakePipe(&pipeEnd, &endWriteString, &endReadString);
   if ((pids[cellsMade++] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (left cell)
      close(parentPipe[READ]);
      close(pipeEnd[READ]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D0", 
       endWriteString, leftValString, NULL);
   }
   close(pipeEnd[WRITE]);
   MakePipe(&pipeOldToNew1, &oldToNew1WriteS, &oldToNew1ReadS);
   MakePipe(&pipeNewToOld1, &newToOld1WriteS, &newToOld1ReadS);
   
   if ((pids[cellsMade++] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (second leftmost cell)
      close(parentPipe[READ]);
      close(pipeOldToNew1[READ]);
      close(pipeNewToOld1[WRITE]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D1", 
       endReadString, oldToNew1WriteS, newToOld1ReadS, NULL);
   } 
   close(pipeEnd[READ]);
   close(pipeOldToNew1[WRITE]);
   close(pipeNewToOld1[READ]);
   for (index = cellsMade; index < numCells - 2; index++) {
      ManyCellsBranch(index, &pids, simTimeS, parentPipeWrite,
       &pipeOldToNew1, &pipeOldToNew2, &pipeNewToOld1, &pipeNewToOld2,
       &oldToNew1WriteS, &oldToNew1ReadS, &oldToNew2WriteS,
       &oldToNew2ReadS, &newToOld1WriteS, &newToOld1ReadS,
       &newToOld2WriteS, &newToOld2ReadString, parentPipe);
   }
   CleanupManyCells(index, &pids, simTimeS, parentPipeWrite,
    &pipeOldToNew1, &pipeOldToNew2, &pipeNewToOld1, &pipeNewToOld2, 
    &oldToNew1WriteS, &oldToNew1ReadS, &oldToNew2WriteS,
    &oldToNew2ReadS, &newToOld1WriteS, &newToOld1ReadS,
    &newToOld2WriteS, &newToOld2ReadString, numCells, simTime,
    parentPipe, &pipeEnd, &endWriteString, &endReadString, rightValS);

}

void ManyCellsBranch(int index, int **pids, char *simTimeS,
 char *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, 
 int **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, 
 char **oldToNew1ReadS, char **oldToNew2WriteS, 
 char **oldToNew2ReadS, char **newToOld1WriteS, 
 char **newToOld1ReadS, char **newToOld2WriteS,
 char **newToOld2ReadString, int *parentPipe) {
   
   if (index % 2) {
      ManyCellsEven(index, pids, simTimeS, parentPipeWrite,//50 here
       pipeOldToNew1, pipeOldToNew2, pipeNewToOld1, pipeNewToOld2,
       oldToNew1WriteS, oldToNew1ReadS, oldToNew2WriteS,
       oldToNew2ReadS, newToOld1WriteS, newToOld1ReadS,
       newToOld2WriteS, newToOld2ReadString, parentPipe);
   }
   else {
      ManyCellsOdd(index, pids, simTimeS, parentPipeWrite,
       pipeOldToNew1, pipeOldToNew2, pipeNewToOld1, pipeNewToOld2,
       oldToNew1WriteS, oldToNew1ReadS, oldToNew2WriteS,
       oldToNew2ReadS, newToOld1WriteS, newToOld1ReadS,
       newToOld2WriteS, newToOld2ReadString, parentPipe);
   }
}

void CleanupManyCells(int index, int **pids, char *simTimeS,
 char *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, 
 int **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, 
 char **oldToNew1ReadS, char **oldToNew2WriteS, char **oldToNew2ReadS, 
 char **newToOld1WriteS, char **newToOld1ReadS, char **newToOld2WriteS, 
 char **newToOld2ReadString, int numCells, int simTime, int parentPipe[], 
 int **pipeEnd, char **endWriteString, char **endReadString, char *rightValS) {
   
   int childPID;
   char *cellID;
   
   MakePipe(pipeEnd, endWriteString, endReadString);
   if (index % 2) {
      if (((*pids)[index++] = childPID = fork()) < 0) {
         fprintf(stderr, "Bad fork call\n"); 
      }
      else if (childPID == 0) {//Child code (second to right cell)
         close((parentPipe)[READ]);
         close((*pipeEnd)[WRITE]);
         sprintf(cellID = (char *)calloc(1, MAX_CHARS), "%s%d", "D",
          index - 1);
         execl("./Cell", "./Cell", parentPipeWrite, simTimeS, 
          cellID, *newToOld2WriteS, *oldToNew2ReadS, *endReadString, 
          NULL);
      }
      close((*pipeOldToNew2)[READ]);
      close((*pipeNewToOld2)[WRITE]);
      close((*pipeEnd)[READ]);
   }
   else {
      if (((*pids)[index++] = childPID = fork()) < 0) {
         fprintf(stderr, "Bad fork call\n"); 
      }
      else if (childPID == 0) {//Child code (second to right cell)
         close((parentPipe)[READ]);
         close((*pipeEnd)[WRITE]);
         sprintf(cellID = (char *)calloc(1, MAX_CHARS), "%s%d", "D", index - 
          1);
         execl("./Cell", "./Cell", parentPipeWrite, simTimeS, 
          cellID, *newToOld1WriteS, *oldToNew1ReadS, *endReadString, 
          NULL);
      }
      close((*pipeOldToNew1)[READ]);
      close((*pipeNewToOld1)[WRITE]);
      close((*pipeEnd)[READ]);
   }
   
   if (((*pids)[index] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (Rightmost cell)
      close(parentPipe[READ]);
      close((*pipeEnd)[READ]);
      sprintf(cellID = (char *)calloc(1, MAX_CHARS), "%s%d", "D", index);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, cellID,
       rightValS, *endWriteString, NULL);
   }
   
   close((*pipeEnd)[WRITE]);
   close((parentPipe)[WRITE]);
   
   ReadReports(numCells, simTime, parentPipe, *pids);   
}

void ManyCellsOdd(int index, int **pids, char *simTimeS, char
 *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, int 
 **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, char 
 **oldToNew1ReadS, char **oldToNew2WriteS, char 
 **oldToNew2ReadS, char **newToOld1WriteS, 
 char **newToOld1ReadS, char **newToOld2WriteS, 
 char **newToOld2ReadString, int *parentPipe) {
   
   char *cellIDString; 
   int childPID;
   
   MakePipe(pipeOldToNew2, oldToNew2WriteS, oldToNew2ReadS);
   MakePipe(pipeNewToOld2, newToOld2WriteS, newToOld2ReadString);
  
   if (((*pids)[index] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code
      sprintf(cellIDString = (char *)calloc(1, MAX_CHARS), "%s%d", "D", index);
      close(parentPipe[READ]);
      close((*pipeOldToNew2)[READ]);
      close((*pipeNewToOld2)[WRITE]);
      
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, 
       cellIDString, *oldToNew1ReadS, *newToOld1WriteS, 
       *newToOld2ReadString, *oldToNew2WriteS, NULL); 
   }
   close((*pipeOldToNew1)[READ]);
   close((*pipeNewToOld1)[WRITE]);
   close((*pipeOldToNew2)[WRITE]);
   close((*pipeNewToOld2)[READ]);
}

void ManyCellsEven(int index, int **pids, char *simTimeS, char
 *parentPipeWrite, int **pipeOldToNew1, int **pipeOldToNew2, int 
 **pipeNewToOld1, int **pipeNewToOld2, char **oldToNew1WriteS, char 
 **oldToNew1ReadS, char **oldToNew2WriteS, char 
 **oldToNew2ReadS, char **newToOld1WriteS, char 
 **newToOld1ReadS, char **newToOld2WriteS, char 
 **newToOld2ReadString, int *parentPipe) {
   
   char *cellIDString;
   int childPID;
   
   MakePipe(pipeOldToNew1, oldToNew1WriteS, oldToNew1ReadS);
   MakePipe(pipeNewToOld1, newToOld1WriteS, newToOld1ReadS); 
   if (((*pids)[index] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n"); //or other error
   }
   else if (childPID == 0) {//Child code
      sprintf(cellIDString = (char *)calloc(1, MAX_CHARS), "%s%d", "D", index);
      close(parentPipe[READ]);
      close((*pipeOldToNew1)[READ]);
      close((*pipeNewToOld1)[WRITE]);
      
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS,
       cellIDString, *oldToNew2ReadS, *newToOld2WriteS, 
       *newToOld1ReadS, *oldToNew1WriteS, NULL); 
   }
   close((*pipeOldToNew2)[READ]);
   close((*pipeNewToOld2)[WRITE]);
   close((*pipeOldToNew1)[WRITE]);
   close((*pipeNewToOld2)[READ]);
}
 
void MakePipe(int **pipeName, char **writeString, char **readString) {
   pipe(*pipeName);
   
   free(*writeString);
   free(*readString);
   sprintf(*writeString = (char *)calloc(1, MAX_CHARS), "%s%d", "O",
    (*pipeName)[WRITE]);
   sprintf(*readString = (char *)calloc(1, MAX_CHARS), "%s%d", "I",
    (*pipeName)[READ]);
}

void MakeOneTimePipe(int **pipeName, char **writeString, char **readString) {
   pipe(*pipeName);

   sprintf(*writeString = (char *)calloc(1, MAX_CHARS), "%s%d", "O",
    (*pipeName)[WRITE]);
   sprintf(*readString = (char *)calloc(1, MAX_CHARS), "%s%d", "I",
    (*pipeName)[READ]);
}

void TwoInnerCells(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells, 
 int simTime, char *rightValS) {

   int childPID, *pipeLtoLM = calloc(2, sizeof(int)), *pipeRtoRM = calloc(2,
    sizeof(int)), pids[TWO_INNER_CELLS], *pipeLMtoRM = calloc(2, sizeof(int)), 
    *pipeRMtoLM = calloc(2, sizeof(int));
   char *ltoLMWriteS, *ltoLMReadS, *lMtoRMWriteS, *lMtoRMReadS, *rMtoLMWriteS,
    *rMtoLMReadS, *rtoRMWriteS, *rtoRMReadS;
   
   MakeOneTimePipe(&pipeLtoLM, &ltoLMWriteS, &ltoLMReadS);

   if ((pids[0] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n"); //or other error
   }
   else if (childPID == 0) {//Child code (left cell)
      close(parentPipe[READ]);
      close(pipeLtoLM[READ]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D0",
       ltoLMWriteS, leftValString, NULL);
   }
   
   close(pipeLtoLM[WRITE]);
   MakeOneTimePipe(&pipeLMtoRM, &lMtoRMWriteS, &lMtoRMReadS);

   MakeOneTimePipe(&pipeRMtoLM, &rMtoLMWriteS, &rMtoLMReadS);

   if ((pids[1] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (left middle cell)
      close(parentPipe[READ]);
      close(pipeLMtoRM[READ]);
      close(pipeRMtoLM[WRITE]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D1",
       ltoLMReadS, rMtoLMReadS, lMtoRMWriteS, NULL);
   }
   close(pipeLtoLM[READ]);
   close(pipeLMtoRM[WRITE]);
   close(pipeRMtoLM[READ]);
   MakeOneTimePipe(&pipeRtoRM, &rtoRMWriteS, &rtoRMReadS);

   if ((pids[2] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (right middle cell)
      close(parentPipe[READ]);
      close(pipeRtoRM[WRITE]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D2",
       rtoRMReadS, lMtoRMReadS, rMtoLMWriteS, NULL);
   }
   
   close(pipeRtoRM[READ]);
   close(pipeLMtoRM[READ]);
   close(pipeRMtoLM[READ]);
   if ((pids[ONE_INNER_CELL] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n"); //or other error
   }
   else if (childPID == 0) {//Child code (right cell)
      close(parentPipe[READ]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D3",
       rtoRMWriteS, rightValS, NULL);
   }
   
   close(pipeRtoRM[WRITE]);
   close((parentPipe)[WRITE]);
   ReadReports(numCells, simTime, parentPipe, pids);

}

void OneInnerCell(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS) {
   
   int childPID, *pipeLtoM = calloc(2, sizeof(int)), *pipeRtoM = calloc(2,
    sizeof(int)), pids[ONE_INNER_CELL];
   char *ltoMWriteString, *ltoMReadString, *rtoMWriteString, *rtoMReadString;
      
   MakeOneTimePipe(&pipeLtoM, &ltoMWriteString, &ltoMReadString);

   if ((pids[0] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (left cell)
      close(parentPipe[READ]);
      close(pipeLtoM[READ]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D0",
       ltoMWriteString, leftValString, NULL);
   }
   
   close(pipeLtoM[WRITE]);
   MakeOneTimePipe(&pipeRtoM, &rtoMWriteString, &rtoMReadString);

   if ((pids[1] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID == 0) {//Child code (middle cell)
      close(parentPipe[READ]);
      close(pipeRtoM[WRITE]);
      execl("./Cell", "./Cell", parentPipeWrite, ltoMReadString,
       rtoMReadString, simTimeS, "D1", NULL); 
   }
   
   close(pipeLtoM[READ]);
   close(pipeRtoM[READ]);
   
   if ((pids[2] = childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n"); 
   }
   else if (childPID == 0) {//Child code (right cell)
      close(parentPipe[READ]);
      execl("./Cell", "./Cell", parentPipeWrite, rtoMWriteString,
       rightValS, simTimeS, "D2", NULL);
   }
   close(pipeRtoM[WRITE]);
   
   close((parentPipe)[WRITE]);
   ReadReports(numCells, simTime, parentPipe, pids);
}

void TwoCells(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime, char *rightValS) {
   
   int childPID, pids[2];
   
   if ((childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childPID > 0) {//Parent Code
      pids[0] = childPID;
      if ((childPID = fork()) < 0) {
         fprintf(stderr, "Bad fork call\n"); 
      }
      else if (childPID > 0) {//Parent Code after second fork
         pids[1] = childPID;
         close((parentPipe)[WRITE]);
      }
      else {//Child code after second fork
         close(parentPipe[READ]);
         execl("./Cell", "./Cell", parentPipeWrite, simTimeS, 
          "D1", rightValS, NULL);
      }     
   }
   else {//Child code
      close(parentPipe[READ]);
      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D0",
       leftValString, NULL); 
   }

   ReadReports(numCells, simTime, parentPipe, pids);      

}

void OneCell(int parentPipe[2], char *simTimeS,
 char *parentPipeWrite, char *leftValString, int numCells,
 int simTime) {
    
   int childPID, pids[1];

   if ((childPID = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n"); 
   }
   else if (childPID > 0) {//Parent Code
      pids[0] = childPID;

      close((parentPipe)[WRITE]);
   }
   else {//Child code
      close(parentPipe[READ]);

      execl("./Cell", "./Cell", parentPipeWrite, simTimeS, "D0",
       leftValString, NULL);
   }
   ReadReports(numCells, simTime, parentPipe, pids);      
}

int CheckStatus(int status, int pids[], int processID, int numCells) {
   
   int index, reportID = 0;
   
   for (index = 0; index < numCells; index++) {
      if (pids[index] == processID) {
         reportID = index;
      }
   }
   if (status != 0) {
      fprintf(stderr, "Error: Child %d exited with %d\n", reportID, 
       status / STATUS_MULT);
      return 1;
   }
   return 0;
}

void InputErrorCheck(int numCells, int rightSet, int simTimeTaken,
 int numCellsTaken) {
   
   if ((rightSet && numCells < 2) || !numCellsTaken || !simTimeTaken) {
      fprintf(stderr, "Usage: LinearSim C S L R (in any order)\n");
      exit(EXIT_FAILURE);
   }

}
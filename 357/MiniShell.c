#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_WORD_LEN 110
#define WORD_FMT "%110s"

char **allCommands;
int superCounter; 	 	
void history();

typedef struct Arg {
   char value[MAX_WORD_LEN+1];
   struct Arg *next;
} Arg;

typedef struct Command {
   int numArgs;
   Arg *args;
   char inFile[MAX_WORD_LEN+1];
   char outFile[MAX_WORD_LEN+1]; 
   struct Command *next;
} Command;

static Arg *NewArg(char *str) {
   Arg *rtn = malloc(sizeof(Arg));

   strncpy(rtn->value, str, MAX_WORD_LEN);
   rtn->next = NULL;

   return rtn;
}

static Command *NewCommand(char *cmd) {
   Command *rtn = malloc(sizeof(Command));

   rtn->numArgs = 1;
   rtn->args = NewArg(cmd);

   rtn->inFile[0] = rtn->outFile[0] = '\0';
   rtn->next = NULL;

   return rtn;
}

static Command *DeleteCommand(Command *cmd) {
   Arg *temp;

   while (cmd->args != NULL) {
      temp = cmd->args;
      cmd->args = temp->next;
      free(temp);
   }
   return cmd->next;
}

static Command *ReadCommands(FILE *in) {
   int nextChar;
   char nextWord[MAX_WORD_LEN+1], place[MAX_WORD_LEN+1];
   Command *rtn, *lastCmd;
   Arg *lastArg;

   if (1 == fscanf(in, WORD_FMT, nextWord)) {
      if (!strcmp(nextWord, "cd")) {
	fscanf(in, WORD_FMT, place);
	chdir(place);
	}
	else if(!strcmp(nextWord, "history")) {
		history();
	}
	allCommands[superCounter] = calloc(MAX_WORD_LEN, sizeof(char));
	strcpy(allCommands[superCounter++], nextWord);
	rtn = lastCmd = NewCommand(nextWord);
      lastArg = lastCmd->args;
   }
   else
      return NULL;

   do {
      while ((nextChar = getc(in)) == ' ') 
         ;

      if (nextChar != '\n' && nextChar != EOF) {
      
         if (nextChar == '|') { 
            if (1 == fscanf(in, WORD_FMT, nextWord)) {
               lastCmd = lastCmd->next = NewCommand(nextWord);
               lastArg = lastCmd->args;
            }
         }
         else {
            ungetc(nextChar, in);  
            fscanf(in, WORD_FMT, nextWord);
            printf("the next word is |%s|\n", nextWord);
            if (!strcmp(nextWord, "<")) 
               fscanf(in, WORD_FMT, lastCmd->inFile);
            else if (!strcmp(nextWord, ">"))
               fscanf(in, WORD_FMT, lastCmd->outFile);
            else {
               lastArg = lastArg->next = NewArg(nextWord);
               lastCmd->numArgs++;
            }
         }
      }
   } while (nextChar != '\n' && nextChar != EOF);

   return rtn;
}

static void RunCommands(Command *cmds) {
   Command *cmd;
   char **cmdArgs, **thisArg;
   Arg *arg;
   int childPID, cmdCount = 0;
   int pipeFDs[2]; 
   int outFD = -1; 
   int inFD = -1;  

   for (cmd = cmds; cmd != NULL; cmd = cmd->next) {
      if (inFD < 0 && cmd->inFile[0]) 
         inFD = open(cmd->inFile, O_RDONLY);

      if (cmd->next != NULL) 
         pipe(pipeFDs);
      
      if ((childPID = fork()) < 0) 
         fprintf(stderr, "Error, cannot fork.\n");
      else if (childPID) {   
         cmdCount++;
         close(inFD);           
         if (cmd->next != NULL) {
            close(pipeFDs[1]);  
            inFD = pipeFDs[0];  
         }
      }
      else {  
         if (inFD >= 0) {  
            dup2(inFD, 0);
            close(inFD);
         }
      
         outFD = -1;     
         if (cmd->next != NULL) {  
            outFD = pipeFDs[1];      
            close(pipeFDs[0]);    
         }
         if (outFD < 0 && cmd->outFile[0])  
            outFD = open(cmd->outFile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
         
         if (outFD >= 0) {
            dup2(outFD, 1);
            close(outFD);
         }

         cmdArgs = thisArg = calloc(sizeof(char *), cmd->numArgs+1);
         for (arg = cmd->args; arg != NULL; arg = arg->next)
            *thisArg++ = arg->value;

         execvp(*cmdArgs, cmdArgs);
      }
   }

   while (cmdCount--)
      wait(NULL);
}
void history() {
	int i;
	for (i = 0; i < superCounter; i++) {
		printf("%s\n", allCommands[i]);
	}
}	

int main() {
   Command *cmds;
	allCommands = (char **)calloc(MAX_WORD_LEN, sizeof(char *));
	superCounter = 0;
   while (!feof(stdin)) {
      // printf(">> ");
      if (NULL != (cmds = ReadCommands(stdin))) {
         RunCommands(cmds);
      }
      while (cmds != NULL) {
         cmds = DeleteCommand(cmds);
      }
   }
}

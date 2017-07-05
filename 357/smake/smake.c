#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include "smake.h"
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define INIT_CAPACITY 10
//#define DOUBLE 2

int main(int argc, char *argv[]) {

   FILE *filePointer;
   char * line = NULL;
   Rule **rules = calloc(INIT_CAPACITY, sizeof(Rule *));
   int numRules = 0;
   Rule *currRule = NULL;
   int ruleIndex = 0;
   
   if ((filePointer = fopen("Smakefile", "r")) == NULL) {
      exit(-1);
   }
   
   numRules = ParseFile(filePointer, &rules);
   
   rewind(filePointer); 
   
   ExecuteArgs(argc, ruleIndex, rules, argv, numRules, currRule, 0, 0, 0);
   
   fclose(filePointer);
   if (line) {
      free(line);
   }
   exit(EXIT_SUCCESS);
    
}

int ParseFile(FILE *filePointer, Rule ***rules) {
   char *token, *spareToken, *string, *originalString, *spareString;
   char * line = NULL;
   int ruleCapacity = INIT_CAPACITY;
   size_t length = 0;
   ssize_t read;
   int numRules = 0;
   Rule *currRule;
   int noEnterthenTab = 0;
   
   while ((read = getline(&line, &length, filePointer)) != -1) {
      string = strdup(line);
      spareString = strstr(string, ":");
      originalString = strdup(line);
      
      if (string[0] == '\t') { 
         if ((numRules == 0 || noEnterthenTab == 2) && (noEnterthenTab = 0)) {
            exit(-1);
         }
         string[strlen(string) - 1] = '\0'; 
         currRule->actions[currRule->numActions] = strdup(string + 1); 
         if (currRule->numActions > currRule->capacityActions - 2) {
            currRule->actions = realloc(currRule->actions, 
             (currRule->capacityActions *= 2) * sizeof(currRule->actions));
         }
         currRule->numActions++;
      }
      else if (string[0] != '\n')  {
         noEnterthenTab = StringCheck(spareString);
         token = strtok(string, " \n:");
         if (token) {
            spareToken = strdup(token);
            (*rules)[numRules] = MakeRule(token, numRules);
            currRule = (*rules)[numRules++];
            if (numRules > ruleCapacity - 2) {
               *rules = realloc(*rules, (ruleCapacity *= 2)
                * sizeof(*(*rules)));
            }
            token = strtok(NULL, " \n:");
            spareString = strstr(originalString, ":");
            if (token == NULL) {
               continue;               
            }
            spareString = strstr(spareString, token);
             
            if (spareString == NULL) {
               exit(-1);
            }
            while (token) {
               currRule->depends[currRule->numDepends] = strdup(token);
               if (currRule->numDepends > currRule->capacityDepends - 2) {
                  currRule->depends = realloc(currRule->depends, 
                   (currRule->capacityDepends *= 2) * 
                   sizeof(currRule->depends));
               }
               noEnterthenTab = 1; 
               currRule->numDepends++;
               token = strtok(NULL, " \n");
            }
         }
         else {
            exit(-1);
         }                       
      }
      else {
         if (noEnterthenTab == 1 && (noEnterthenTab = 2)) {
         }
      }       
   }
   return numRules;
}

int StringCheck(char *spareString) {
   if (spareString++ == NULL) {
      exit(-1);
   }
   spareString = strstr(spareString, ":");
   if (spareString != NULL) {
      exit(-1);
   }
   return 0;
}



void ExecuteArgs(int argc, int ruleIndex, Rule **rules, 
 char *argv[], int numRules, Rule *currRule, int ndx, int flag, int temp) {
  
   if (argc == 2) {
      ruleIndex = FindRule(rules, argv[1], numRules);
      if (ruleIndex == -1) {
         exit(-1);
      }
      currRule = rules[ruleIndex];
   }
   else if (argc == 1) {
      currRule = rules[0];
   }
   else {
      exit(-1);
   }
   for (ndx = 0; ndx < currRule->numDepends; ndx++) {
      if (ExecuteDepends(rules, numRules, currRule->ruleIndex, 
       ndx, 0, currRule->ruleIndex)) {
         flag = 1;
      } 
   }
   if (flag || currRule->numDepends == 0) {
      for (ndx = 0; ndx < rules[currRule->ruleIndex]->numActions; ndx++) {
         printf("%s\n", rules[currRule->ruleIndex]->actions[ndx]);
         int temp6 = system(rules[currRule->ruleIndex]->actions[ndx]);
         if (temp6) {
            exit(-1);
         }      
      }
   }

}

struct Rule *MakeRule(char *token, int numRules)
{
   struct Rule *rule = calloc(1, sizeof(Rule));
   
   rule->name = strdup(token);
   rule->ruleIndex = numRules;
   rule->actions = calloc(INIT_CAPACITY, sizeof(char *));
   rule->depends = calloc(INIT_CAPACITY, sizeof(char *));
   rule->capacityDepends = INIT_CAPACITY;
   rule->capacityActions = INIT_CAPACITY;
   rule->numDepends = 0;
   rule->numActions = 0;
   
   return rule;
}

int FindRule(Rule **rulebook, char *name, int numRules) {

   int ndx;
   for (ndx = 0; ndx < numRules; ndx++) {
      if (strcmp(rulebook[ndx]->name, name) == 0)
      {
         return ndx;
      }
   }
   return -1;
}

int ExecuteDepends(Rule **rules, int numRules, int indexRule, 
 int indexDepends, int updated, int rootIndex) { 
   Rule *currRule = rules[indexRule];
   int ndx = 0;
   time_t targetTime;
   int executeMe = 0;   
   int temp = 0;
   int originalUpdated = updated;
   int temp6;
   struct stat targetBuffer; 
   int validDepends = 0;
   struct stat buffer;

   
   if (currRule->numDepends == 0) {
      if (rootIndex != indexRule) {
         for (ndx = 0; ndx < rules[indexRule]->numActions; ndx++) {
            printf("%s\n", rules[indexRule]->actions[ndx]);
            temp6 = system(rules[indexRule]->actions[ndx]);
            if (temp6) {
               exit(-1);
            }     
         }
      }
     return updated + 1;
   }
   
   if (stat(currRule->name, &targetBuffer))
   {
      if (rootIndex != indexRule) {
         executeMe = 1;
      } 
      updated++;
   }
   targetTime = targetBuffer.st_mtime;   
   
   temp = FindRule(rules, currRule->depends[indexDepends], numRules);
   
   if (temp != -1) {
      validDepends = 1;
      updated += ExecuteDepends(rules, numRules, temp, 0, updated, 
       rootIndex);
      for (ndx = 1; ndx < rules[temp]->numDepends; ndx++) {
         updated += ExecuteDepends(rules, numRules, temp, ndx, updated,
          rootIndex);
       }
         if (originalUpdated != updated) {
            executeMe = 1; 
         } 
   }
   
   if (FileExist(currRule->depends[indexDepends])) { 
      validDepends = 1;
      if (stat(currRule->depends[indexDepends], &buffer))
      {
         exit(-1);
      }
      if (difftime(buffer.st_mtime, targetTime) > 0) {
         updated++;
         executeMe = 1;
      } 
   }
   ExecuteEnd(validDepends, executeMe, indexRule, rootIndex, ndx, rules, 
    numRules, updated, 0, indexDepends);
   return updated; 
}

void ExecuteEnd(int validDepends, int executeMe, int indexRule,
 int rootIndex, int ndx, Rule **rules, int numRules, int updated, 
 int temp, int indexDepends) {
   if (validDepends != 1) {
      exit(-1);
   }
   if (executeMe && indexRule != rootIndex) {
      for (ndx = indexDepends + 1; ndx < rules[indexRule]->numDepends;
       ndx++) {
         ExecuteDepends(rules, numRules, indexRule, 
          ndx, updated, rootIndex);
      } 
      for (ndx = 0; ndx < rules[indexRule]->numActions; ndx++) {
         printf("%s\n", rules[indexRule]->actions[ndx]);
         temp = system(rules[indexRule]->actions[ndx]);
         if (temp) {
            exit(-1);
         }      
      }
   }
}  
//Function for checking file Exist adapted from 
//http://stackoverflow.com/questions/230062/
//whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
int FileExist(char *filename)
{
   struct stat buffer;   
   return (stat(filename, &buffer) == 0);
}
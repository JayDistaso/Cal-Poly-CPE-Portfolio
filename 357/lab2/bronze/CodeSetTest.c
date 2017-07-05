#include <stdio.h>
#include "SmartAlloc.h" // #include "SmartAlloc.h" if not on IHS
#include "CodeSet.h"

#define NUM_POINTERS 3
#define CHAR_PTR_SIZE 4
#define CODEENTRY_32BIT 35
#define CODEENTRY_64BIT 45

/* Checking Space */
int CheckUsedSpace() {
   // Allocated Space Checking
   unsigned long space, codeSetSpace, codeEntrySpace;

   void *spaceCheck = CreateCodeSet(0);
   codeSetSpace = report_space();
   DestroyCodeSet(spaceCheck);

   spaceCheck = CreateCodeSet(1);
   codeEntrySpace = report_space() - codeSetSpace;
   DestroyCodeSet(spaceCheck);

   // CodeEntry Check
   if (sizeof(char *) == CHAR_PTR_SIZE) {
      if (codeEntrySpace > CODEENTRY_32BIT) { 
         // Ensures the size of CodeEntry is less than 40.
         fprintf(stderr, "CodeEntry size must be less than 35.\n");
         return 1;
      }
   }
   else {
      if (codeEntrySpace > CODEENTRY_64BIT) { 
         // Ensures the size of CodeEntry is less than 50.
         fprintf(stderr, "CodeEntry size must be less than 45.\n");
         return 1;
      }
   }
   return 0;
}

/*Given tests*/
void GivenCases(void *cs[NUM_POINTERS], char command);

int main() {

   char command;
   void *cs[NUM_POINTERS];  // Array of three void pointers
   int ndx, set, val;
   Code code;
   
   while (EOF != scanf(" %c", &command)) {
      switch (command) {
      case 'E': // extend existing code   E <set> <codeIndex>
         

scanf("%d%d", &set, &ndx);
printf("Added extended code %d\n", ExtendCode(cs[set], ndx));

         break;

      case 'S': // set suffix             S <set> <codeIndex> <suffix>
         

scanf("%d%d%d", &set, &ndx, &val);
SetSuffix(cs[set], ndx, val);

         break;

      case 'G': // get code               G <set> <codeIndex>
         

scanf("%d%d", &set, &ndx);                        
printf("Code %d in set %d has value:", ndx, set); 
code = GetCode(cs[set], ndx);                     
for (ndx = 0; ndx < code.size; ndx++) {           
   printf(" %d", code.data[ndx]);                 
}                                                 
printf("\n");                                     


         break;

	case '(':
	printf("This is the breakp\n");
	break;

      case 'F':
         

scanf("%d%d", &set, &ndx);
FreeCode(cs[set], ndx);

         break;

      default:
         GivenCases(cs, command);


      }
   }

   return CheckUsedSpace();
}

void GivenCases(void *cs[NUM_POINTERS], char command) {
   int ndx, numCodes, set, val, depth;
   unsigned long add;
   static unsigned long space = 0;

   switch (command) {

   case 'C': // create new codeSet     C <set> <maxCodes>
      scanf("%d%d", &set, &numCodes);
      cs[set] = CreateCodeSet(numCodes);
      printf("Set %d initialized\n", set);
      break;

   case 'N': // add new code           N <set> <value>
      scanf("%d%d", &set, &val);
      printf("New code in set %d at index %d\n", set, NewCode(cs[set], val));
      break;

   case 'R': // Report current space
      printf("Current space: %ld\n", report_space());
      break;

   case 'B': // Grab current space used
      space = report_space();
      break;

   case '+': // Adds an amount to the current used space.
      scanf("%lu", &add);
      space += add;
      break;  

   case '-': // Decreases an amount to the current used space.
      scanf("%lu", &add);
      space -= add;
      break;

   case 'L': // Call limit space
      limit_space(space);
      break;

   case 'X': // Add many codes in prefix order  X <set> <depth>
      scanf("%d%d", &set, &depth);

      ndx = NewCode(cs[set], 'X');
      while (--depth) {
         ndx = ExtendCode(cs[set], ndx);
         SetSuffix(cs[set], ndx, 'X');
      }
      break;

   case 'D': // Destroy existing code set  D <set>
      scanf("%d", &set);
      DestroyCodeSet(cs[set]);
      printf("Set %d destroyed\n", set);

   }
}

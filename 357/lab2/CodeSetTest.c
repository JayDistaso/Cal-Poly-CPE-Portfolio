#include <stdio.h>
#include "SmartAlloc.h" // #include "SmartAlloc.h" if not on IHS
#include "CodeSet.h"

#define NUM_POINTERS 3

int main() {
   Code code;
   char command;
   void *cs[NUM_POINTERS];  // Array of three void pointers
   int ndx, numCodes, val, set, depth;

   while (EOF != scanf(" %c", &command)) {
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

      case 'E': // extend existing code   E <set> <codeIndex>
         

         scanf("%d%d", &set, &val);
         printf("Added extended code %d\n", ExtendCode(cs[set], val));
         
         break;

      case 'S': // set suffix             S <set> <codeIndex> <suffix>
         

         scanf("%d%d%d", &set, &ndx, &val);
         SetSuffix(cs[set], ndx, val);
         
         
         break;

      case 'G': // get code               G <set> <codeIndex>
         

         depth = 0; //ask about this                        // Base: 3, Surcharge: 0
         scanf("%d%d", &set, &ndx);                         // Base: 8, Surcharge: 0
         printf("Code %d in set %d has value:", ndx, set); // Base: 6, Surcharge: 0
         code = GetCode(cs[set], ndx);                      // Base: 8, Surcharge: 0
         for (; depth < code.size; depth++) {               // Base: 7, Surcharge: 2.2
            printf(" %d", code.data[depth]);                // Base: 7, Surcharge: 0
         }                                                 
         printf("\n");                                      // Base: 4, Surcharge: 0
         
         
         break;

      case 'F':
         /* NOT USED FOR THIS IMPLEMENTATION */
         break;

      case 'R': // Report current space
         printf("Current space: %ld\n", report_space());
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

   return 0;
}

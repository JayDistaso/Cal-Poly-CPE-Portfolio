#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "SmartAlloc.h" // #include "SmartAlloc.h" if not on IHS

void PrintArgs(char **args) {
   printf("%s\n", *args);
   while (*args)
      printf("%s\n", *args++);
}

void BurnArgs(char **args, int argc) {
   while (argc--) {
      **args = 'X';
      *args++ = NULL;
   }
}


void CopyArgs(char **argv, char ***dupArgs) { // Base: 4, Surcharge: 0
   //printf("0 Here\n");
   //char **temp = *dupArgs;
   *dupArgs = calloc(1, sizeof(argv));
                                              
   while (*(++argv)) {
      //printf("1 argv %s ", *argv);                
      **dupArgs = calloc(1, sizeof(*argv)); // Base: 10, Surcharge: 0
      strcpy(**dupArgs, *argv);    
      //printf("dupArgs %s\n", **dupArgs);            
      *dupArgs++;                             // Base: 3, Surcharge: 0
   }                                          

}                                             



int main(int argc, char **argv) {
   char **dupArgs;
   
   CopyArgs(argv, &dupArgs);
   BurnArgs(argv, argc);
   PrintArgs(dupArgs);
//   printf("Space: %ld\n", report_space());

   return 0;
}



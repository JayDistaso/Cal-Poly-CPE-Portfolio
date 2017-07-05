
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <SmartAlloc.h> // #include "SmartAlloc.h" if not on IHS

void PrintArgs(char **args) {
	printf("%p\n",*args);
   while (*args)
      printf("%s\n", *args++);
}

void BurnArgs(char **args, int argc) {
   while (argc--) {
      **args = 'X';
	printf("args is kill = %p\n", *args);
      *args++ = NULL;
   }
	printf("done burning\n");
}


void CopyArgs(char ***dup, char **argv) { // Base: 4, Surcharge: 0
   //argv = pointer to pointer array to string
   //*argv pointer to string (in array)
   //**argv string              (object of point)
   //***char in string  
   *dup = calloc(1, sizeof(argv));
	printf("main calloc is %p\n", *dup);                   
   while (*(++argv)) {                        // Base: 5, Surcharge: 3.2
      **dup = (char*)calloc(1, sizeof(char*));
	printf("subcalloc is %p\n", **dup);
	printf("***dup, **dup, *dup, dup, %p\n%p\n%p\n%p\n", ***dup, **dup, *dup, dup);
	printf("going to copy %s\n", *argv);
      strcpy(**dup, *argv);
	printf("*dup = %p\n", *dup);                    // Base: 7, Surcharge: 0
      (*dup)++;                           // Base: 3, Surcharge: 0     // Base: 3, Surcharge: 0
   }
printf("done copying!\n");                                       
}                                          



int main(int argc, char **argv) {
   char **dupArgs;
   
   CopyArgs(

&dupArgs, argv
);
   BurnArgs(argv, argc);
   PrintArgs(dupArgs);
   //printf("Space: %ld\n", report_space());

   return 0;
}

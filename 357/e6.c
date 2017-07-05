#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   char *firstArg = argv[0];
   char *secondArg = argv[1];
printf("argc = %d\n", argc);
   if (argc == 2) {
      printf("Done. %s %s %s", secondArg, secondArg, secondArg); // Too complex to repeat in open spot
      return 0;
   }   
   else {
      printf("Not yet...\n");
      

execl(firstArg, secondArg, NULL); // Base: 4, Surcharge: 0


   }   
   printf("Error\n");
   return 1;
}

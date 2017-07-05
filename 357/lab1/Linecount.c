#include <stdio.h> //main library
#include <string.h>
void main(int argc, char *argv[]) {
   FILE *ifp; //arrays for file
   char filename[100];
   int i = 1;
   char c;
   int charcount;
   int linecount;
   for (; i<argc; i++) {//Repeat once per file
      ifp = fopen(argv[i], "r");
      linecount = 1;
      while ((c = fgetc(ifp)) != EOF){//while we arent at EOF keep reading
         charcount++;
         if(c == '\n'){
            if(charcount > 80){
               printf("In %s, line %d has a line length of %d\n", argv[i], linecount, charcount); 
            }
            linecount++;
            charcount = 0;  
         }
      }
   } 
}
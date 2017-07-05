// Move the target of "loc", forward integer by integer,
// for as long as the integers form a rising or falling
// run.  Stop at "stop", regardless.
#include <stdio.h>
void NextRun(int **loc, int *stop) {
   int prior, rising;
                               
   if ((*loc) == stop) {       
      return;                  
   }                           
                               
   prior = **loc; //prior is set to first in sequence
   (*loc)++;                   
   rising = (prior <= **loc);                           
   prior = **loc;              
   while (*loc != stop) {      
      (*loc)++;                
      if (rising) { //case for rising runs
         if (prior > **loc) {  
            return;            
         }                     
      }                        
      else {//case for falling runs
         if (prior <= **loc) { 
            return;            
         }                     
      }                        
      prior = **loc;           
   } 
}

int main() {
   int i, vals[100], numVals, *p1, *p2;
   
   printf("Enter count (at most 100) and that many values: ");
   scanf("%d", &numVals);
   for (i = 0; i < numVals; i++)
      scanf("%d", &vals[i]);

   p2 = p1 = vals;
   while (p2 < vals + numVals) {
      p1 = p2;
      NextRun(&p2, vals+numVals);
	  while (p1 < p2)
	     printf("%d ", *p1++);
	  printf("\n");
   }
}
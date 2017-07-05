#include <stdio.h>

#define MAX 100

// One pair is "less" than another if either its val1 is less than the other's val1, or if
// val1's are tied, then if its val2 is less than the other's val2.
typedef struct {
   int val1;
   int val2;
} Pair;

// Takes in two Pairs and returns 1 if the first Pair is "greater" than the second Pair. 
// else it returns 0


int CompPair(Pair *a, Pair *b) { // Base: 4, Surcharge: 0
   if (a->val1 > b->val1) {      // Base: 5, Surcharge: 0
      return 1;                  // Base: 11, Surcharge: 0
   }                             
   return (a->val2 > b->val2);   // Base: 3, Surcharge: 0
}                                      



// Swap the contents of the first Pair with the second Pair


void Swap(Pair *a, Pair *b) { // Base: 4, Surcharge: 0
   Pair temp = *a;            // Base: 4, Surcharge: 0
   *a = *b;                   // Base: 5, Surcharge: 0
   *b = temp;                // Base: 4, Surcharge: 0
}                             



void InsertSort(Pair vals[], int size) {
   Pair *next = vals + 1, *loc, *pLoc, *valsEnd = vals + size;

   while (next < valsEnd) {
      pLoc = next - 1;
      loc = next++;
      

      while (loc > vals && CompPair(pLoc, loc)) { // Base: 10, Surcharge: 2.8
         Swap(pLoc--, loc--);    // Base: 6, Surcharge: 0
      }                                           
      
      
   }
}

int main() {
   Pair vals[MAX];
   int numVals, ndx;

   scanf("%d", &numVals);
   for (ndx = 0; ndx < numVals; ndx++)
      scanf("%d %d", &vals[ndx].val1, &vals[ndx].val2);

   InsertSort(vals, numVals);

   for (ndx = 0; ndx < numVals; ndx++)
      printf("(%d, %d) ", vals[ndx].val1, vals[ndx].val2);
   printf("\n");

   return 0;
}

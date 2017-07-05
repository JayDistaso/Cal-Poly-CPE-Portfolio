#include <stdio.h>

#define DIM 8

int IsLess(double v1, double v2) {
   return v1 < v2;
}


#include <stdlib.h>                   
int IsGreater(double v1, double v2) { // Base: 4, Surcharge: 0
   return v1 > v2;                    // Base: 3, Surcharge: 0
}                                     
                                      
int IsAbsLess(double v1, double v2) { // Base: 4, Surcharge: 0
   return abs(v1) < abs(v2);          // Base: 7, Surcharge: 0
}                                     



// Iterate through "vals" (by incrementing vals itself for conciseness) to
// find the best value, as defined by parameter "better".  Assume "dim"
// is at least 1.  Careful definition of "better" will be essential.
double FindBest(double vals[], int dim, 

int (*f)(double, double)
) {
   double best, *end = vals + dim;
   
   best = *vals;
   

while (*vals) {          // Base: 4, Surcharge: 3
   if (f(*vals, best)) { // Base: 7, Surcharge: 0
      best = *(vals);  // Base: 4, Surcharge: 0
   }                     
   vals++;               // Base: 2, Surcharge: 0
}                        


   return best;
}

int main() {
   double vals[DIM];
   int i;
   
   for (i = 0; i < DIM; i++)
      scanf("%lf", vals + i);
     
   printf("Min value %0.3f\n", FindBest(vals, DIM, IsLess));
   printf("Max value %0.3f\n", FindBest(vals, DIM, IsGreater));
   printf("Lowest absolute value %0.3f\n", FindBest(vals, DIM, IsAbsLess));

   return 0;
}

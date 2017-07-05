#include <stdio.h>

#define SwapInts (x, y){int *tmp = &(x);int *tmp1 = &(y);*tmp = *tmp1;*tmp1 = (x);}

   int vals[] = {10, 4, 20000000, 1, 10000000, 0}, i = 3, j = 1;
   
   SwapInts(vals[++i], vals[++j]);
   SwapInts(vals[i], i);
   SwapInts(vals[j], j);
   SwapInts(vals[vals[5]], vals[5]);
   SwapInts(vals[3], vals[vals[3]]);
   
   printf("i: %d j: %d vals:", i, j);
   for (i = 0; i < 6; i++)
      printf(" %d", vals[i]);
   printf("\n");

   return 0;
}


/*
 * Copyright Software Inventions 2014
 */

#include <stdio.h>
#include "SmartAlloc.h" // #include "SmartAlloc.h" if not on IHS
 // Remove if not on IHS and function call below.

#define UINT_SIZE 32
#define CHAR_SIZE 8 

typedef unsigned char UChar;
typedef unsigned int UInt;

typedef struct {
   UInt packedData;
   int bitsUsed;
} BitPacker;

//BpInit


void BpInit(BitPacker *bp) { // Base: 3, Surcharge: 0
   bp->packedData = 0;       // Base: 3, Surcharge: 0
   bp->bitsUsed = 0;         // Base: 3, Surcharge: 0
}                            



//BpPack


void BpPack(BitPacker *bp, UChar c, int last) { // Base: 5, Surcharge: 0                        // Base: 5, Surcharge: 0
                                                
   bp->packedData |= (c << bp->bitsUsed);       // Base: 5, Surcharge: 0
   printf("Now it is %d\n", bp->packedData);    // Base: 5, Surcharge: 0
   bp->bitsUsed += CHAR_SIZE;                   // Base: 3, Surcharge: 0
   if (last || bp->bitsUsed == UINT_SIZE) {     // Base: 7, Surcharge: 0     // Base: 4, Surcharge: 0
      printf("%u", bp->packedData);           
   }                                            
}                                               



int main() {
   int count;
   UChar data;
   BitPacker bp; 



   BpInit(&bp);
   
   scanf("%d", &count);

   while (count--) {
      scanf(" %c", &data);
      BpPack(&bp, data, count == 0);
   }   
   printf("\n");
   return 0;
}

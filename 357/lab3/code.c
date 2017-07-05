#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UINT_SIZE 32
#define UINT_MASK 0xFFFFFFFF

typedef unsigned int UInt;

typedef struct {
   UInt curData;
   UInt nextData;
   int bitsLeft;
   int validNext;
} BitUnpacker;

//BuInit

void BuInit(BitUnpacker *bu) { // Base: 3, Surcharge: 0
   bu->curData = 0;            // Base: 3, Surcharge: 0
   bu->nextData = 0;           // Base: 3, Surcharge: 0
   bu->bitsLeft = 0;           // Base: 3, Surcharge: 0
   bu->validNext = 0;          // Base: 3, Surcharge: 0
}                              



//BuTakeData

void BuTakeData(BitUnpacker *bu, UInt next) { // Base: 4, Surcharge: 0
   bu->nextData = next;                       // Base: 3, Surcharge: 0
   bu->validNext = 1;                         // Base: 3, Surcharge: 0
}                                             



/*BuUnpack
 * return 1 on successful, 0 for needing next block
 */

int BuUnpack(BitUnpacker *bu, int size, UInt *ret) {                         // Base: 5, Surcharge: 0
   UInt temp, temp1;                                                  
   int left; //flag = bu->validNext;                                  
   if (bu->bitsLeft <= 0 && bu->validNext) {                                 // Base: 7, Surcharge: 0
      bu->curData = bu->nextData;                                            // Base: 3, Surcharge: 0
      bu->bitsLeft = UINT_SIZE;                                              // Base: 3, Surcharge: 0
      bu->validNext = 0;                                                     // Base: 3, Surcharge: 0
   }                                                                  
   if (bu->bitsLeft) {//thing to unpack spans blocks                  // Base: 3, Surcharge: 0
      left = size - bu->bitsLeft;                         // Base: 5, Surcharge: 0
      if (left > 0 && bu->validNext) {//spans multiple blocks         // Base: 7, Surcharge: 0
         ///if (flag) {                                               
           // return 0;                                               
         //}                                                          
         printf("i span two blocks \n");                            
         temp = (bu->curData & ((1 << bu->bitsLeft) - 1)) << left;           // Base: 11, Surcharge: 0
         printf("temp = %X\n", temp);      
         bu->curData = bu->nextData;                                         // Base: 3, Surcharge: 0
         //printf("after changing next data = %X\n", bu->nextData);     
                                                                      
         bu->bitsLeft = UINT_SIZE;                                           // Base: 3, Surcharge: 0
         bu->validNext = 0;                                           // Base: 3, Surcharge: 0
         bu->bitsLeft -= left;                                                  // Base: 3, Surcharge: 0
         temp1 = (bu->curData & ((1 << left) - 1) << (bu->bitsLeft)); // Base: 11, Surcharge: 0
         *ret = temp |= (temp1 >> (bu->bitsLeft));                     // Base: 11, Surcharge: 0
      }                                                               
      else if (left <= 0) {                                                                 // Base: 4, Surcharge: 0
         bu->bitsLeft -= size;                                               // Base: 3, Surcharge: 0
         temp = bu->curData & ((UINT_MASK >> (UINT_SIZE - size))             // Base: 11, Surcharge: 0
          << (bu->bitsLeft));                                         
         *ret = temp >> bu->bitsLeft;                              // Base: 6, Surcharge: 0
      }                                                               
      else {                                                          // Base: 1, Surcharge: 0
         return 0;                                                    // Base: 11, Surcharge: 0
      }                                                               
      return 1;                                                              // Base: 11, Surcharge: 0
   }                                                                  
   return 0;                                                                 // Base: 1, Surcharge: 0
}                                                                    



int main() {
   BitUnpacker bu;
   int numBlocks, size, i;
   UInt ret, *temp;


   scanf("%d", &numBlocks);
   temp = calloc(sizeof(UInt), numBlocks);

   for (i = 0; i < numBlocks; i++) {
      scanf(" %x", &temp[i]);
   }

   BuInit(&bu);
   i = 0;

   while (scanf(" %d", &size) != EOF) {
      if (!BuUnpack(&bu, size, &ret)) {
         if (i < numBlocks) {
            BuTakeData(&bu, temp[i++]);
            BuUnpack(&bu, size, &ret);
         }
         else {
            printf("Not enough data left.\n");
         }
      }
      printf("curData is %X (Should be unchanged until no bitsLeft)\n",
       bu.curData);
      printf("data: %08X\n", ret);
   }
   printf("\n");
   free(temp);

   return 0;
}
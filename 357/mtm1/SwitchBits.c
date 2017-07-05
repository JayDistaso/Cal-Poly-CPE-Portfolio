#include <stdio.h>

void zeroSet(unsigned int val) {
   val |= val + 1

 ;
   printf("Bottommost zero set: %X ", val);
}

void oneClear(unsigned int val) {
   val &= val - 1

 ;
   printf("Bottommost one cleared: %X\n", val);
}

int main() {
   unsigned int val;
   
   while (EOF != scanf("%X", &val)) {
      printf("Val: %X ", val);
      zeroSet(val);
      oneClear(val);
   }
   
   return 0;
}
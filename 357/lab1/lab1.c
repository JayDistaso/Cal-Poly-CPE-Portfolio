#define RETURN_VALUE 0
#define NUM 5

int main(void) {
   int i = 0;
   int *bad;

   while (i < NUM) {
      printf("%d ", i);
      i++;
   }

   printf("%p", bad);
   
   return RETURN_VALUE;
}

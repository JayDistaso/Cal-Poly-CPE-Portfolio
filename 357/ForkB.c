#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>


#define FORKS 30

int main() {
   int i, pid, status;
	double store;
   for (i=0;i<122;i++) {
	pid = (i/122) * 6.28;
      store = sin(pid); 
      printf("array[%d] = %d\n", i, (store * (3473/2)) + (3473/2));
   }
}

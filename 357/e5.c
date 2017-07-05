#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define FORKS 30

int main() {
   int i, pid;
   
   for (i = 0; i < FORKS; i++) {
      if (!(pid = fork())) {
         if (!(i % 10))
            sleep(2);
         printf("%d\n", i);
         printf("my pid is (up high) %d\n", pid);



      }
      else
 	printf("my pid is (down low)%d\n", pid);        



   }

   return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROCS 5

int main(int argc, char *argv[]) {
   int pid, proc, pidTotal = 0, reportFds[2];
   
   pipe(reportFds);

   /* Create PROCS children, call them child 0 through child PROCS-1 */
   for (proc = 0; proc < PROCS; proc++) {
      if (!(pid = fork())) {
         

pid = getpid();                          // Base: 4, Surcharge: 0
printf("my child pid is %d\n", pid);
write(reportFds[1], &pid, sizeof(&pid)); // Base: 10, Surcharge: 0
close(reportFds[1]);                     // Base: 5, Surcharge: 0
return (pid);                            // Base: 11, Surcharge: 0


      }
      else 
         pidTotal += pid;
   }

   for (proc = 0; proc < PROCS; proc++) {
      wait(NULL);
   }

   

close(reportFds[1]); // Base: 5, Surcharge: 0

printf("total pids = %d\n", pidTotal);

   for (proc = 0; proc < PROCS; proc++)
      if (0 < read(reportFds[0], &pid, sizeof(pid))) {
         printf("Good read\n");
	printf("I read %d\n", pid);
         pidTotal -= pid;
	printf("pidTotal = %d\n", pidTotal);
      }
	pid = read(reportFds[0], &pid, sizeof(pid));	
	printf("the return status is %d\n", pid);
   if (0 == pid)
      printf("After EOF on pipe, pidTotal is %d\n", pidTotal);
 
   return 0;
}

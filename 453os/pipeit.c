#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SORTPATH "/bin/sort" //Path to sort command
#define LSPATH "/bin/ls" //Path to ls command

int main(int argc, char **argv) {
   int lsPipe[2], childLs, childSort, sortStatus, lsStatus;
   
   if (pipe(lsPipe) != 0) { //This pipe is where ls outputts to, and sort reads from
      fprintf(stderr, "Pipe failed\n");
      return 1;
   }
   
   if ((childLs = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childLs == 0) { //child code will exec to ls
      close(lsPipe[0]);
      dup2(lsPipe[1], 1); //redirect stdout to lspipe, which goes to sort child
      execl(LSPATH, LSPATH, NULL);
      _exit(1); //If we get here the exec failed
   }
   
   close(lsPipe[1]);
   
   if ((childSort = fork()) < 0) {
      fprintf(stderr, "Bad fork call\n");
   }
   else if (childSort == 0) { //child code will exec to sort
      
      int fd = open("outfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      
      dup2(lsPipe[0], 0); //get input from ls pipe
      
      if (fd == -1)
      {
         printf("Error opening outfile!\n");
         _exit(1);
      }
      
      dup2(fd, 1); //send output to file
      execl(SORTPATH, SORTPATH, "-r", NULL);
      _exit(1); //If we get here the exec failed.
   }
   
   close(lsPipe[0]);
   
   //Check to make sure out children exited properly
   waitpid(childLs, &lsStatus, 0);
   
   if(!WIFEXITED(lsStatus)) {
      printf("ls did not exit properly!\n");
      printf("Exiting\n");
      exit(1);
   }
   
   waitpid(childSort, &sortStatus, 0);
   
   if(!WIFEXITED(sortStatus)) {
      printf("sort did not exit properly!\n");
      printf("Exiting\n");
      exit(1);
   }
   
   exit(0);

}
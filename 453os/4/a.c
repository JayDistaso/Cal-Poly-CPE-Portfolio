#include <sys/types.h>
#include <unistd.h>
#include <sys/ucred.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <minix/const.h>
#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <minix/ds.h>
#include <minix/syslib.h>

#define FILENAME "/dev/Secret"
#define msg "testyyyy"

int main(int argc, char *argv[]) {
   int fd = open(FILENAME, O_WRONLY);
   printf("Opening... fd=%d\n",fd);
   int res = write(fd,msg,strlen(msg));
   printf("Writing... res=%d\n",res);
   /* try grant */
   int uid;
   if ( argc > 1 && 0 != (uid=atoi(argv[1]))) {
      if ( (res = ioctl(fd,SSGRANT,&uid)) )
         perror("ioctl");
         printf("Trying to change owner to %d...res=%d\n",uid, res);
      }
   res=close(fd);
}
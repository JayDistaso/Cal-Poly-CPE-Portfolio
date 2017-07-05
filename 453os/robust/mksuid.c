#include "mksuid.h"

int main(int argc, char *argv[]) {
   /*Check if student is running the program by comparing the real
    UID of process with that of student.*/
    /*getuid returns real, geteuid returns effective*/
    uid_t uid=getuid()/*, euid=geteuid()*/;
    
    char password[LONG_ENOUGH_NUMBER];

    if (uid != STUDENT_UID) {
      printf("Incorrect udi. Exiting\n");
      exit(-1);
    }
    
    /*Prompt user for pass, validate against stored pass*/
    printf("Please enter your password\n");
    //Disable echo
    struct termios oflags, nflags;
    if (!tcgetattr(fileno(STDIN_FILENO), &oflags)) {
      perror("tcsetattr");
      return EXIT_FAILURE;
    }
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    
    if (tcsetattr(fileno(STDIN_FILENO), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        return EXIT_FAILURE;
    }

    printf("password: ");
    if (read(STDIN_FILENO, password, sizeof(password)) == -1) {
      perror("read");
      return EXIT_FAILURE;
    }

    if (password[strlen(password) - 1] == '\n') {
      password[strlen(password) - 1] = 0;
    }
       
    struct passwd *pass = getpwuid(uid);
    if (pass == NULL) {
      printf("Call to get password failed. Exiting\n");
      exit(-1);
    }   
    struct spwd *spass = getspnam(pass->pw_name);
    if (spass == NULL && errno == EACCES) {
      printf("No permission to read sp file. Exiting\n");
      exit(-1);
    }
    
    encrypted = crypt(password, spass->sp_pwdp);    

    if (encrypted == NULL) {
       printf("Encyption failed, Exiting\n");
       exit(-1);
    }
    
    if (strcmp(encrypted, passhash) != 0) {
      printf("Incorrect password\n");
      exit(-1);
    }
    
    
    /* restore terminal */
    if (tcsetattr(fileno(STDIN_FILENO), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        return EXIT_FAILURE;
    }
    
    /*Check thatg sniff is an ordinary file*/
    //?
    
    /*Check to see it is owned by student*/
    struct stat info;
    
    int fd = open(FILENAME, O_RDONLY);
    
    if (fd < 0) {
      perror("open");
      exit(-1);
    }
    
    if (fstat(fd, &info) == -1) {
      perror("fstat");
      exit(-1);
    }
    
    if (info.st_mtime + 60 < time(NULL)) {
      printf("Modification made over 1 minute ago. Exiting\n");
      exit(-1);
    }    
    
    if (STUDENT_UID != info.st_uid) {
      printf("File owner uid does not match student. Exiting\n");
      exit(-1);
    }
           
    fchown(fd, ROOT, PROJ);
    fchmod(fd, PROTECTION_MODE);
    
    return 0;
}
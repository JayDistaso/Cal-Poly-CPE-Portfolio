#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>
#include <pwd.h>
#include <termios.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <crypt.h>

#define STUDENT_UID 2640311//Define this
#define FILENAME "sniff"
#define ROOT 0
#define PROJ 95
#define LONG_ENOUGH_NUMBER 256
#define PROTECTION_MODE 04550
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>

#define EXIT_FAILURE -1
#define HANDLE_SIZE 255

typedef struct {
   int flag;
   char handle[HANDLE_SIZE];
} Handle
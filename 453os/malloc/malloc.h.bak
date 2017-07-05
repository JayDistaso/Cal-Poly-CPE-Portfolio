#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#define HEADER_SIZE sizeof(struct Header) 

/*Start of Data spaces will be divisible by this number*/
#define ALIGNED_NUMBER 16

/*Size memory is requested using sbrk()*/
#define DEFAULT_CHUNK 64000

#define DEBUG_VAR "DEBUG_MALLOC"
#define DEBUG_MESSAGE_BUFFER 100

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
int alignChunk();
int moveBreak();
struct Header *newChunk(struct Header *last, size_t size);
struct Header *find_Free_Chunk(struct Header **last, size_t size);

//structure to keep track of memory chunk metadata
typedef struct Header {
   size_t size;
   int isFree;
   struct Header *nextChunk;
   void *startOfData; /*pointer to memory actually intended for caller*/
   int offset; /*Amount of padding to keep address aligned*/
} Header;

/*This is the header of the first chunk of memory */
static void *rootOfLinkedList;

/* This points to the next piece of memory not yet put into chunks*/
static void *myBreak;

/*How much space we have left to give out*/
static long unchunkedSpace = 0;

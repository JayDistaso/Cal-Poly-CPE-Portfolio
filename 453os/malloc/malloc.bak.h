#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>


void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void alignChunk();
int moveBreak();
struct Header *newChunk(struct Header *last, size_t size);
struct Header *find_Free_Chunk(struct Header **last, size_t size);

//structure to keep track of memory chunk metadata
typedef struct Header {
   size_t size;
   int isFree;
   struct Header *nextChunk;
   void *startOfData; //pointer to actual allocated memory
   int offset; 
/*wasted space in between header and memory to keep memory alligned*/
} Header;

#define HEADER_SIZE sizeof(struct Header) 
#define ALIGNED_NUMBER 16
#define DEFAULT_CHUNK 64000
#define DEBUG_VAR "DEBUG_MALLOC"

static void *rootOfLinkedList;
static void *myBreak;
static int unchunkedSpace = 0;

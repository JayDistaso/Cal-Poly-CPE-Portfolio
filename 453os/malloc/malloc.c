#include "malloc.h"

/*Calloc functions similarly to malloc but zero's out data*/
void *calloc(size_t nmemb, size_t size) {
   const size_t totalSize = nmemb * size;
   char debugMessage[DEBUG_MESSAGE_BUFFER];
   void *ptr = NULL;
   
   if (totalSize <= 0) {
      return NULL;
   }
   
   /* Call malloc function to get memory chunk*/
   ptr = malloc(totalSize);
   
   /*0 out memory*/
   if (ptr) {
      memset(ptr, 0, totalSize);
   }
   
   /*Check if DEBUG_MALLOC is set and output if so*/
   if (getenv(DEBUG_VAR) != NULL) {
      snprintf(debugMessage, DEBUG_MESSAGE_BUFFER, "MALLOC: calloc(%zd,%zd) =>"
       " (ptr=%p, size=%zd)\n", nmemb, size, ptr, totalSize);
      fputs(debugMessage, stderr);
	}
   
   return ptr;
}

/*Malloc returns a pointer to memory that has been allocated from the heap*/
void *malloc(size_t size) {
   Header *chunk, *last;
   char debugMessage[DEBUG_MESSAGE_BUFFER];
   
   if (size <= 0) {
      return NULL;
   }
   
   /*Initial Call*/
   if (!rootOfLinkedList) {
      
      /*start out our break at alligned address*/
      if (!alignChunk()) {
         /*alignChunk failed*/
         return NULL;
      } 
      
      /*Cut off chunk of memory obtained from callig sbrk*/
      chunk = newChunk(NULL, size);
      
      if (!chunk) {
         /*newChunk failed*/
         return NULL;
      }
      
      rootOfLinkedList = chunk;
      
   }
   else {
      last = rootOfLinkedList;
      
      /*see if we have free chunk before carving out a new one*/
      chunk = find_Free_Chunk(&last, size); 
      
      /*If we do not have a suitable free chunk*/
      if (!chunk) {
         chunk = newChunk(last, size);
         if (!chunk) {
            /*If newChunk failed then we are likely out of memory*/
            return NULL; 
         }
      }
      else {
         chunk->isFree = 0;
      }
   }
   if (getenv(DEBUG_VAR) != NULL) {
      snprintf(debugMessage, DEBUG_MESSAGE_BUFFER, "MALLOC: malloc(%zd) =>" 
      "(ptr=%p, size=%zd)\n", size, chunk->startOfData, size);
		fputs(debugMessage, stderr);
	}
   
   /*Return a pointer to the start of data segment*/
   return chunk->startOfData;
}

/*Free takes a pointer and marks it as unused*/
void free(void *ptr) {
   char debugMessage[DEBUG_MESSAGE_BUFFER];
   Header *current = rootOfLinkedList;
   Header *last = NULL, *next = NULL;

   if (!ptr) {
      return;
   }
   
   /*walk through linked list searching for which chunk this ptr belongs to*/
   while (current) {
      if ((ptr >= current->startOfData) && 
      (ptr <= current->startOfData + current->size)) {
         current->isFree = 1;
         
         /*merge next chunk with current if free*/
         if ((current->nextChunk) && (current->nextChunk)->isFree) { 
            next = current->nextChunk;
            current->nextChunk = next->nextChunk;
            current->size += next->offset + next->size + HEADER_SIZE;
         }
         
         /*merge current to last chunk if free*/
         if (last && last->isFree) { 
            last->nextChunk = current->nextChunk;
            last->size += current->offset + current->size + HEADER_SIZE;
         }

         break;
      }
      else {
         /*Keep walking the list*/
         last = current;
         current = current->nextChunk;     
      }
   }
   
   /*Output debug message as needed*/
   if (getenv(DEBUG_VAR) != NULL) {
		snprintf(debugMessage, DEBUG_MESSAGE_BUFFER,
       "MALLOC: free(%p)\n", ptr);
		fputs(debugMessage, stderr);
   }
}

/*realloc takes a pointer to memory and a size to resize it to*/
void *realloc(void *ptr, size_t size) {
   char debugMessage[DEBUG_MESSAGE_BUFFER];
   Header *current = rootOfLinkedList, *next = NULL;
   Header *ptrToNewChunk = rootOfLinkedList;
   void *new_ptr = NULL;
   
   if (getenv(DEBUG_VAR) != NULL) {
      snprintf(debugMessage, DEBUG_MESSAGE_BUFFER, "MALLOC: realloc(%p,%zd)"
      " => (ptr=%p, size=%zd)\n", ptr, size, ptr, size);
      fputs(debugMessage, stderr);
	}
   
   /*realloc(NULL, size) ptr is like malloc(size) call*/
   if (!ptr) {
      return malloc(size);
   }
   /*realloc(ptr, 0) is like free(ptr) call*/
   if (size == 0) {
      free(ptr);
   }
   /*Walk through linked list searching for which chunk this ptr is in*/ 
   while (current) {
      if ((ptr >= current->startOfData) && 
      (ptr <= current->startOfData + current->size)) { 
         if (current->isFree) {
            /*The memory refrenced by ptr is free
             so it shouldnt be used with realloc*/
            current = NULL;
         }
         break;
      }
      current = current->nextChunk;
   }
   
   /*the ptr passed in was not in any previously 
    allocd chunk so it is not valid*/
   if (!current) { 
      return NULL;
   }
   
   /*If the realloc size is smaller than no action is needed*/
   if (current->size >= size) {
      return current->startOfData;
   }
   
   /*realloc is called and there is a free chunk adjecent in memory 
   so we try add it to current chunk*/
   if ((current->nextChunk) && (current->nextChunk)->isFree) {
      next = current->nextChunk;
      
      /* If it is not big enough than we will not merge the two chunks*/
      if (current->size + next->size + HEADER_SIZE + next->offset >= size) {
         current->nextChunk = next->nextChunk;
         current->size += next->offset + next->size + HEADER_SIZE;
         return current->startOfData;
      }
   }
   
   /*we are at the last chunk, we can in-place expand nto a new chink*/
   if (!current->nextChunk) {
      
      /*We don't want to use more memory than we need*/
      ptrToNewChunk = newChunk(current, size - current->size - HEADER_SIZE);
      
      current->nextChunk = ptrToNewChunk->nextChunk;
      current->size += ptrToNewChunk->offset 
       + ptrToNewChunk->size + HEADER_SIZE;
      return current->startOfData;
   }
   
   /*No way to in-place expand so we call malloc, 
    it will place in usused chunk or carve a new one*/
   new_ptr = malloc(size);
   
   /*malloc returns a ptr to the chunk of memory, not the header itself,
    so we walk through linked list*/
   while (ptrToNewChunk) {
      if ((new_ptr >= ptrToNewChunk->startOfData) && 
      (new_ptr <= ptrToNewChunk->startOfData + ptrToNewChunk->size)) { 
         break;
      }
      ptrToNewChunk = ptrToNewChunk->nextChunk;
   }
   
   /*If the pointer does not exsist n any chunk something went wrong*/
   if (!ptrToNewChunk) {
      return NULL;
   }
   
   /*We want to copy the min(size and current->size)*/
   if (size > current->size) {
      size = current->size; 
   } 
   
   /*memmove ensures we will not overwite our data while copying*/
   memmove(ptrToNewChunk->startOfData, current->startOfData, size);
   
   /*We should free old block since we put data in new block*/
   free(current->startOfData);
   
   return ptrToNewChunk->startOfData; 
   
}

/*Utility Function to align address space on first call, wastes 0-15 bytes*/
int alignChunk() {
   void *breakLocation = sbrk(0);
   char debugMessage[DEBUG_MESSAGE_BUFFER];
   
   /*We can't do math on ptrs without casting them first,
    this conditional checks if the break is at a aligned address*/
   if ((uintptr_t)breakLocation % ALIGNED_NUMBER != 0) {
      breakLocation = sbrk(ALIGNED_NUMBER - 
      ((uintptr_t)breakLocation % ALIGNED_NUMBER)); 
      
      /*We want to mae sure the sbrk call actualy worked*/
      if ((breakLocation == (void *) - 1)) {
         errno = ENOMEM;
         if (getenv(DEBUG_VAR) != NULL) {
		      snprintf(debugMessage, DEBUG_MESSAGE_BUFFER, 
            "Sbrk call failed, you are likely out of memory!\n");
		      fputs(debugMessage, stderr);
            return 0;
	      } 
      }
   }
   
   myBreak = sbrk(0);
   return 1;
}

/*This function moves the break by calling sbrk*/
int moveBreak() {
   void *sbrkCheck, *newBreakReturn;
   char debugMessage[DEBUG_MESSAGE_BUFFER];

   sbrkCheck = sbrk(0);
   newBreakReturn = sbrk(DEFAULT_CHUNK);
   
   /*Check that sbrk call worked*/
   if (newBreakReturn == (char *) - 1) {
      errno = ENOMEM;
      if (getenv(DEBUG_VAR) != NULL) {
		   snprintf(debugMessage, DEBUG_MESSAGE_BUFFER, 
         "Sbrk call failed, you are likely out of memory!\n");
         fputs(debugMessage, stderr);
      }
      return 0;   
   }
   
   /*Increment variable keeping track of unused memory*/
   unchunkedSpace += DEFAULT_CHUNK;
   
   /*Return 1 so other functions can make sure this worked*/
   return 1;
}

/*This function constructs the header and structure of
 chunks of memory we will hand out*/
struct Header *newChunk(struct Header *last, size_t size) {
   Header *chunk;
   char *ptrMath;
   int offset = 0;
   
   /*Sometimes we need to move the break once, or multiple times to
    make a chunk big enough*/
   while (size + HEADER_SIZE + ALIGNED_NUMBER > unchunkedSpace) {
      if (!moveBreak()) {
         /*moveBreak failed*/
         return NULL;
      }
   }
   
   /*This puts a header where our break is */
   chunk = ((Header *)myBreak);
   
   /*Now we move break forward*/
   myBreak += size + HEADER_SIZE;
   
   /*We want start of data block to be aligned*/
   if (((uintptr_t)myBreak - size) % ALIGNED_NUMBER != 0) {
      offset = (ALIGNED_NUMBER - 
      (((uintptr_t)myBreak - size) % ALIGNED_NUMBER));
      ptrMath = (char *) myBreak;
      ptrMath += offset;
      myBreak = (void *)ptrMath;
   }
   
   /*We attach the new chunk to the last, linked list style*/
   if (last) {
      last->nextChunk = chunk;
   }
   
   /*Decrement variable keeping track of unused space*/
   unchunkedSpace -= size + HEADER_SIZE + offset;
   
   /*initialize structure*/
   chunk->size = size;
   chunk->offset = offset;
   chunk->nextChunk = NULL;
   chunk->isFree = 0;
   chunk->startOfData = myBreak - size; 
   
   return chunk;
    
}

/*Utility function for walking list and searching for one that is free,
 also moves pointer to last chunk if it cant find a free chunk*/
struct Header *find_Free_Chunk(struct Header **last, size_t size) {
   Header *current = rootOfLinkedList;

   while (current && ((!current->isFree) || (current->size < size))) {
      *last = current;
      current = current->nextChunk;
   }
  
   return current;
}
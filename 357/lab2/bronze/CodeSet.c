#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "SmartAlloc.h"
#include "CodeSet.h"

#include <stdio.h>

typedef struct CodeEntry {
   struct CodeEntry *prev;
   unsigned char suffix;
   int first;
   int size;
   unsigned char *codeToBeFreed;
} CodeEntry;

typedef struct CodeSet {
   int capacity;
   int numCodes;
   CodeEntry *entries;
} CodeSet;


/* Allocate, initialize, and return a CodeSet object, via void *
 * The CodeSet will have room for |numCodes| codes, though it will
 * initially be empty. */ 

void *CreateCodeSet(int numCodes) {

   CodeSet *cs = calloc(1, sizeof(CodeSet));
   //printf("\nAmount of space callocd for codeset = %d\n", sizeof(CodeSet));
   cs->capacity = numCodes;
   cs->entries = (CodeEntry*)calloc(numCodes, sizeof(CodeEntry));
   //printf("\nAmount of space callocd for code entry = %d\n",numCodes * sizeof(CodeEntry));
   return cs;
}


/* Add a new 1-byte code to |codeSet|, returning its index, with
 * the first added code having index 0.  The new code's byte is
 * equal to |val|.  Assume (and assert if needed) that there
 * is room in the |codeSet| for a new code. */


int NewCode(void *codeSet, char val) {

   CodeSet *cs = codeSet;

   cs->entries[cs->numCodes].size = 1;
   cs->entries[cs->numCodes].first = 1;
   cs->entries[cs->numCodes].suffix = val;
   cs->entries[cs->numCodes].prev = NULL;

   return cs->numCodes++;
}


/* Create a new code by copying the existing code at index
 * |oldCode| and extending it by one zero-valued byte.  Any
 * existing code might be extended, not just the most recently
 * added one. Return the new code's index.  Assume |oldCode| 
 * is a valid index and that there is enough room for a new code. */


int ExtendCode(void *codeSet, int oldCode) {
   CodeSet *cs = codeSet;
   CodeEntry ce;
   ce.size = cs->entries[oldCode].size + 1;
   ce.first = 0;
   ce.suffix = 0;
   ce.prev = &cs->entries[oldCode];
   (cs->entries[cs->numCodes]) = ce;

   return cs->numCodes++;
}

/* Set the final byte of the code at index |code| to |suffix|.  
 * This is used to override the zero-byte added by ExtendCode. 
 * If the code in question has been returned by a GetCode call,
 * and not yet freed via FreeCode, then the changed final byte
 * will also show in the Code data that was returned from GetCode.*/

void SetSuffix(void *codeSet, int code, char suffix) {
   CodeSet *cs = codeSet;
   cs->entries[code].suffix = suffix;
}


/* Return the code at index |code| */


Code GetCode(void *codeSet, int code) {
   CodeSet *cs = codeSet;
   CodeEntry *ce = &(cs->entries[code]);
   //printf("Amount of space callocd in get code for codeset capcity %d = %d\n", sizeof(CodeSet), cs->capacity);
   Code ret;
   cs->entries[code].codeToBeFreed = ret.data = calloc(ce->size, sizeof(unsigned char));
   int i = ret.size = ce->size;
   for (i -= 1; i >= 0; i--) {
      ret.data[i] = ce->suffix;
      ce = ce->prev;
   }
   printf("\nI put code in entries[%d]\n", code);
   return ret;
}


/* Mark the code at index |code| as no longer needed, until a new
 * GetCode call is made for that code. */

void FreeCode(void *codeSet, int code) {
   CodeSet *cs = codeSet;
   free(cs->entries[code].codeToBeFreed);
   printf("Freed space for codeset with capacity %d after a getcode for index %d\n", cs->capacity, code);
}

/* Free all dynamic storage associated with |codeSet| */


void DestroyCodeSet(void *codeSet) {
   CodeSet *cs = codeSet;
   //printf("\nAmount of space freed for codeset = %d\n", sizeof(CodeSet));
   //printf("\nAmount of space freed for code entry = %d\n",cs->capacity * sizeof(CodeEntry));
   free(cs->entries);
   free(cs);
}
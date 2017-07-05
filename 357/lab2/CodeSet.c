#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "SmartAlloc.h"
#include "CodeSet.h"

#include <stdio.h>

/* One entry in a dictionary of codes.
* |data| points to the sequence of bytes the code represents.
* |size| is the sequence length.

typedef struct Code {
   unsigned char *data;
   unsigned short size;
} Code;

*/

typedef struct CodeEntry {
   unsigned char *code;
   int size;
   int index;
} CodeEntry;

typedef struct CodeSet {
   int numCodes;
   int size;
   CodeEntry *entries;
} CodeSet;

/* Allocate, initialize, and return a CodeSet object, via void *
* The CodeSet will have room for |numCodes| codes, though it will
* initially be empty. */

void *CreateCodeSet(int numCodes) {

   CodeSet *cs = calloc(1, sizeof(CodeSet)); //how big to make this?
   cs->size = numCodes;
   cs->entries = (CodeEntry*)calloc(numCodes, sizeof(CodeEntry));

   return cs;
}


/* Add a new 1-byte code to |codeSet|, returning its index, with
* the first added code having index 0.  The new code's byte is
* equal to |val|.  Assume (and assert if needed) that there
* is room in the |codeSet| for a new code. */

int NewCode(void *codeSet, char val) {

   CodeSet *cs = codeSet;
   unsigned char *code = (unsigned char *)calloc(1, sizeof(unsigned char));
   //
   cs->entries[cs->numCodes].size = 1;
   cs->entries[cs->numCodes].index = cs->numCodes;
   code[0] = val;  
   //
   cs->entries[cs->numCodes].code = code;

   return cs->numCodes++;
}

/* Create a new code by copying the existing code at index
* |oldCode| and extending it by one zero-valued byte.  Any
* existing code might be extended, not just the most recently
* added one. Return the new code's index.  Assume |oldCode|
* is a valid index and that there is enough room for a new code. */

int ExtendCode(void *codeSet, int oldCode) {
   CodeSet *cs = codeSet;
   CodeEntry *ce = (CodeEntry*)calloc(1, sizeof(CodeEntry));
   ce->size = cs->entries[oldCode].size + 1;
   unsigned char *code;
   ce->code = (unsigned char *)calloc(ce->size, sizeof(unsigned char));
   int i = 0;
   for (; i < ce->size - 1; i++) {
      ce->code[i] = cs->entries[oldCode].code[i];
   }

   ce->index = cs->numCodes;
   printf("giving callocd block to cs->entries[%d]\n", cs->numCodes);
   (cs->entries[cs->numCodes]) = *ce;
   free(ce);
   return cs->numCodes++;
}

/* Set the final byte of the code at index |code| to |suffix|.
* This is used to override the zero-byte added by ExtendCode.
* If the code in question has been returned by a GetCode call,
* and not yet freed via FreeCode, then the changed final byte
* will also show in the Code data that was returned from GetCode.*/

void SetSuffix(void *codeSet, int code, char suffix) {
   CodeSet *cs = codeSet;
   CodeEntry *ce = &(cs->entries[code]);
   ce->code[ce->size - 1] = suffix;
}

/* Return the code at index |code| */

Code GetCode(void *codeSet, int code) {
   Code ret;
   CodeSet *cs = codeSet;
   ret.size = cs->entries[code].size;
   ret.data = cs->entries[code].code;
   return ret;
}

/* NOT USED FOR THIS IMPLEMENTATION
* Mark the code at index |code| as no longer needed, until a new
* GetCode call is made for that code.
void FreeCode(void *codeSet, int code);
*/

/* Free all dynamic storage associated with |codeSet| */

void DestroyCodeSet(void *codeSet) {
   int i = 0;
   CodeSet *cs = codeSet;
   
   for (; i < cs->numCodes; i++) {
      unsigned char *code = cs->entries[i].code;
      free(code);
   }
   free(cs->entries);
   free(cs);
}




//End of CodeSet.c
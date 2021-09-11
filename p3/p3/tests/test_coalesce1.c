// check for coalesce free space
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void * ptr[5];

   ptr[0] = myAlloc(800);
   assert(ptr[0] != NULL);

   ptr[1] = myAlloc(800);
   assert(ptr[1] != NULL);

   ptr[2] = myAlloc(800);
   assert(ptr[2] != NULL);

   ptr[3] = myAlloc(800);
   assert(ptr[3] != NULL);

   ptr[4] = myAlloc(800);
   dispMem();

   myFree(ptr[4]);
   dispMem();

   ptr[4] = myAlloc(800);
   dispMem();

   myFree(ptr[3]);
   myFree(ptr[4]);
   dispMem();

   ptr[3] = myAlloc(800);
   dispMem();
   ptr[4] = myAlloc(800);
	dispMem();
   myFree(ptr[3]);
   dispMem();
   myFree(ptr[4]);

   dispMem();
   ptr[3] = myAlloc(4);

   dispMem();
   assert(myFree(ptr[1]) == 0);
   assert(myFree(ptr[2]) == 0);
   dispMem();
   ptr[2] = myAlloc(4);
   assert(ptr[2] != NULL);
   dispMem();
   exit(0);
}

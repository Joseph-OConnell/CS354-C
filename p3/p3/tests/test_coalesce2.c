// check for coalesce free space
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
   assert(myInit(4096) == 0);
   void * ptr[4];

   dispMem();
   ptr[0] = myAlloc(800);
   assert(ptr[0] != NULL);
   dispMem();

   ptr[1] = myAlloc(800);
   assert(ptr[1] != NULL);
dispMem();

   ptr[2] = myAlloc(800);
   assert(ptr[2] != NULL);
dispMem();

   ptr[3] = myAlloc(800);
   assert(ptr[3] != NULL);

   while (myAlloc(800) != NULL)
     ;
dispMem();

   assert(myFree(ptr[2]) == 0);
dispMem();
   assert(myFree(ptr[1]) == 0);
dispMem();
   ptr[2] = myAlloc(1600);
   assert(ptr[2] != NULL);
dispMem();
   exit(0);
}

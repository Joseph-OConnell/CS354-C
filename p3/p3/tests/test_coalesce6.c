// check for coalesce free space (last chunk)
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

#define HEADER (4)
#define SLACK (8)

int main() {
   assert(myInit(4096) == 0);
   void * ptr[5];

   ptr[0] = myAlloc(884);
   assert(ptr[0] != NULL);

   ptr[1] = myAlloc(884);
   assert(ptr[1] != NULL);

   ptr[2] = myAlloc(884);
   assert(ptr[2] != NULL);

   ptr[3] = myAlloc(884);
   assert(ptr[3] != NULL);
dispMem();
   assert(myAlloc(884) == NULL);
  dispMem(); 
   // last free chunk is at least this big
   int free = (4096 - (884 + HEADER) * 4) - SLACK;
dispMem();
   assert(myFree(ptr[3]) == 0);
dispMem();
   free += (884 + HEADER);
   ptr[3] = myAlloc(free - HEADER);
dispMem();
   assert(ptr[3] != NULL);

   myFree(ptr[0]);
   dispMem();
   myFree(ptr[1]);
   dispMem();
   myFree(ptr[2]);
   dispMem();
   myFree(ptr[3]);
dispMem();
   ptr[0] = myAlloc(20);

   dispMem();

    printf("%s\n%", "testing bad input");
   if( myAlloc(-1) != NULL){
        printf("%s\n%", "-1 breaks alloc");
	}

   if (myAlloc(444444444) != NULL){
	    printf("%s\n%", "big alloc breaks");
   }

    printf("%s\n%", "testing frees");

    if(myFree(ptr[1]) != -1){
	     printf("%s\n%", "trying to free something already free fail");
    }

    ptr[3] = NULL;

    if (myFree(ptr[3]) !=  -1){
	     printf("%s\n%", "null pointer free fail");
    }

    if (myFree(ptr[0] + 1) != -1){
	     printf("%s\n%", "not divisible by 8 fails");
    }

    if (myFree(ptr[0] + 8000) != -1){
	     printf("%s\n%", "outside range fail");
    }


   exit(0);
}

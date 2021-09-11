// write to a block of allocated heap and check the value
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
	 printf("%s\n%", "initial mem alloc");
   assert(myInit(4096) == 0);
    dispMem();
   int* ptr = (int*) myAlloc(sizeof(int));
   dispMem();
   assert(ptr != NULL);
   *ptr = 42;   // check pointer is in a writeable page
   assert(*ptr == 42);
   exit(0);
}

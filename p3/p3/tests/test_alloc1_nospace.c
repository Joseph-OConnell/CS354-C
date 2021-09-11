// allocation is too big to fit
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
    printf("%s\n%", "initial allocation");
    assert(myInit(4096)  == 0);
    dispMem();
     printf("%s\n%", "single 8 bit alloc");
    assert(myAlloc(1)    != NULL);
    dispMem(); 
    printf("%s\n%", "too large of allocation");
    assert(myAlloc(4095) == NULL);
    dispMem();
    exit(0);
}

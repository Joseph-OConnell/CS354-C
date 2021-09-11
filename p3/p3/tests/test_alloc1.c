// a simple 8 byte allocation
#include <assert.h>
#include <stdlib.h>
#include "myHeap.h"

int main() {
    assert(myInit(4096) == 0);
    printf("%s\n%", "initial mem allocation");
    dispMem();
    void* ptr = myAlloc(4);
    printf("%s\n%", "single 8 bit alloc");
    dispMem();
    assert(ptr != NULL);
    exit(0);
}

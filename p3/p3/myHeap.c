////////////////////////////////////////////////////////////////////////////////
// Main File:        myHeap.c
// This File:        myHeap.c
// Other Files:      
// Semester:         CS 354 Fall 2020
// Instructor:       deppeler
// 
// Discussion Group: 613
// Author:           Joseph O'Connell 
// Email:            jpoconnell2@wisc.edu
// CS Login:         jo-connell
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */

/*
 * Reference to the last header that was allocated.
 * Needed for the next fit placement
 */
blockHeader *currentHeader = NULL;

/*
 * Size of a page for this memory allocator.
 * This constant is mainly used to clean up code and to find the remainder
 * of headers or round down headers size status
 */
const int PAGE_SIZE = 8;

/*
 * The size of a word for this memory allocator.  Used to clean up code.
 * Also represents the size of the header in bytes.
 */
const int WORD_SIZE = 4;

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* myAlloc(int size) {
    // return NULL if not positive or if larger than heap space
    
    // if size is not positive
    if (size < 1){
        return NULL;
    }

    // if larger than heap space
    if((size + WORD_SIZE) > allocsize){
        return NULL;
    }

    // first calculate size of allocation with header and padding (if necessary)
    // blockSize is the total size of the block in bytes
    int blockSize = size + WORD_SIZE;

    // padding represents the number of padding in bytes 
    int padding = PAGE_SIZE - (blockSize % PAGE_SIZE);

    // if padding is equal to 8, don't add 8 bytes of padding
    if(padding != PAGE_SIZE){
        blockSize = blockSize + padding;
    }
    
    //have true size of allocation at this point
    
    //Use next fit placement policy
    //if this is the first allocation, currentHeader is null
    if(currentHeader == NULL){
        currentHeader = heapStart;
    }

    //keep track of starting point to provide exit point for next-fit algo
    blockHeader *start = currentHeader;

    //set this variable equal to 1 if a good allocation has been found
    int validAlloc = 0;
    
    // check the current header for free and large enough alloc
    if((currentHeader->size_status % PAGE_SIZE == 0 ||
		    currentHeader->size_status % PAGE_SIZE == 2) &&
		    ((currentHeader->size_status/PAGE_SIZE)*PAGE_SIZE) >= blockSize){
	    
        // if we reach this point, this is a suitable header address
	validAlloc = 1;
    }
    
    //else iterate up to next blockheader address for loop
    else{
	currentHeader += ((currentHeader->size_status/PAGE_SIZE)*2);
    }

    //if address is now at the end mark, move pointer to start
    if(currentHeader->size_status == 1){
	currentHeader = heapStart;
    }

    //enter while loop that breaks if the original address (start) is reached
    //If the starting point is valid, loop breaks immediately
    //If it was not valid, loop will iterate until it find a spot
    //If it does not, validAlloc stays at 0
    while(currentHeader != start){
	   
    	// check the current header for free and large enough alloc
    	if((currentHeader->size_status % PAGE_SIZE == 0 ||
		    currentHeader->size_status % PAGE_SIZE == 2) &&
		    ((currentHeader->size_status/PAGE_SIZE)*PAGE_SIZE) >= blockSize){
    	    validAlloc = 1;
	    break;
        }
    
    	//else iterate up to next block Header address for loop
    	else{
	    currentHeader += ((currentHeader->size_status/PAGE_SIZE)*2);
    	}

    	//if at end mark, move pointer to heap start
	if(currentHeader->size_status == 1){
	    currentHeader = heapStart;
	}	
    }

    //if we reached this point, either at start, or good alloc location, or both
    // If validAlloc is 0, no good locations so return NULL
    if(validAlloc == 0){
        return NULL;
    }

    //if we reach this point, good alloc location
    
    // Attempt to split the memory allocation into
    // checking for < guarentees at least 8 byte difference
    if(blockSize < ((currentHeader->size_status/PAGE_SIZE)*PAGE_SIZE)){
    	// blockSize is at least 8 bytes less than the current header
	
	//create necessary headers/footers
	//first block header is already defined
	//don't need first block footer as it will be allocated
	
	//create reference for second block Header
	blockHeader *secondHeader = currentHeader + (blockSize/(sizeof(blockHeader)));
	
	//create reference for second block footer
	blockHeader *secondFooter = currentHeader + ((currentHeader->size_status/PAGE_SIZE)*2) - 1;
	
	//input both their size status values accordingly
	secondHeader->size_status = ((currentHeader->size_status/PAGE_SIZE)*PAGE_SIZE) - blockSize + 2;
	secondFooter->size_status = ((currentHeader->size_status/PAGE_SIZE)*PAGE_SIZE) - blockSize;

	//change first header size but keep p and a bits
	currentHeader->size_status = currentHeader->size_status % PAGE_SIZE + blockSize;	
    }

    //either split allocation or not large enough to split at this point
    
    // change header to show it is now allocated and return ptr
    currentHeader->size_status += 1;
    return (currentHeader + 1);
}
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int myFree(void *ptr) {
	// return -1 if ptr is NULL
	if(ptr == NULL){
	    return -1;
	}

	//if ptr is not multiple of 8 return -1
	if(((unsigned int) ptr) % PAGE_SIZE != 0){
	    return -1;
	}

	//if ptr is outside of heap space return -1
	if(((unsigned int) ptr) < ((unsigned int) heapStart) ||(unsigned int) ptr > (unsigned int)(heapStart + (allocsize/(sizeof(blockHeader))))){
	    return -1;
	}

	//main header is a reference to the blockHeader of the ptr block
	blockHeader *mainHeader = ptr - WORD_SIZE;
	
	// return -1 if already freed
	if( mainHeader->size_status % PAGE_SIZE == 0 ||
			mainHeader->size_status % PAGE_SIZE == 2){
	    return -1;
	}
	
	// no errors if we reach this point
	
	// first free the block we started on
	
	// update the header to show it is no longer allocated
	mainHeader->size_status -= 1;

	// create a block footer for the main block and assign it an according value
	blockHeader *mainFooter = ptr + ((((mainHeader->size_status)/PAGE_SIZE)*PAGE_SIZE) - 2*WORD_SIZE);
	mainFooter->size_status = (((mainHeader->size_status)/PAGE_SIZE)*PAGE_SIZE);

	// Create a blockHeader reference to the block ahead of the starting block
	blockHeader *nextBlock = ptr + (((mainHeader->size_status)/PAGE_SIZE)*PAGE_SIZE) - WORD_SIZE;
	
	//if at end mark, don't change, else change next header to say previous is free
	if (nextBlock->size_status != 1){
	    nextBlock->size_status -= 2;
	}

	// first try to coalesce with previous block
	if(mainHeader->size_status % PAGE_SIZE == 0){
	    // previous block is not allocated, coalesce with previous block
	    
	    // create reference for previous block header and footer
	    blockHeader *prevFooter = ptr - (2*WORD_SIZE);
	    blockHeader *prevHeader = ptr - (((prevFooter->size_status)) + WORD_SIZE);
	
	    //change previous block header to new size keep p and a bit same
	    prevHeader->size_status += (((mainHeader->size_status)/PAGE_SIZE)*PAGE_SIZE);

	    //change main footer to new size
	    mainFooter->size_status = (((prevHeader->size_status)/PAGE_SIZE)*PAGE_SIZE);

	    //change ptr reference so method will work if 1 or both adjacent blocks are free
	    ptr = prevHeader + 1;

	    //change currentHeader reference so myalloc works properly 
	    // this only runs if the block being freed was the last allocated
	    if(currentHeader == mainHeader){
	        currentHeader = prevHeader;
	    }
	}

	// try to coalesce with block following main block
	if(nextBlock->size_status % PAGE_SIZE == 0){
	    //already have reference to nextBlock so don't need to redefine
	    
	    // redefine first block block header because it could have changed
	    blockHeader *beforeHeader = ptr - WORD_SIZE;

	    // change the first block headers value accordingly
	    beforeHeader->size_status += (((nextBlock->size_status)/PAGE_SIZE)*PAGE_SIZE);

	    // Create a new footer reference for the coalesced block 
	    // Assign a value accordingly
	    blockHeader *newFooter = ptr + (((beforeHeader->size_status)/PAGE_SIZE)*PAGE_SIZE) - 2*WORD_SIZE;
	    newFooter->size_status = (((beforeHeader->size_status)/PAGE_SIZE)*PAGE_SIZE);

	    //change currentHeader reference if it is on nextBlocks header
	    if(currentHeader == nextBlock){
	        currentHeader = beforeHeader;
	    }
	}
	return 0;
} 
 
/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int myInit(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dispMem() {     
 
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;
    void *footer_loc = NULL;

    blockHeader *current = heapStart;
    blockHeader *footer = NULL;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int footer_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\t\tFooter\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
	footer = NULL;

        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;

	// footer stuff I added here
	if (is_used){
	    footer_size = 0;
	}
	else{
	    footer_loc = t_end - 3;
	    footer = footer_loc;
	    footer_size = footer->size_status;
	}

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\t\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size, footer_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
} 


// end of myHeap.c (fall 2020)


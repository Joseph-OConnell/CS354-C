///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      example board files for testing
// Semester:         CS 354 Fall 2020
//
// Author:           Joseph O'Connell
// Email:            jpoconnell2@wisc.edu
// CS Login:         jo-connell
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;      // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    // I realized afterwards this isn't supposed to loop so I just added exit functions
	
    int userInput = 0;			// initialized value for userInput so loop functions
    while(userInput < 3 || userInput % 2 == 0){
	 printf("%s\n", "Enter magic square's size (odd integer >=3)");
	 if(fscanf(stdin, "%i", &userInput) == 1){
	 }
	 else{
	     printf("%s\n", "Error reading input");
	     exit(1);
	 }

	 //if even, print needs to be odd.  
	 if(userInput % 2 == 0){
	    printf("%s\n", "Size must be odd.");
	    exit(1);
	 }

	 //if less than 3, print needs to be greater than 3
	 if(userInput < 3){
	    printf("%s\n", "Size must be >= 3.");
   	    exit(1);	    
	 }	 
    }
    return userInput;   
} 
   
/* Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
    // create a stack pointer to a MagicSquare instance on the heap     
    MagicSquare *mySquare;
    mySquare = malloc(sizeof(MagicSquare));
    if(mySquare == NULL){
	printf("memory did not allocate properly.\n");
	exit(1);
    }
    // Set size within struct
    (*mySquare).size = n;

    // Define dynamically allocated 2D array
    int **board;				//the magicSquare represented as a 2D array
    board = malloc(sizeof(int *) * n);
    if(board == NULL){
   	printf("memory did not allocate properly.\n");
	exit(1);
    }
    for(int i = 0; i < n; i++){
	*(board + i) = malloc(sizeof(int) * n);
	if(*(board + i) == NULL){
	   printf("memory did not allocate properly.\n");
	   exit(1);
	}
    }

    // Have a dynamically allocated array at this point, build the magic square
    // Setting every value equal to -1 just to initialize
    for(int i = 0; i < n; i++){
	for(int j = 0; j < n; j++){
	    *(*(board + i) + j) = -1;
	}
    }
   
    // now create the magic square    
    int col = (n - 1)/2;	// holds the index for the current column
    int row = 0;		// holds the index for the current row
    
    int colCheck;		// holds the index for potentially the next column
    int rowCheck;		// holds the index for potentially the next row
    // do the first square on my own
    *(*(board + row) + col) = 1;
    //loop through and create the rest;
    for(int i = 2; i <= (n*n); i++){
	colCheck = (col + 1) % n;
	rowCheck = (row + n - 1) % n;

	// if there is not a number yet, use new location as index
	if(*(*(board + rowCheck) + colCheck) == -1){
	   row = rowCheck;
	   col = colCheck;
	}

	//move down 1 row otherwise
	else{
	   row = (row + 1) % n;
	   //col stays the same
	}
	*(*(board + row) + col) = i;
    }

    // assign 2d array to struct
    (*mySquare).magic_square = board;
    return mySquare;    
} 

/* Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
     FILE *fp = fopen(filename, "w");
	if(fp == NULL) {
	   printf("Can't open file for writing.\n");
	   exit(1);
	}
     // Print the Magic Square to the the file
     int size = (*magic_square).size;		// holds the n size from the struct
     
     // print size on first line of file
     fprintf(fp,"%i\n", size);

     //print the 2d array to the file
     for(int i = 0; i < size; i++){
	for(int j = 0; j < size; j++){
	   fprintf(fp, "%i", *(*((*magic_square).magic_square + i) + j));
	   if(j != (size - 1)){
	      fprintf(fp, "%s", ",");
	   }
	}
	fprintf(fp,"%s\n", "");
     }
     // close the file
     if(fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
     } 
}

/* Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {
    // Check input arguments to get output filename
    
    // Check number of input args for proper usage
    if(argc != 2){
       printf("%s\n", "Usage: ./myMagicSquare <output_filename>");
       exit(1);
    }

    //Assume good input for the rest
    //output file name is *(argv + 1)
    // Get magin square's size from user

    int size = getSize();				// the n size of the square
    // Generate the magic square
    MagicSquare *mySquare = generateMagicSquare(size);	// pointer to the magic square struct

    // Output the magic square
    fileOutputMagicSquare(mySquare, *(argv + 1));	
    
    // free up memory
    // free up internal areas of magic square
    for(int i = 0; i < size; i++){
        free(*((*mySquare).magic_square + i));
	*((*mySquare).magic_square + i) = NULL;
    }
    
    // free magic square
    free((*mySquare).magic_square);
    (*mySquare).magic_square = NULL;
    
    // free the struct
    free(mySquare);
    mySquare = NULL;
    return 0;
} 


                                                         
//				myMagicSquare.c      


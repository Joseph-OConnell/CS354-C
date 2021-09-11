////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        division.c
// Other Files:      sendsig.c, mySigHandler.c
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



#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// global variable to keep track of successful divisions
int divisions = 0;

/* 
 * handler for a divide by zero signal.  Prints error message along
 * with total number of successful divisions and exits the program. *
 *
 */
void handler_SIGFPE(){
	//print out error, number of divisions, and exit
	printf("Error: a division by 0 operation was attempted.\n");
	printf("Total number of operations completed successfully: %i\n", divisions);
	printf("The program will be terminated.\n");
	exit(0);
}

/* 
 * handler for Ctrl + C signal.  prints the number of successful
 * divisions and then exits the program.  
 * 
 */
void handler_SIGINT(){
        // print messages and exit
        printf("\nTotal number of operations completed successfully: %i\n", divisions);
        printf("The program will be terminated.\n");
        exit(0);
}

int main() {
	// initialize sigaction struct for divide by zero
	struct sigaction sz;
	memset(&sz, 0, sizeof(sz));
	sz.sa_handler = &handler_SIGFPE;
	// call sigaction and check return value
	if(sigaction(SIGFPE, &sz, NULL) != 0){
		printf("Error binding SIGFPE handler\n");
		exit(1);
	}
	// initialize sigaction struct for Ctrl+C
	struct sigaction si;
        memset(&si, 0, sizeof(si));
        si.sa_handler = &handler_SIGINT;
	// call sigaction and check return value
        if(sigaction(SIGINT, &si, NULL) != 0){
                printf("Error binding SIGINT handler\n");
                exit(1);
        }
	// set input char array to 101 so space for null terminator
	char input1[101];
	char input2[101];

	// initialize vars prior to loop
	int numerator = 0;
	int denominator = 0;
	int val = 0;
	int rem = 0;

	while (1){
		// print prompts and put input into variables
		// check return values for each fgets call
		printf("Enter first integer: ");
		if (fgets(input1,100,stdin) == NULL){
			printf("Could not read numerator input");
			exit(1);
		} 
		printf("Enter second integer: ");
                if (fgets(input2,100,stdin) == NULL){
			printf("Could not read denominator input");
			exit(1);
		}
		// cast both inputs to ints
		// if it fails, value will be 0
		numerator = atoi(input1);
		denominator = atoi(input2);

		// perform calculations
		val = numerator/denominator;
		rem = numerator % denominator;

		// if code reaches here there is no divide by zero error
		// output result and increment
		printf("%i / %i is %i with a remainder of %i\n", numerator, denominator, val, rem);
		divisions++;
	}
}













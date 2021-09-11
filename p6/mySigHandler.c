////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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



#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// global vars
// alarm time in seconds
int timer = 3;
// number of SIGUSR1 signals
int interrupt = 0;


/*
 * Handler function for alarm signals.  Prints the PID and current date/time each call.
 * Calls alarm at the end causing an infinite loop
 *
 */
void handler_SIGALRM(){
	// print out PID & time
	int pid = getpid();
	time_t current;
	time(&current);
	// check return value of time()
	if(current == -1){
		printf("error calling time()");
		exit(1);
	}

	// call ctime and check return value
	char* currentTime = ctime(&current);
	if (currentTime == NULL){
		printf("error calling ctime()");
		exit(1);
	}
	// print out statement and call alarm again to repeat
	printf("PID: %d CURRENT TIME: %s", pid, currentTime);
	alarm(timer);
}


/* 
 * handler for kill -10/SIGUSER1
 * increments the number of interrupts and prints a message
 */
void handler_SIGUSR1(){
        // increment interrupt and print message
	interrupt++;
        printf("SIGUSR1 handled and counted!\n");
}


/* 
 * handler for Ctrl+C signals
 * prints a message with the number of interrupts and exits
 * 
 */
void handler_SIGINT(){
        // print messages and exit
        printf("\nSIGINT handled.\n");
	printf("SIGUSR1 was handled %i times. Exiting now.\n", interrupt);
	exit(0);
}



int main() {
	// print opening message
	printf("Pid and time print every 3 seconds.\nEnter Ctrl-C to end the program.\n");
	
	// assign alarm struct
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler_SIGALRM;
	// call sigaction and check return value
	if(sigaction(SIGALRM, &sa, NULL) != 0){
		printf("Error binding SIGALRM handler\n");
		exit(1);
	}

	// assign SIGUSER1 Struct (kill -10)
	struct sigaction su1;
        memset(&su1, 0, sizeof(su1));
        su1.sa_handler = &handler_SIGUSR1;
	// call sigaction and check return value
        if(sigaction(SIGUSR1, &su1, NULL) != 0){
                printf("Error binding SIGUSR1 handler\n");
                exit(1);
        }

	// assign SIGINT Struct (Ctrl + C)
	struct sigaction si;
        memset(&si, 0, sizeof(si));
        si.sa_handler = &handler_SIGINT;
	// call sigaction and check return value
        if(sigaction(SIGINT, &si, NULL) != 0){
                printf("Error binding SIGINT handler\n");
                exit(1);
        }

	// call alarm for 3 seconds then enter infinite loop
	alarm(timer);
	while(1){
	}
}


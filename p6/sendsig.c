////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c, division.c
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]) {

    // check for right number of args
    if (argc != 3){
	printf("Usage: <signal type> <pid>\n");
	exit(1);
    }

    // check for -u or -i as second argument
    int u = strcmp("-u",argv[1]);
    int i = strcmp("-i", argv[1]);
    // if right flag is not entered, exit the program
    if (u != 0 && i != 0){
        exit(1);
    }

    // convert last arg to int
    int pid = atoi(argv[2]);
    // exit program if non-int is entered as arg
    if (pid == 0){
	exit(1);
    }

    // ret holds the return value of kill
    int ret = 0;

    // send respective signal based on arg
    if (u == 0){
	ret = kill(pid, SIGUSR1);
    }
    if (i == 0){
	ret = kill(pid, SIGINT);
    }

    // check return value of kill
    if (ret == -1){
	printf("error sending signal");
	exit(1);
    }
}



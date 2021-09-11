///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:      
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
     
char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, DELIM);
    *size = atoi(token);
}       
 
   
  
/* TODO:
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {
	// if size is even return 0
	if(size % 2 == 0){
		return 0;
	}
	// same number of Xs and Os or one more X than O
	int numXs = 0;				//counter for Xs in board
	int numOs = 0;				//counter for Os in board
	//loop through array
	for(int i = 0; i < size; i++){
	   for(int j = 0; j < size; j++){
	      if(*(*(board + i) + j) == 1){
	         numXs++;
	      }
	      if(*(*(board + i) + j) == 2){
	 	 numOs++;
	      }
	   }
	}
	if(numXs != numOs && numXs != (numOs + 1)){
	   return 0;
	}

	//either no winner or one winner; X and O both can't be winners
	// check horizontal wins
	int rowWinCount = 0;			//keeps track of the number of wins
	int streak;				//keeps track of how many X or O in a row/column
	int detect;				//keeps track of X or O in the row/column
	int rowWinner = 0;			//keeps track of the winner
	for(int i = 0; i < size; i++){
	   streak = 0;
	   detect = 3;
	   for(int j = 0; j < size; j++){
	      // if it is a blank, next row
	      if(*(*(board + i) + j) == 0){
		 break;
	      }
	      //if first int in row, store
	      if( j == 0){
	         detect = *(*(board + i) + j);
	      }
	      //if doesn't match first, next row
	      if( detect != *(*(board + i) + j)){
		 break;
	      }
	      //part of a streak if it reaches here
	      streak++;
	      if( streak == size){
		 rowWinner = *(*(board + i) + j);
		 rowWinCount++;
	      }
	   }
	}
	//if there is more than 1 horizontal win, invalid
	if(rowWinCount > 1){
	   return 0;
	}

	//rowWinner could be 0, 1, or 2 here
	
	//check vertical wins
	int colWinCount = 0;			//keeps track of the number of wins
	int colWinner= 0;			//keeps track of the winner
	//reuse streak and detect from before
	for(int j = 0; j < size; j++){
	   streak = 0;
	   detect = 3;
	   for(int i = 0; i < size; i++){
	      // if it is a blank, next col
	      if(*(*(board + i) + j) == 0){
		 break;
	      }
	      //if first int in col, store
	      if( i == 0){
	         detect = *(*(board + i) + j);
	      }
	      //if doesn't match first, next col
	      if( detect != *(*(board + i) + j)){
		 break;
	      }
	      //part of a streak if it reaches here
	      streak++;
	      if( streak == size){
		 colWinner = *(*(board + i) + j);
		 colWinCount++;
	      }
	   }
	}
	//if there is more than 1 horizontal win, invalid
	if(colWinCount > 1){
	   return 0;
	}

	//colWinner could be 0, 1, or 2 here
	
	//test for diagonal wins
	//backslash first
	int backslashWinCount = 0;		//keeps track of the number of wins
	int backslashWinner = 0;		//keeps track of the winner
	int start = 0;				//starting index for start side
	int end = size - 1;			//starting index for end side
	for(int i = 0; i < (size + 1)/2; i++){
	   //if start is a blank, break
	   if(*(*(board + start) + start) == 0){
	      break;
	   }
	   //if end is blank, break
	   if(*(*(board + end) + end) == 0){
	      break;
	   }
	   //if end and start are not the same, break
	   if(*(*(board + start) + start) != *(*(board + end) + end)){
	   break;
	   }
	   // if we reach this point, the streak exists
	   // save the part of the streak
	   if(start == 0){
	      detect = *(*(board + end) + end);
	   }
	   if(*(*(board + end) + end) != detect){
	      break;
	   }
	   //We don't need the streak variable here.  matching indexes works
	   if(start == end){
	      backslashWinCount++;
	      backslashWinner = *(*(board + end) + end);
	      break;
	   }
	   start++;
	   end--;
	}

	//backslashWinner could be 0, 1, or 2 here
	//forwardslash second
	int forwardslashWinCount = 0;		//keeps track of the number of wins
	int forwardslashWinner = 0;		//keeps track of the winner
	start = 0;
	end = size - 1;
	for(int i = 0; i < (size + 1)/2; i++){
	   //if start is a blank, break
	   if(*(*(board + start) + end) == 0){
	      break;
	   }
	   //if end is blank, break
	   if(*(*(board + end) + start) == 0){
	      break;
	   }
	   //if end and start are not the same, break
	   if(*(*(board + start) + end) != *(*(board + end) + start)){
	   break;
	   }
	   // if we reach this point, the streak exists
	   // save the part of the streak
	   if(start == 0){
	      detect = *(*(board + start) + end);
	   }
	   if(*(*(board + start) + end) != detect){
	      break;
	   }
	   //We don't need the streak variable here.  matching indexes works
	   if(start == end){
	      forwardslashWinCount++;
	      forwardslashWinner = *(*(board + start) + end);
	      break;
	   }
	   start++;
	   end--;
	}

	//forwardslashWinner could be 0, 1, or 2 here

	//My code already makes boards with more than 1 winner invalid
	// between the 4 winner variables, there are the following cases:
	// all 1
	// all 0
	// all 2
	// mix of 0 and 1
	// mix of 0 and 2
	
	//I need to now account for fringe cases.
	//Project specifications only allow for 2 winning lines.
	
        //Case where more than 2 winning X lines
	
	if(rowWinner == 1 || colWinner == 1 || 
			backslashWinner == 1 || forwardslashWinner == 1){
	   if(rowWinner + colWinner + backslashWinner 
				+ forwardslashWinner > 2){
	      return 0;
		}
	}

	//Case where more than 2 winning O lines
	
	if(rowWinner == 2 || colWinner == 2 || 
			backslashWinner == 2 || forwardslashWinner == 2){
	   if(rowWinner + colWinner + backslashWinner 
				+ forwardslashWinner > 4){
	      return 0;
		}
	}
	//The board is valid
	
    return 1;   
}    
  
 
   
/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    //TODO: Check if number of command-line arguments is correct
    if (argc > 2){
	printf("Only one file is allowed\n");
	exit(1);
    }
    if (argc == 1){
	printf("File name is required.\n");
	exit(1);
    }


    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;

    //TODO: Call get_dimensions to retrieve the board size.
    get_dimensions(fp, &size);
// DEBUGGING    printf("%d\n",size);
    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    int **board;					//the game board represented as a 2D array
    board = malloc(sizeof(int *) * size);
    if (board == NULL){
	printf("memory did not allocate properly.\n");
	exit(1);
    }
    for(int i = 0; i < size; i++){
	*(board + i) = malloc(sizeof(int) * size);
	if(*(board + i) == NULL){
	   printf("memory did not allocate properly.\n");
	   exit(1);
	}
    }
    //Read the file line by line.
    //Tokenize each line wrt the delimiter character to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, DELIM);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            *(*(board + i) + j) = atoi(token);
            token = strtok(NULL, DELIM);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    if(n_in_a_row(board, size) == 1){
	   printf("%s\n", "valid");
    }
    else{
	   printf("%s\n", "invalid");
    } 

    //TODO: Free all dynamically allocated memory.
    
    // board & internal areas of board
    for(int i = 0; i < size; i++){
	    free(*(board + i));
    }
    free(board);
    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       

// still to do
// close file?
// check return value of malloc and fopen

                                        // FIN

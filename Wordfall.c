#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define BOXHEIGHT 17
#define BOXWIDTH 70
#define MAXWORDLENGTH 20

void generate_board(char box[BOXHEIGHT][BOXWIDTH], char* gameBoardWords[15], int wordPositions[15]); // prototype for function that calls game board

void main(){
	
	struct initialQuestions{ //start of initialQuestions structure to store answers to initalQuestion (pre game)
		char userGameStart[3]; //string to hold user's response to whether they want to start the game or not (Y/N)
		char wordBankAdd[3]; //asks the user if they want to add words to the text file
		int wordAddQuantity; //number of words the user wants to enter to the word bank
	}initialQuestions;
	
	char box[BOXHEIGHT][BOXWIDTH]; //define 2d character array of defined width and length
	char* fullWordBank[1050]; // array 1050 long, each index contains char* string
	char* gameBoardWords[15];// array 15 long, each index contains char* string
	int wordPositions[15];//array 15 long, each index contains int value, keeps word position on gameboard
	char userEnteredWord[MAXWORDLENGTH]; // array of chars 15 characters long
	char indivWord[MAXWORDLENGTH]; // single word array of characters no longer than the predefined MAXWORDLENGTH
	char* userEnteredWords[25]; //words user is adding to word bank
	
	int gameOver  = 0; //sets game state. game can be exited by setting gameOver to true
	int wordBankWordQuantity = 0; // counts how many words in total word bank
	int score = 0; //holds players score
	int result = 0; //hold result for comparison
	int roundKeeper = 0;
	int masterRound = 0;
	int i; //index for use in for loop
	int roundNumber = 0;
	int scoreTest = 0;
	
	time_t startTime, endTime; //variables used to count time of user input
	srand(time(0)); //seeds random time
	
	FILE *myFilePointer;// creating file pointer to work with files
	myFilePointer = fopen("wordList.txt", "r"); // opening for reading and appending
	
	if(myFilePointer == NULL){
		printf("Unable to open file numbers.txt");
		exit(0);
	}
	
	while(!feof(myFilePointer)){// read full file that myFilePointer is pointing at
		fscanf(myFilePointer, "%s", indivWord);
		fullWordBank[wordBankWordQuantity] = (char*)malloc(sizeof(char)*(strlen(indivWord))+2); // dynamic memory allocation for fullWordBank
		strcpy(fullWordBank[wordBankWordQuantity], indivWord); // copies words from indivWord to the array fullWordBank at the given index based on wordBankWordQuantity
		wordBankWordQuantity++; //counts up word quantity for each word counted, allowing strcpy to copy all words
	}
	
	fclose(myFilePointer); //close file for reading
	
	FILE *myFilePointerAppend; //creating file pointer to append to the file
	myFilePointerAppend = fopen("wordList.txt", "a"); // opening for reading and appending
	
	if(myFilePointerAppend == NULL){
		printf("Unable to open file wordList.txt");
		exit(0);
	}
	
	printf("\n-------------------------------------------");
	printf("\n******");
	printf("\nWordfall");
	printf("\n******");
	printf("\nWould you like to play the game? (Y/N)");
	printf("\n-------------------------------------------\n");
	scanf("%s", initialQuestions.userGameStart);
		
	if(strcmp(initialQuestions.userGameStart, "Y") == 0 || strcmp(initialQuestions.userGameStart, "y") == 0){
		printf("--------------------------------------------------------");
		printf("\nGreat! Let's begin.");
		printf("\nWould you like to add any words to the word bank? (Y/N)");
		printf("\n--------------------------------------------------------\n");
		scanf("%s", initialQuestions.wordBankAdd);
			
		if(strcmp(initialQuestions.wordBankAdd, "Y") == 0 || strcmp(initialQuestions.wordBankAdd, "y") == 0){
			
			printf("\nHow many words would you like to add?\n"); //prints as described
			scanf("%d", &initialQuestions.wordAddQuantity); //holds number of words the user would like to add
		}
			
		else if(strcmp(initialQuestions.wordBankAdd, "N") == 0 || strcmp(initialQuestions.wordBankAdd, "n") == 0){
			printf("\nThat's okay, we have plenty of words already.\nLets start the game!");
		}
			
		if(initialQuestions.wordAddQuantity > 0){
			for(int i = 0; i < initialQuestions.wordAddQuantity; i++){ //asks user to enter words based on quantity, adds words to file
				printf("\nEnter word %d: ", i+1);
				scanf("%s", userEnteredWords[i]);
				fprintf(myFilePointerAppend, "%s\n", userEnteredWords[i]);
				fullWordBank[wordBankWordQuantity] = (char*)malloc(sizeof(char)*(strlen(userEnteredWords[i]))+2);
				strcpy(fullWordBank[wordBankWordQuantity], userEnteredWords[i]);
				wordBankWordQuantity++;
			}
			
			printf("\nWords appended to the end of the wordList.txt file are:");
			
			for(i = 0; i < initialQuestions.wordAddQuantity; i++){ //prints user entered words
				printf("\n%s", userEnteredWords[i]);
				
			}//end of section that prints the user entered words
		}//end of section that adds words to the file and word bank array
	} //end of "if user says they would like to start the game"
	else{ //user answered N to "Would you like to play the game? (Y/N)"
		printf("\nThats okay. See ya later!\n");
		exit(0);
	}
	
	fclose(myFilePointerAppend); //close file for reading
	
	while(gameOver == 0){ //game can be ended by setting gameOver to true
	
		roundNumber = roundNumber + 1;
		
		printf("\n--------------------");
		printf("\n THIS IS ROUND: %d", roundNumber);
		printf("\n--------------------");
		
		double wordAdder = 0.0;
		
		wordAdder = difftime(endTime, startTime);
		
		wordAdder = wordAdder * 1.25; //makes words appear faster reach round
		
		while(wordAdder > 0 || masterRound == 0){
			
			if(gameBoardWords[14] != NULL){ //condition that ends game if there's a word in the final index of gameBoardWords
				printf("\n");
				printf("\n* * * * * * * * * * * * * * * * ");
				printf("\n--------------------------------");
				printf("\n         GAME OVER!");
				printf("\n    You lost on round: %d", roundNumber);
				printf("\n");
				printf("\n       Final score: %d", score);
				printf("\n--------------------------------");
				printf("\n* * * * * * * * * * * * * * * * ");
				printf("\n");
				exit(0);
			}
			
			for(roundKeeper = 14; roundKeeper >= 0; roundKeeper--){
				gameBoardWords[roundKeeper] = gameBoardWords[roundKeeper-1]; // sets gameBoardWords at 14, sets it to gameBoardWords at 13
				wordPositions[roundKeeper] = wordPositions[roundKeeper-1];
			}
			
			int indexRandWord = rand()%wordBankWordQuantity; // psuedo randomly generates an index in range of 0 to wordBankWordQuantity (amount of words in .txt file)
			wordPositions[0] = rand() % (BOXWIDTH-(MAXWORDLENGTH)-2); //sets wordPositions to random value within boundaries
			gameBoardWords[0] = (char*)malloc(sizeof(char)*(MAXWORDLENGTH));//dynamic memory allocation for gameBoardWords
			strcpy(gameBoardWords[0], fullWordBank[indexRandWord]); //copies random word from the fullWordBank array at index (rand) to the first element of the gameBoardWords array
			wordAdder = wordAdder - 1;
			masterRound = masterRound + 1;
		}
		
		if(masterRound > 1 && score > 0){
			printf("\n Current score --->: %d", score);
		}
		
		printf("\n");
		generate_board(box, gameBoardWords, wordPositions); //calls game board

		printf("\nStarting timer now.");
		
		time(&startTime); //start timer to capture how long it takes user to type a word
		
		printf("\nEnter word here! --> ");
		scanf("%s", userEnteredWord); //scans user input to the first element of userEnteredWord array, use for comparison later
		
		time(&endTime); // ends timer to capture how long it takes user to type a word
		
		for(roundKeeper = 14; roundKeeper >= 0; roundKeeper--){
			if(gameBoardWords[roundKeeper] != NULL){
				result = strcmp(userEnteredWord, gameBoardWords[roundKeeper]); /* Compare the two strings provided */
				if (result == 0){ // If the two strings are the same, say so
					//printf("\nUser-entered word and random game board word are the same");
					scoreTest = strlen(userEnteredWord);
					score = score + scoreTest;
					printf("+%d Points!!!", scoreTest);
					gameBoardWords[roundKeeper] = '\0';
					break;
				}
				else{
					printf("User entered word does not match any of the game board words.");
				}
			}	
		}

	} //end of gameOver loop that starts the game
	
}//end of main function

void generate_board(char box[BOXHEIGHT][BOXWIDTH], char* gameBoardWords[15], int wordPositions[15]){
	
	for(int g = 0; g < BOXHEIGHT; g++){
		for(int h = 0; h < BOXWIDTH; h++){
			if(g > 0 && gameBoardWords[g-1] != NULL && h == wordPositions[g-1]){ //Inserts the word at the top and at the random position //game board has word at this index then print a word
				for(int i = 0; i < strlen(gameBoardWords[g-1]); i++){
					box[g][h] = gameBoardWords[g - 1][i];
					h++;
				}
				box[g][h] = ' ';
			}
			else if(g == 0 || g == BOXHEIGHT - 1)
				box[g][h] = '-';
			else if(h == 0 || h == BOXWIDTH - 1)
				box[g][h] = '|';
			else
				box[g][h] = ' ';
		}
	}
	for(int g = 0; g < BOXHEIGHT; g++){ //prints game board
			for(int h = 0; h < BOXWIDTH; h++){
				printf("%c", box[g][h]);
			}
			printf("\n");
	}
}
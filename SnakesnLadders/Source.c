#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//to use boolean values
typedef int bool;
#define true 1
#define false 0

//STUCTS for Snakes and Ladders start and end locations
typedef struct snake_t
{
	int head;
	int tail;
} snake_t;

typedef struct ladder_t
{
	int top;
	int bottom;
} ladder_t;

//Global Variables
int i, j;
int gameBoard[100];
struct snake_t snakes[7];
struct ladder_t ladders[7];
int numPlayers;
int *playerPos;
bool winner = false;
int roll;
int diceValue;
int playerWinner;
FILE*fptr;
char saveName[15];

//Function prototypes
int rollDice();
void initBoard();
void printBoard();
void playerTurn();
void openFile(char filename[15], char c[1]);
void closeFile();
void saveGame();
void loadGame();

void main()
{	
	int option;
	int choice;

	//seed random number based on current time
	srand(time(NULL));

	printf("======Snakes And Ladders======\n");

	//initial read
	printf("\nNew Game (1) Load Game (2) Quit (3)\n");
	scanf("%d", &option);

	while (option != 3)
	{
		switch (option)
		{
			case 1:
				do
				{
					printf("\nChoose number of players 2 - 6\n");
					scanf("%d", &numPlayers);
				} while (numPlayers < 2 || numPlayers > 6);

				//Dynamically allocate memory for array of player positions
				playerPos = (int *)malloc(numPlayers * sizeof(int));

				initBoard();
				printBoard();
				//Initilises player positions to 0
				for (i = 0; i < numPlayers; i++)
				{
					playerPos[i] = 0;
				}

				printf("\nRoll Dice(1) Save Game (2) Quit (3)\n");
				scanf("%d", &choice);

				while (choice != 3)
				{
					switch (choice)
					{
						case 1:
							//Roll Dice and take player turn, increment position, check for winner
					
							//If there is already a winner tell the user to return to main menu
							//Otherwise call playerTurn() function to rollDice and play a game
							if (winner == true)
							{
								printf("\nPlayer #%d Wins!!!\n\tReturn to main menu to start a new game", playerWinner);
							}
							else
							{
								//call playerTurn() function to start playing the game
								playerTurn();
							}

							break;
						case 2:
							//Save Game writes data to file
							saveGame();
							printf("Your Game has been saved to %s", saveName);
							break;
						case 3:
							printf("Quitting");
							//Quit
							break;
					} //end switch

					printf("\nRoll Dice(1) Save Game (2) Quit (3)\n");
					scanf("%d", &choice);
				} //end while
				break;
			case 2:
				//Load Game
				//Populate arrays with file data using loadGame()
				//Print the board
				//And resume the game using playerTurn()
				loadGame();
				printBoard();
				playerTurn();
				break;
			case 3:
				//Quit Game - break's switch
				break;
		}
		//subsequent read
		printf("\nNew Game (1) Load Game (2) Quit (3)\n");
		scanf("%d", &option);
	} //while

	getch();
}

void playerTurn()
{
	//playerTurn
	while (winner == false)
	{
		for (i = 0; i < numPlayers; i++)
		{
			printf("\nPlayer: #%d Position: %d Roll!", (i + 1), playerPos[i]);
			printf("\n\tRoll (1) Back (2): ");
			scanf("%d", &roll);

			if (roll == 1)
			{
				//Roll the dice
				diceValue = rollDice();
				printf("\t%d has been rolled!", diceValue);
				
				//increment the player position by the value rolled
				playerPos[i] += diceValue;

				//Loop through snakes and ladders arrays and check if player lands on either
				//If so, move the players position to the appropriate location
				for (j = 0; j < 7; j++)
				{
					if (playerPos[i] == snakes[j].head)
					{
						playerPos[i] = snakes[j].tail;
					}

					if (playerPos[i] == ladders[j].bottom)
					{
						playerPos[i] = ladders[j].top;
					}
				}

				//Check if the player position is greater than or equal to 100, if so set winner to true
				if (playerPos[i] >= 100)
				{
					winner = true;
					playerWinner = i + 1;

					printf("\nPlayer #%d Wins!!!", (i + 1));
					break;
				}

				//Print new player position
				printf("\n\tNew Player #%d Position: %d\n", (i + 1), playerPos[i]);
			}
			else
			{
				break;
			}
		} //end for

		if (roll == 2)
		{
			break;
		}
	} //end while
}

//Simple rollDice function which returns a random number between 1 - 6
int rollDice()
{
	return rand() % 6 + 1;
}

//initBoard() will generate random positions for snakes head and tail positions and ladders top and bottom positions
void initBoard()
{
	for (i = 0; i < 7; i++)
	{
		//snake head position cannot be located in the bottom row of the board (1 - 10),
		//cannot be located at 100, cannot be in an occupied position
		//hence generate random numbers that are greater than 10, not 100 and not occupied.
		do
		{
			snakes[i].head = rand() % 100 + 1;
		} while (snakes[i].head <= 10 || snakes[i].head == 100 || gameBoard[snakes[i].head] == 1);

		//fill position
		gameBoard[snakes[i].head] = 1;

		//snake tail location cannot be greater than the head,
		//cannot be in an occupied position
		//cannot be in the same row
		//hence generate random tail position until it is not occupied and less than the head position - head position % 10
		do
		{
			snakes[i].tail = rand() % 100 + 1;
		} while (snakes[i].tail > (snakes[i].head - snakes[i].head % 10) || gameBoard[snakes[i].tail] == 1);

		//fill position
		gameBoard[snakes[i].tail] = 1;

		//ladder top position cannot be located in first row (1-10),
		//cannot be occupied
		//hence generate random top position until it is not occupied and greater than 10
		do
		{
			ladders[i].top = rand() % 100 + 1;
		} while (ladders[i].top <= 10 || gameBoard[ladders[i].top] == 1);

		//fill position
		gameBoard[ladders[i].top] = 1;

		//ladder bottom position cannot be greater than the top,
		//cannot be occupied
		//cannot be in the same row
		//hence generate random bottom positions that are less than top - top position % 10 and no#'t occupied
		do
		{
			ladders[i].bottom = rand() % 100 + 1;
		} while (ladders[i].bottom > (ladders[i].top - ladders[i].top % 10) || gameBoard[ladders[i].bottom] == 1);

		//fill position
		gameBoard[ladders[i].bottom] = 1;
	} //end for

}

//Prints snakes - head and tail locations, ladders - top and bottom locations
void printBoard()
{
	printf("\nThe Snakes and Ladders Locations on the board are as follows: \n");
	for (i = 0; i < 7; i++)
	{
		printf("\nSnake #%d: Head Location: %d, Tail Location: %d", (i + 1), snakes[i].head, snakes[i].tail);
		printf("\nLadder #%d: Bottom Location: %d, Top Location: %d\n", (i + 1), ladders[i].bottom, ladders[i].top);
	}
}

//Opens file with the name specified by the user, the mode it is opened in is also determined by the user by entering the character as the 2nd arg
void openFile(char filename[15], char c[1])
{
	fptr = fopen(filename, c);

	if (fptr == NULL)
	{
		printf("ERROR opening file!  \n");
	}
}

//Closes File
void closeFile()
{
	fclose(fptr);
}

//saveGame() function writes array data to a file specified by the user
void saveGame()
{
	//Prompt the user to enter a name for their game save
	printf("Please enter a name for your save (eg: file.txt): ");
	scanf("%s", &saveName);

	//Open a new file to write with the name given by the user
	openFile(saveName, "w");

	//Save snakes and ladders positions - (board)
	for (i = 0; i < 7; i++)
	{
		fprintf(fptr, "%d \n%d \n%d \n%d \n", snakes[i].head, snakes[i].tail, ladders[i].top, ladders[i].bottom);
	}
	
	//Save number of players participating in the game
	fprintf(fptr, "\n%d \n\n", numPlayers);

	//Save the player positions
	for (i = 0; i < numPlayers; i++)
	{
		fprintf(fptr, "%d \n", playerPos[i]);		
	}

	closeFile();
}

//Loads file data and populates the arrays used to play the game with the data
void loadGame()
{
	//Prompt the user to enter the name of their saved game
	printf("Please enter the name of your saved game: ");
	scanf("%s", &saveName);

	//Open file to read
	openFile(saveName, "r");

	//Populate snakes and ladders arrays - (board)
	for (i = 0; i < 7; i++)
	{
		fscanf(fptr, "%d %d %d %d", &snakes[i].head, &snakes[i].tail, &ladders[i].top, &ladders[i].bottom);
	}

	//Read number of players participating in the game
	fscanf(fptr, "%d", &numPlayers);

	//Must dynamically allocate memory for playerPos array
	playerPos = (int *)malloc(numPlayers * sizeof(int));

	//Populate player positions array
	for (i = 0; i < numPlayers; i++)
	{
		fscanf(fptr, "%d", &playerPos[i]);
	}

	closeFile();
}
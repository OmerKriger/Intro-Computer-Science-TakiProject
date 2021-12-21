#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Omer Kriger, ID: 207752536
Let's Play TAKI :)
The Pogram is game called taki
all player in his turn choose throw card to the stack as the rule of the game
the player want free hand from cards and need throw his cards as the rules of the game say
*/

// Defines
#define MAX_LETTERS_FIRST_NAME 21
#define DEFAULT_CARD_START 4
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define STOP 10
#define CHANGE_DIR 11
#define PLUS2 12
#define TAKI 13
#define CHANGE_COLOR 14
#define COLORS 15
#define COLOR_YELLOW 15
#define COLOR_BLUE 16
#define COLOR_GREEN 17
#define COLOR_RED 18
#define NO_NUMBER -1
#define NO_COLOR -1
#define CARD_TYPES 14
#define NUM_OF_COLOR_CARDS 4
#define MULTIPLY_THE_HAND 2
#define UNDFINED -1

// Structs
typedef struct Card
{
	int type; // Colors (Green,Blue,Red,Yello) or spicel card (+2,STOP,Change Direction, TAKI,Change Color) types code saved in the defines
	int color; // card nunber if card is spicel number is -1
}stCard;
typedef struct Player
{
	char name[MAX_LETTERS_FIRST_NAME]; // Name of the player
	stCard* pPlayerCards; // the pointer for the array of the player hand
	int numOfCards; // logic size of the hand cards array
	int sizePlayerCards; // size of array cards
}stPlayer;
// Functions decleare
void Intro(); // V
void getPlayersInfo(stPlayer* pPlayers,int* pNumOfPlayers); // V
void getInfo(stPlayer* player, int playerNumber); // V
void getCards(stPlayer* player, int numOfCards); // 
void randomCard(stCard* pSlotCard); // V
void makeBiggerHand(stPlayer* player, int slotsToAdd); // 
void copyCardData(stPlayer* destPlayer, stCard* handSrc); // V
// Functions
void main()
{
	stPlayer* pPlayers = NULL;
	int numOfPlayers,i;
	Intro();
	getPlayersInfo(&pPlayers,&numOfPlayers);
	printf("DONE");
	for (i = 0; i < numOfPlayers; i++)
	{
		printf("Player #%d Name is: %s \n", i + 1, pPlayers[i].name);
		for (int j = 0; j < pPlayers[i].numOfCards; j++)
			printf("CardType %d Color: %d \n", pPlayers[i].pPlayerCards[j].type, pPlayers[i].pPlayerCards[j].color);
	}

}
void Intro()
{
	printf("************ Welcome to TAKI game !!! ************\n");
}
void getPlayersInfo(stPlayer** ppPlayers, int* pNumOfPlayers)
{
	int numOfPlayers,i;
	printf("Please enter the number of players: ");
	scanf("%d",&numOfPlayers);
	(*ppPlayers) = (stPlayer*) malloc(sizeof(stPlayer) * numOfPlayers);
	if ((*ppPlayers) == NULL)
	{
		printf("ERROR! Memory allocation failed ! Code:1 \n");
		exit(1);
	}
	for (i = 0; i < numOfPlayers; i++)
		getInfo((*ppPlayers)+i ,i+1);
	(*pNumOfPlayers) = numOfPlayers;
}
void getInfo(stPlayer* player,int playerNumber)
{
	char name[MAX_LETTERS_FIRST_NAME] = "\0";
	printf("Please enter the first name of player #%d: \n",playerNumber);
	scanf("%s",name);
	strcpy((*player).name, name);
	(*player).pPlayerCards = NULL;
	(*player).numOfCards = 0;
	(*player).sizePlayerCards = 0;
	getCards(player, DEFAULT_CARD_START);
}
void getCards(stPlayer* player, int numOfCardsToAdd)
{
	int i;
	if((*player).pPlayerCards == NULL || (*player).numOfCards + numOfCardsToAdd < (*player).sizePlayerCards) // check if there are a empty slots in hand for new cards 
		makeBiggerHand(player, numOfCardsToAdd); // make a bigger hand (make bigger array)
	// take cards
	for (i = 0; i < numOfCardsToAdd; i++) // take numOfCard from the deck
	{
		randomCard(((*player).pPlayerCards) + i); // get random card for this hand 
		(*player).numOfCards++;
	}
}
void randomCard(stCard* pSlotCard)
{
	int cardType,CardColor = NO_COLOR;
	cardType = 1 + rand() % CARD_TYPES; // pick a rondom card type between 1-14 types (1-9) numbers and (10-14) spicels
	if ((ONE <= cardType && cardType <= NINE) || cardType == STOP || cardType == CHANGE_DIR || cardType == PLUS2 || cardType == TAKI)
		CardColor = COLORS + rand() % NUM_OF_COLOR_CARDS;
	// setCard
	(*pSlotCard).type = cardType;
	(*pSlotCard).color = CardColor;
}
void makeBiggerHand(stPlayer* player,int slotsToAdd)
{
	int newSize;
	stCard* tempDataKeep = NULL; // pointer for the old data we want to copy for reallocate
	if ((*player).sizePlayerCards != 0)
	{
		newSize = sizeof(stCard) * ((*player).sizePlayerCards) + slotsToAdd; // set new size bigger x2 from the old size
		tempDataKeep = ((*player).pPlayerCards);
	}
	else
		newSize = sizeof(stCard)*slotsToAdd;
	// allocate
	((*player).pPlayerCards) = (stCard*) malloc(newSize); // allocate hand for player (stCard array)
	if ((*player).pPlayerCards == NULL) // if allocate failed quit from the pogram
	{
		printf("ERROR! Memory allocation failed ! Code:2 \n");
		exit(1);
	}
	else // if allocate success
	{
		if ((*player).sizePlayerCards != 0)
		{
			copyCardData(player, tempDataKeep);
			free(tempDataKeep);
		}
		(*player).sizePlayerCards = newSize;
	}
	
}
void copyCardData(stPlayer* destPlayer,stCard* handSrc)
{
	int i;
	for (i = 0; i < (*destPlayer).numOfCards; i++)
	{
		(*destPlayer).pPlayerCards[i] = handSrc[i];
	}
}

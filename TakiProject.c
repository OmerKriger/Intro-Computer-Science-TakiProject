#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Defines
#define MAX_LETTERS_FIRST_NAME 21
#define DEFAULT_CARD_START 4
#define COLOR_RANGE_START 1
#define COLOR_RANGE_END 4
#define ONE 1
#define SIZE_FOR_STATS 2
#define NINE 9
#define STOP 10
#define CHANGE_DIR 11
#define PLUS 12
#define TAKI 13
#define CHANGE_COLOR 14
#define COLORS 1
#define COLOR_YELLOW 1
#define COLOR_BLUE 3
#define COLOR_GREEN 4
#define COLOR_RED 2
#define NO_NUMBER -1
#define NO_COLOR -1
#define CARD_TYPES 14
#define START_CARDS 9
#define NUM_OF_COLOR_CARDS 4
#define MULTIPLY_THE_HAND 2
#define UNDFINED -1
#define STACK 0
#define STARS '*'
#define CARD_WIDTH 7 // the actully size is bigger in 2
#define CARD_LENGTH 6
#define SPACES ' '
#define THROW_CARD_MSG 99
#define TAKI_MSG 100
#define RAND_START 1
#define FIRST_CARD_HAND 1
#define TAKE_CARD_DECK 0
#define STATS_CARD 0
#define STATS_VALUE 1

// Structs

typedef struct Card
{
	int type; // Colors (Green,Blue,Red,Yello) or special card (+2,STOP,Change Direction, TAKI,Change Color) types code saved in the defines
	int color; // card nunber if card is special number is -1
}stCard;
typedef struct Player
{
	char name[MAX_LETTERS_FIRST_NAME]; // Name of the player
	stCard* pPlayerCards; // the pointer for the array of the player hand
	int numOfCards; // logic size of the hand cards array
	int sizePlayerCards; // size of array cards
}stPlayer;

// Functions decleare
void Intro();
void resetStats(int gameStats[CARD_TYPES][SIZE_FOR_STATS]); 
void getPlayersInfo(stPlayer** pPlayers,int* pNumOfPlayers, int gameStats[CARD_TYPES][SIZE_FOR_STATS]);
void getInfo(stPlayer* player, int playerNumber, int gameStats[CARD_TYPES][SIZE_FOR_STATS]);
void getCards(stPlayer* player, int numOfCards, int gameStats[CARD_TYPES][SIZE_FOR_STATS]); 
void randomCard(stCard* pSlotCard, bool startCard, int gameStats[CARD_TYPES][SIZE_FOR_STATS]); 
void makeBiggerHand(stPlayer* player, int slotsToAdd);
void copyCardData(stPlayer* destPlayer, stCard* handSrc); 
void printMsgs(int num,int msgCode);
void showCards(stCard* playerCards, stCard stackCard, int numOfCards, char name[]); 
void printLineOf(char charToPrint, int amount);
void printCard(stCard currentCard);
void printCardTypeDetails(stCard currentCard); 
void printCardColorDetails(stCard currentCard); 
bool gameStart(stCard* startCard, int gameStats[CARD_TYPES][SIZE_FOR_STATS]); 
bool logicTestOfRules(stPlayer* pPlayer, int cardChosen, stCard UpperCard, bool takiMODE, bool stillMyTurn);
void throwCard(stPlayer* pPlayer, int cardIndexInHand, stCard* UpperCard); 
bool turnOfPlayer(stCard* UpperCard, stPlayer* pPlayer, int* roundDir, int gameStats[CARD_TYPES][SIZE_FOR_STATS],bool gameOfTwo);
void swapCardPos(stCard* card1, stCard* card2); 
void specialCardExecute(stCard* UpperCard, bool* takiMODE, int* takiRound , bool* stillMyTurn, int* roundDir, int numOfCards, stPlayer* pPlayer, int gameStats[CARD_TYPES][SIZE_FOR_STATS], bool gameOfTwo);
void changeColorFunc(stCard* UpperCard);
void WinnerMsgAndStats(stPlayer winner, int gameStats[CARD_TYPES][SIZE_FOR_STATS]);
void StatsOrderSWAP(int* arr1, int* arr2);
void freeMemory(stPlayer* pPlayers, int numOfPlayers);

// Functions
void main()
{
	// varibles define
	srand(time(NULL)); // run the rand func and put the time function for seed the rand func.
	int gameStats[CARD_TYPES][SIZE_FOR_STATS]; 
	// array that will keep the stats of card frequnecy the rows at the start will be for the card type and at the end will be represent the card with the highest frequency
	// in cols the first cell keep the card type and the second cell is for the times 
	stPlayer* pPlayers = NULL; // pointer for players array (start null and function getPlayerInfo create the array later)
	stCard lastCardThrow; // this sturct keep the last card the players puted in the stack
	bool GameOfTwo = false; // boolian var if is two players game it is true and for any other amount of players is false (made for special rules for two players game)
	int roundDirection = 1; // this int keep for round direction 1 or -1 (represent "Clockwise" or "Anti-Clockwise")
	int numOfPlayers,turnNum=0; // turnNum is counter of the turns it increase or decrease depend in direction of the game
	int turnOfPlayerNum;
	bool gameON; // this boolian var is for the while loop that keep the game on
	// functions call
	Intro(); // intro of the game say welcome
	resetStats(gameStats); // this function set the stats array of the game for the new game 
	getPlayersInfo(&pPlayers,&numOfPlayers,gameStats); // get from players info and make player in the game
	if (numOfPlayers == 2) // we check if two players game for the speical rules
		 GameOfTwo = true;
	gameON = gameStart(&lastCardThrow, gameStats);// set the last things for begin the game (start card in stack)
	while (gameON) // while game running
	{
		if (turnNum < 0)// if turnNum is negative we want modlue positive so we will multiply -1 turnNum , (turnNum % numOfPlayers) is the number of player will play his turn we will get number between 0 to (numofPlayers-1) we will use it for the index players array
			turnOfPlayerNum = (-1 * turnNum) % numOfPlayers;
		else
			turnOfPlayerNum = turnNum % numOfPlayers;
		
		gameON = turnOfPlayer(&lastCardThrow, pPlayers + turnOfPlayerNum, &roundDirection, gameStats,GameOfTwo); // let player play his turn 
		if (gameON == true)
		{
			turnNum = turnNum + roundDirection; // layed if card stop proundDir become 2 or -2 depend in direction of playing
			if (roundDirection == 2 || roundDirection == -2) // reset the STOP card skip after we jump on player turn
				roundDirection /= 2; // make it 1 or -1 again;
		}
	}
	WinnerMsgAndStats(pPlayers[turnNum % numOfPlayers], gameStats); // call function sort the data we collected in the game, print the winner and show the satistics
	freeMemory(pPlayers, numOfPlayers); // free the memory we allocated in the pogram
	// the end of the game
}
void Intro() // function for intro say welcome to taki game
{
	printf("************ Welcome to TAKI game !!! ************\n");
}
void getPlayersInfo(stPlayer** ppPlayers, int* pNumOfPlayers, int gameStats[CARD_TYPES][SIZE_FOR_STATS]) // this function get the info about number of players and create array for players
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
	for (i = 0; i < numOfPlayers; i++) // we get here all players details
		getInfo((*ppPlayers)+i ,i+1,gameStats); // this function getInfo into player name and cards
	(*pNumOfPlayers) = numOfPlayers; // set the number of Players in the main function by reference
}
void getInfo(stPlayer* player,int playerNumber, int gameStats[CARD_TYPES][SIZE_FOR_STATS]) // this function will get player name and set his staff to zero
{
	char name[MAX_LETTERS_FIRST_NAME] = "\0";
	printf("Please enter the first name of player #%d: \n",playerNumber);
	scanf("%s",name);
	strcpy((*player).name, name); // we cpy the player name to player struct
	// set all varibles to zero
	(*player).pPlayerCards = NULL; 
	(*player).numOfCards = 0; 
	(*player).sizePlayerCards = 0;
	getCards(player, DEFAULT_CARD_START, gameStats); // get the players the cards he will start the game  
}
void getCards(stPlayer* player, int numOfCardsToAdd, int gameStats[CARD_TYPES][SIZE_FOR_STATS]) // the function get the player cards from "deck" by random function
{
	int i;
	int numOfCards = (*player).numOfCards;
	if ((*player).pPlayerCards == NULL || (*player).numOfCards + numOfCardsToAdd > (*player).sizePlayerCards) // check if there are a empty slots in hand for new cards 
		makeBiggerHand(player, numOfCardsToAdd); // make a bigger hand (make bigger array)
	// take cards
	for (i = numOfCards; i < numOfCards + numOfCardsToAdd; i++) // run for few time for the amount of cards it need to get from the deck
	{
		randomCard(((*player).pPlayerCards) + i, false, gameStats); // get a random card for specific slot in the array of cards (take a card from the deck to the hand of player)
		(*player).numOfCards++; // increase in one the numOfCards in the hand of player
	}

}
void randomCard(stCard* pSlotCard,bool startCard,int gameStats[CARD_TYPES][SIZE_FOR_STATS])
{
	int cardType,CardColor = NO_COLOR; // default card no color untill he get from random so if he wont get this is no color card
	if(startCard == false)
		cardType = RAND_START + rand() % CARD_TYPES; // pick a random card type between 1-14 types (1-9) numbers and (10-14) specials
	else
		cardType = RAND_START + rand() % START_CARDS; // pick a random card type between 1-9 for start card
	if ((ONE <= cardType && cardType <= NINE) || cardType == STOP || cardType == CHANGE_DIR || cardType == PLUS || cardType == TAKI) // pick color for this cards
		CardColor = COLORS + rand() % NUM_OF_COLOR_CARDS; // pick a random color
	// setCard
	gameStats[cardType][STATS_VALUE]++; // increase in one the valuse in the stats
	(*pSlotCard).type = cardType; // make card type for what picked up in random
	(*pSlotCard).color = CardColor; // make card color for what picked up in random
}
void makeBiggerHand(stPlayer* player,int slotsToAdd)
{
	stCard* tempDataKeep = NULL; // pointer for the old data we want to copy for reallocate
	int currentSize = (*player).sizePlayerCards; // current size - physic size of the array
	int newSize = currentSize; // newSize physic size temp equal to currnetSize
	if (currentSize > 0) // if current physic size bigger than zero so the array already exist
	{
		while (newSize < currentSize+slotsToAdd) // we check here how much time we need to multiply the size for will be enough space for all the cards we want to add
			newSize *= MULTIPLY_THE_HAND; // set new size bigger x2 from the old size
		tempDataKeep = ((*player).pPlayerCards); // we keep the data of this cards in the side for data wont lost
	}
	else // first time we make array
		newSize = slotsToAdd; // if physic size is not positive so array didnt exsit so make the newSize for the amount we need of cards
	// allocate
	((*player).pPlayerCards) = (stCard*) malloc(sizeof(stCard)*newSize); // allocate hand for player (stCard array)
	if ((*player).pPlayerCards == NULL) // if allocate failed quit from the pogram
	{
		printf("ERROR! Memory allocation failed !\n");
		exit(1); // something critical happend so we canot continue this pogram exit from everything
	}
	else // if allocate success
	{
		if ((*player).sizePlayerCards != 0) // if this isnt the first array of cards
		{
			copyCardData(player, tempDataKeep); // we copy the Card data from the old array to the new one
			free(tempDataKeep); // we release the old array from the memory
		}
		(*player).sizePlayerCards = newSize; // save the new physic size in the player info
	}
}
void copyCardData(stPlayer* destPlayer,stCard* handSrc) // this function copy data from one array of cards to a new one
{
	int i;
	for (i = 0; i < (*destPlayer).numOfCards; i++) // run on all of the cards
	{
		(*destPlayer).pPlayerCards[i] = handSrc[i]; // copy a card by card
	}
}
void showCards(stCard* playerCards,stCard stackCard, int numOfCards, char name[]) // this function show all the cards player have and the upper card in the stack
{
	int j;
	for (j = 0; j <= numOfCards; j++) // 0 = STACK card 1+ = HAND cards
	{
		// print title
		if (j == STACK) // show the upper stack card first
		{
			printf("\nUpper card: \n");
			printCard(stackCard); // print the card
		}
		else if (j >= FIRST_CARD_HAND) // show ater it all the rest of cards
		{
			if (j == FIRST_CARD_HAND) // print the player who is his turn
				printf("%s's turn: \n", name);
			printf("\nCard #%d: \n", j); // print of card number
			printCard(playerCards[j-1]); // print the card
		}
		printf("\n");
	}
}
void printCard(stCard currentCard)
{
	int i;
	// print the card
	for (i = 1; i <= CARD_LENGTH; i++)
	{
		printf("*"); // left boarder of card
		if (i == 1) // top boarder
			printLineOf(STARS, CARD_WIDTH); // upper line of stars
		else if (i == CARD_LENGTH) // bottom boarder
			printLineOf(STARS, CARD_WIDTH); // bottom line of stars
		else if (i == CARD_LENGTH / 2) // middle up
			printCardTypeDetails(currentCard); // print type/number
		else if (i == (CARD_LENGTH / 2) + 1) // middle down
			printCardColorDetails(currentCard); // print color
		else if (i < (CARD_LENGTH / 2) || (CARD_LENGTH / 2) + 1 < i) // if isnt the middle of the card in first and last lines
			printLineOf(SPACES, CARD_WIDTH); // print blank spaces
		printf("*\n"); // right boarder of card
	}
}
void printLineOf(char charToPrint, int amount) // this function print line of char
{
	int i;
	for (i = 0; i < amount; i++) // time we want to print
		printf("%c", charToPrint); // print the char
}
void printCardTypeDetails(stCard currentCard) // this function print the details of the type of card
{
	if (ONE <= currentCard.type && currentCard.type <= NINE) // print of regular card 1-9 numbers
		printf("   %d   ", currentCard.type);
	else // print of spiecal cards
	{
		switch (currentCard.type) // which type of card we print
		{
		case CHANGE_COLOR:
			printf(" COLOR ");
			break;
		case TAKI:
			printf("  TAKI ");
			break;
		case CHANGE_DIR:
			printf("  <->  ");
			break;
		case PLUS:
			printf("   +   ");
			break;
		case STOP:
			printf("  STOP ");
			break;
		default:
			printf(" ERROR "); // something going wrong so print error (shoudnt print)
		}
	}
}
void printCardColorDetails(stCard currentCard) // this function print the color of the card
{
	switch (currentCard.color) // check which color we print
	{
	case COLOR_RED:
		printf("   R   ");
		break;
	case COLOR_GREEN:
		printf("   G   ");
		break;
	case COLOR_BLUE:
		printf("   B   ");
		break;
	case COLOR_YELLOW:
		printf("   Y   ");
		break;
	case NO_COLOR: // when no color card print spaces
		printLineOf(SPACES, CARD_WIDTH);
	}
}
bool gameStart(stCard* startCard, int gameStats[CARD_TYPES][SIZE_FOR_STATS]) // this function starting the game pick a random card for first card in stack
{
	randomCard(startCard,true, gameStats);
	return true; // return true to the game while loop will run
}
bool turnOfPlayer(stCard* UpperCard, stPlayer* pPlayer, int* roundDir, int gameStats[CARD_TYPES][SIZE_FOR_STATS],bool gameOfTwo)
{ // need to add taki mode
	int choose=UNDFINED;
	bool stillMyTurn = 0; // if work only if player put special cards
	bool takiMODE = false,ChoiceIsOK = false;
	int takiRoundLimit = 0;
	int numOfCards = (*pPlayer).numOfCards;
	 // print the player hand cards
	
	while (choose == UNDFINED || stillMyTurn || takiMODE) // work if player didnt choose card or the turn is repeating because special card
	{
		showCards((*pPlayer).pPlayerCards,*UpperCard, numOfCards,(*pPlayer).name); // show the player his cards and the top card in the stack
		if (stillMyTurn == true) // disable turn for next turn
			stillMyTurn = false;
		if (takiRoundLimit == 1 || takiMODE == false) // print regular msg or taki msg (taki msg round 2+ is for press 0 for finish the turn)
			printMsgs((*pPlayer).numOfCards, THROW_CARD_MSG);
		else
			printMsgs((*pPlayer).numOfCards, TAKI_MSG);
		scanf("%d", &choose);
		ChoiceIsOK = logicTestOfRules(pPlayer, choose, *UpperCard, takiMODE, stillMyTurn);
		while ( ChoiceIsOK == false) // while the card that player choose is illegal keep asking for new one
		{
			printf("Invalid choice! Try again.\n");
			if (takiRoundLimit == 1) // print regular msg or taki msg
				printMsgs((*pPlayer).numOfCards, TAKI_MSG);
			else
				printMsgs((*pPlayer).numOfCards, THROW_CARD_MSG);
			scanf("%d", &choose); // get choose again
			ChoiceIsOK = logicTestOfRules(pPlayer, choose, *UpperCard, takiMODE, stillMyTurn); // test the logic rule for next round
		}
		if (choose > 0) // if player didnt choose take card from the deck throw this card and if card is spiecal execute his rules
		{
			throwCard(pPlayer, choose, UpperCard); // throw the card the played choose
			numOfCards = (*pPlayer).numOfCards;
			if (takiRoundLimit == 1)
				takiRoundLimit = 0; // the limit of first round of taki end
			if(takiMODE == false || (*UpperCard).type == CHANGE_COLOR || numOfCards == 0) // execute speical card only if takiMODE false or player choosed change color (but taki will be end after it)
				specialCardExecute(UpperCard, &takiMODE,&takiRoundLimit, &stillMyTurn, roundDir, numOfCards, pPlayer, gameStats, gameOfTwo); // execute the special rules for this card				
		}
		else
			if (takiMODE == false)
				getCards(pPlayer, ONE, gameStats); // if NO taki so player choosed take card from deck so we will get a new card
			else
			{
				specialCardExecute(UpperCard, &takiMODE, &takiRoundLimit, &stillMyTurn, roundDir,numOfCards,pPlayer,gameStats,gameOfTwo); // execute the special rules for this card if he is the last card when taki card end (if last card was taki so takiMODE will turn off)
				if (takiRoundLimit == 1); // if last card is the player throw is taki so the player need take card from the deck as the rules says
				{
					takiRoundLimit = 0; // reset the taki round
					getCards(pPlayer, ONE, gameStats); // get card from the deck
				}
				takiMODE = false;// we turn off takiMODE if it is on because player throwed only taki card and finish his turn
			}

		numOfCards = (*pPlayer).numOfCards;
		if (numOfCards == 0 && stillMyTurn == false && takiRoundLimit != 1) // if player no cards anymore and no
			return false; // stop the game
	}
	return true;
}
void printMsgs(int num, int msgCode) // print the messages before he decide what to throw
{
	switch (msgCode) // get msgCode and decide which msg send player
	{
	case THROW_CARD_MSG:
		printf("Please enter 0 if you want to take a card from the deck\n"); // msg for taki first turn and regular turn
		break;
	case TAKI_MSG: 
		printf("Please enter 0 if you want to finish your turn\n"); // msg for taki turns exclude first taki turn
		break;
	}
	printf("or 1-%d if you want to put one of your cards in the middle:\n", num); // print the option for throw away
	
}
bool logicTestOfRules(stPlayer* pPlayer, int cardChosen, stCard UpperCard,bool takiMODE, bool stillMyTurn) // this function check if the choice of card the player made is legal by the rules of the game and the cards he have actully exist
{
	// check if the number chosen is legal
	if (cardChosen == TAKE_CARD_DECK) // 0 is legal always we can anytime take from deck or in taki to finish out turn
		return true;
	if (cardChosen < 0 || (*pPlayer).numOfCards < cardChosen) // if card isnt exist in player hand return false
		return false;

	stCard CardToTest = (*pPlayer).pPlayerCards[cardChosen - 1]; // keep the card we check in
	bool cardIsOK = false;
	// check the logic rules for this card
	if (UpperCard.type == CardToTest.type && takiMODE == false) // we check if the the type is same like the upper card in the stack and there are no taki
		cardIsOK = true; // mark this choice as legal
	else if (UpperCard.color == CardToTest.color || CardToTest.color == NO_COLOR) // check if the color is same or change color card
		cardIsOK = true; // mark this choice as legal
	if (cardIsOK == false) // if card isnt good till now return false because he failed all the laws
		return false; // card isnt good
	return true;
}
void throwCard(stPlayer* pPlayer, int cardInHand,stCard* UpperCard) // the function take the choice of the player and remove the card from his hand (array of cards) and put this card in the stack
{
	int lastCardInHandIndex = ((*pPlayer).numOfCards)-1; // keep the index of the last card in player hand (array of cards)
	if (cardInHand < lastCardInHandIndex+1) // we check if the player choosed the last card in the array of not
	{ // if it is not the last card we will swap this card with the last card and decrease the number of card for "remove" it from the array.
		swapCardPos( &( (*pPlayer).pPlayerCards[cardInHand-1] ), &( (*pPlayer).pPlayerCards[lastCardInHandIndex] ) ); // put the card we want to throw last in the hand
		(*UpperCard) = (*pPlayer).pPlayerCards[lastCardInHandIndex]; // throw the last card (put him in the stack)
		(*pPlayer).numOfCards--; // remove the card from the hand (decrease the logic size of the array)
	}
	else
	{
		(*UpperCard) = (*pPlayer).pPlayerCards[cardInHand-1]; // throw the last card (put him in the stack)
		(*pPlayer).numOfCards--; // remove the card from the hand (decrease the size of the array)
	}
}
void swapCardPos(stCard* card1, stCard* card2) // make swaping between 2 card positions
{
	stCard temp = *card2;
	*card2 = *card1;
	*card1 = temp;
}
void specialCardExecute(stCard* UpperCard, bool* takiMODE,int* takiRound, bool* stillMyTurn, int* roundDir, int numOfCards, stPlayer* pPlayer, int gameStats[CARD_TYPES][SIZE_FOR_STATS], bool gameOfTwo)
{ // this function check which special card the player throwed to the stack and care that the player get another round or the direction of the game will change clockwise or anti clock wise and ect.
	int ThrowenCard = (*UpperCard).type;
	switch (ThrowenCard) // check which card the player throw
	{
	case STOP:
		if (numOfCards == 0 && gameOfTwo == true) // if its two players game and last card of player was STOP he need take card from deck
		{
			getCards(pPlayer, ONE, gameStats);
			*takiMODE = false; // takiEnd no cards (if is true)
		}
		else
			*roundDir *= 2; // we change the round for skip 2 or -2 depend in the direction for skip one player (after we will add 2/-2 it will back to 1/-1)
		break;
	case CHANGE_DIR:
		if (gameOfTwo == true) // in two players game the turn is still belong to the same player
			*stillMyTurn = true; // get another turn
		else
			*roundDir *= -1; // we change the round for the oppsite from neg to pos or from pos to neg
		break;
	case TAKI:
		if (*takiMODE == false)
		{
			*takiMODE = true;  // we turn on takiMODE it will keep the player turn untill he will choose 0 for finish his turn
			*takiRound = 1;
		}
		break;
	case CHANGE_COLOR:
		if (*takiMODE == true)
			*takiMODE = false; // change color finish taki turn
		changeColorFunc(UpperCard); // ask the player which color he want change the card
		break;
	case PLUS:
		if (numOfCards == 0 || *takiMODE == true) // if last card is PLUS the player will get card automaticly
		{
			getCards(pPlayer, ONE, gameStats);
			*takiMODE = false; // takiEnd no cards
		}
		else
			*stillMyTurn = true; // give the player one more card to throw away from the hand (in next while loop it back to be false value)
	}
}
void changeColorFunc(stCard* UpperCard) // function for the card change color, the function ask the player which color he want to change
{
	int color = UNDFINED; // default value
	while (color == UNDFINED) // while loop for color undefined
	{
		// print msg of how to choose color
		printf("Please enter your color choice:\n");
		printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
		scanf("%d", &color); // get from player the color
		if (color < COLOR_RANGE_START && COLOR_RANGE_END < color) // check if color in legal range or not
		{
			color = UNDFINED; // the number isnt legal so color undefine for continue while loop
			printf("Invalid choice!Try again.\n"); // illegal choice msg
		}
	}
	(*UpperCard).color = color; // set the new color for the card
}
void WinnerMsgAndStats(stPlayer winner, int gameStats[CARD_TYPES][SIZE_FOR_STATS]) // function decleare who won and reorder the stats matrix and at the end print the statistic of the game
{
	int i, j;
	printf("The winner is... %s! Congratulations!\n\n",winner.name); // winner decleare
	for(i=0; i<CARD_TYPES-1;i++) // bubble sort two the matrix 
		for(j=0; j<CARD_TYPES-i-1;i++)
			if (gameStats[j][STATS_VALUE] > gameStats[j + 1][STATS_VALUE]) // check which CARD apears more times in the game
			{
				StatsOrderSWAP(gameStats[j], gameStats[j + 1]);// and making a swap
			}
	// print the game Statistics
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	for (i = CARD_TYPES - 1; i >= 0; i--) // start from the end because we make reorder that the highest shown at the end of the matrix
	{
		if (ONE <= gameStats[i][STATS_CARD] && gameStats[i][STATS_CARD] <= NINE) // print for 1-9 cards
			printf("   %d   |    %d \n", gameStats[i][STATS_CARD], gameStats[i][STATS_VALUE]);// print the basic card and times (1-9)
		else // print for spiecal cards STOP,+,change color, taki,change direction
		{
			switch (gameStats[i][STATS_CARD]) // printf the card uniqe names
			{
			case TAKI:
				printf(" TAKI  |");
				break;
			case STOP:
				printf(" STOP  |");
				break;
			case CHANGE_COLOR:
				printf(" COLOR |");
				break;
			case CHANGE_DIR:
				printf("  <->  |");
				break;
			case PLUS:
				printf("   +   |");
				break;
			}
			printf("    %d \n", gameStats[i][STATS_VALUE]); // print the times of uniqe cards
		}
	}

}
void StatsOrderSWAP(int* arr1, int* arr2) // function make swap between arrays in matrix
{
	int tempArr[SIZE_FOR_STATS];
	tempArr[STATS_CARD] = arr1[STATS_CARD];
	tempArr[STATS_VALUE] = arr1[STATS_VALUE];
	arr1[STATS_CARD] = arr2[STATS_CARD];
	arr1[STATS_VALUE] = arr2[STATS_VALUE];
	arr2[STATS_CARD] = tempArr[STATS_CARD];
	arr2[STATS_VALUE] = tempArr[STATS_VALUE];
}
void resetStats(int gameStats[CARD_TYPES][SIZE_FOR_STATS])
{
	int i;
	for (i = 0; i < CARD_TYPES; i++) // order all the card types by frequency they showed in the game
	{
		gameStats[i][STATS_CARD] = i+1; // make the first cell value the card code (1-9) for basic cards and (10-14) for special 
		gameStats[i][STATS_VALUE] = 0; // reset the counter for zero
	}
}
void freeMemory(stPlayer* pPlayers, int numOfPlayers) // release all the memory we allocated in the pogram
{
	int i;
	for (i = 0; i < numOfPlayers; i++) // run on all the players
	{
		free(pPlayers[i].pPlayerCards); // realse the card array we allocated
		pPlayers[i].sizePlayerCards = 0; // set the size to 0
		pPlayers[i].numOfCards = 0; // set num of cards to 0
	}
	free(pPlayers); // free the players array
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_DIE	6

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void opening(void)
{
	printf("####################\n");
	printf("||||||||||||||||||||\n");
	printf("     START GAME     \n");
	printf("||||||||||||||||||||\n");
	printf("####################\n");
}

int rolldie(void)
{
	return rand() % MAX_DIE+1;
}

int main(int argc, char *argv[]) {
	int pos = 0;
	srand((unsigned)time(NULL));
	
	// 0. opening 
	opening();
	
	// 1. initializing & decide player's name
	board_initboard();
	// 2. loop(player's turn)
	do {
		int board_getBoard, c; //*********
		int step = rolldie();
		int coinResult = board_getBoard;
		pos += step;
		printf("~to ~from");
		coinResult = board_getBoardCoin(pos);
		
		printf("press any key to continue.");
		scanf("%d", &c);
		fflush(stdin);
		
		// 2-1. print player's statue
		// 2-2. roll a dice
		// 2-3. move
		// 2-4. collect coins
		// 2-5. next turn
		// 2-6. if(Every player takes a turn)
			// move the shark
	}
	while(1);
		
	// 3. calculate, print winner 
	
	return 0;
}

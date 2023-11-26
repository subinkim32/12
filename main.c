#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_CHARNAME	200 
#define N_PLAYER	3
#define MAX_DIE		6

#define PLAYERSTATUS_LIVE	0
#define PLAYERSTATUS_DIE	1
#define PLAYERSTATUS_END	2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = {"LIVE", "DIE", "END"};


void opening(void)
{
	printf("####################\n");
	printf("||||||||||||||||||||\n");
	printf("     START GAME     \n");
	printf("||||||||||||||||||||\n");
	printf("####################\n");
}

void ending(void)
{
	printf("\n");
	printf("####################\n");
	printf("||||||||||||||||||||\n");
	printf("      GAME END      \n");
	printf("||||||||||||||||||||\n");
	printf("####################\n");
}

int rolldie(void)
{
	return rand() % MAX_DIE+1;
}

void printPlayerPosition(int player)
{
	int i;
	for (i=0; i<N_BOARD; i++)
	{
		printf("|");
		if (i == player_position[player])
			printf("%c", player_name[player][0]);
		else
		{
			if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
				printf("X");
			else
				printf(" ");
		}
	}
	printf("|\n");
}

void printPlayerStatus(void)
{
	int i;
	printf("player status ----\n");
	
	for (i=0; i<N_PLAYER; i++)
	{
		printf("%s: pos %i, coin %i, status: %s\n",	player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
	}
	printf("-----------------------\n");
}

void checkDie(void)
{
	int i;
	int turn = (turn + 1) % N_PLAYER;
	int deadPlayers[N_PLAYER];
	int numDeadPlayers = 0;
	
	for (i=0; i<N_PLAYER; i++)
	{
		if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
			{
				player_status[i] = PLAYERSTATUS_DIE;
				deadPlayers[numDeadPlayers++] = i;
			}	
	}
	
	if (numDeadPlayers > 0)
	{
		printf("Dead players: ");
		for (i=0; i<numDeadPlayers; i++)
			printf("%s\n", player_name[deadPlayers[i]]);
	}	
}

int game_end(void)
	{
		int i;
		int flag_end = 1;
		
		//if all the players are died
		for (i=0; i<N_PLAYER; i++)
		{
			if (player_status[i] == PLAYERSTATUS_LIVE)
			{
				flag_end = 0;
				break;
			}
		}
		
		return flag_end;
	}
	
int getAlivePlayer(void)
	{
		int i;
		int cnt = 0;
		for (i=0; i<N_PLAYER; i++)
		{
			if (player_status[i] == PLAYERSTATUS_END)
				cnt++;
		}
		return cnt;
	}
	
int getWinner(void)
	{
		int i;
		int winner = 0;
		int max_coin = -1;
		
		for (i=0; i<N_PLAYER; i++)
		{
			if (player_coin[i] > max_coin)
			{
				max_coin = player_coin[i];
				winner = i;
			}
		}
		return winner;
	}
	
int allPlayersAlive(void) {
	int i;
	for (i=0; i<N_PLAYER; i++)
	{
		if (player_status[i] != PLAYERSTATUS_LIVE)
		{
			return 0;
		}
	}
	return 1;
}
	
int main(int argc, char *argv[]) {
	int pos = 0;
	int i;
	int turn = 0;
	srand((unsigned)time(NULL));
	
	// 0. opening 
	opening();
	
	// 1. initializing & decide player's name
	board_initboard();
		// 1-2.
	for (i=0; i<N_PLAYER; i++)
	{
		player_position[i] = 0;
		player_coin[i] = 0;
		player_status[i] = PLAYERSTATUS_LIVE;
		//player name
		printf("Player %i's name: ", i);
		scanf("%s", player_name[i]);
	}
	
	
	// 2. loop(player's turn)
	int game_end = 0;
	do {
		int coinResult;
		int step;
		char c;
		
		if (player_status[turn] != PLAYERSTATUS_LIVE)
		{
			turn = (turn + 1) % N_PLAYER;
			continue;
		}
			
		
		// 2-1. print player's statue
		board_printBoardStatus();
		
		for (i=0; i<N_PLAYER; i++)
			printPlayerPosition(i);
		printPlayerStatus();
		
		// 2-2. roll a dice
		printf("%s turn!\n", player_name[turn]);
		printf("press any key to roll a die\n");
		
		scanf("%d", &c);
		fflush(stdin);
		
		step = rolldie();
		
		// 2-3. move
		player_position[turn] += step;
		if (player_position[turn] >= N_BOARD)
		{
			player_position[turn] = N_BOARD - 1;
		}
		if (player_position[turn] == N_BOARD -1)
		{
			player_status[turn] = PLAYERSTATUS_END;
		}
		printf("Die result : %d, %s moved to %d!\n", step, player_name[turn], player_position[turn]);
		
		
		// 2-4. collect coins
		coinResult = board_getBoardCoin(pos);
		player_coin[turn] += coinResult;
		printf("%s collected %d coins!\n", player_name[turn], coinResult);
		
		// 2-5. next turn
		turn = (turn + 1) % N_PLAYER;
		
		// 2-6. if(Every player takes a turn)
		if (turn == 0)
		{
			// move the shark
			int shark_pos = board_stepShark();
			
			//printf(); the shark moved
			checkDie();
		}
	} while(!game_end);
		
	// 3. calculate, print winner
	int alivePlayers = getAlivePlayer();
	int winnerIndex = getWinner();
	
	if (alivePlayers == N_PLAYER)
	{
		//All players survived
		printf("Congratulations! All players survived!\n");
		printf("Winner is %s!!", player_name[winnerIndex]);
	}
	else
	{
		printf("%s reached to the end! (coin: %d)", player_name[winnerIndex], player_coin[winnerIndex]);
		printf("%d player(s) is/are alive! Winner is %s", alivePlayers, player_name[winnerIndex]);
	}
	
	
	
	// 4. ending
	ending();
	
	return 0;
}

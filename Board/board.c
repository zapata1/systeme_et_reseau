#include <stdio.h>
#include <stdlib.h> /* random */
#include <time.h>	/* time */
#include <stdint.h>
#include <string.h>

#include "board.h"

/* Boards */

uint8_t board0[BOARD_SIZE][BOARD_SIZE] = {
			{EMPTY, INTERROGATION_POINT, EMPTY},
			{SPAWN, WRECK, BLUE_PLANKTON},
			{EMPTY, WRECK, EMPTY}};


uint8_t board1[BOARD_SIZE][BOARD_SIZE] = {
			{EMPTY, WRECK, EMPTY},
			{SPAWN, YELLOW_PLANKTON, INTERROGATION_POINT},
			{EMPTY, WRECK, EMPTY}};

uint8_t board2[BOARD_SIZE][BOARD_SIZE] = {
			{EMPTY, WRECK, EMPTY},
			{SPAWN, GREEN_PLANKTON, WRECK},
			{EMPTY, INTERROGATION_POINT, EMPTY}};

uint8_t board3[BOARD_SIZE][BOARD_SIZE] = {
			{EMPTY, RED_PLANKTON, EMPTY},
			{WRECK, SPAWN, WRECK},
			{EMPTY, INTERROGATION_POINT, EMPTY}};



void printCase(uint8_t value){
	switch(value)
	{
		case EMPTY : printf("|E|");
				 break;
		case SPAWN : printf("|S|");
					 break;
		case WRECK : printf("|W|");
					 break;
		case GREEN_PLANKTON : printf("|G|");
							  break;
		case RED_PLANKTON : printf("|R|");
							break;
		case YELLOW_PLANKTON : printf("|Y|");
							   break;
		case BLUE_PLANKTON : printf("|B|");
							 break;
		case INTERROGATION_POINT : printf("|I|");
								   break;
	}
}

void sendCase(uint8_t value,char * board){
  switch(value)
    {
    case EMPTY : strcat(board,"|E|");
      break;
    case SPAWN : strcat(board,"|S|");
      break;
    case WRECK : strcat(board,"|W|");
      break;
    case GREEN_PLANKTON : strcat(board,"|G|");
      break;
    case RED_PLANKTON : strcat(board,"|R|");
      break;
    case YELLOW_PLANKTON : strcat(board,"|Y|");;
      break;
    case BLUE_PLANKTON : strcat(board,"|B|");
      break;
    case INTERROGATION_POINT :strcat(board,"|I|");
      break;
    }

}

void board_to_string(void * board[BOARDS_SIZE][BOARDS_SIZE], char * board_str){
  uint8_t i;
  uint8_t j;
  char temp[50];
  strcat(board_str,"\n    ------------------\n");
  strcat(board_str,"    |A||B||C||D||E||F|\n");
  strcat(board_str,"----------------------\n");

  for(i=0; i<BOARD_SIZE; i++)
  {
      sprintf(temp,"|%d| ",i);
      strcat(board_str,temp);
      for(j=0; j<BOARD_SIZE; j++)
				{
				  sendCase(((int8_t *)board[0][0])[3*i+j],board_str);
				}
      for(j=0; j<BOARD_SIZE; j++)
				{
				  sendCase(((int8_t *)board[0][1])[3*i+j],board_str);
				}
      strcat(board_str,"\n----------------------\n");
    }

  for(i=0; i<BOARD_SIZE; i++)
    {
      sprintf(temp,"|%d| ",i+BOARD_SIZE);
      strcat(board_str,temp);

      for(j=0; j<BOARD_SIZE; j++)
				{
				  sendCase(((int8_t *)board[1][0])[3*i+j],board_str);
				}
      for(j=0; j<BOARD_SIZE; j++)
				{
				  sendCase(((int8_t *)board[1][1])[3*i+j],board_str);
				}
      strcat(board_str,"\n----------------------\n");
    }
}

void printBoard(void *board[BOARDS_SIZE][BOARDS_SIZE]){
	uint8_t i;
	uint8_t j;

	printf("\n    ------------------\n");
	printf("    |A||B||C||D||E||F|\n");
	printf("----------------------\n");

	for(i=0; i<BOARD_SIZE; i++)
	{
		printf("|%d| ",i);
		for(j=0; j<BOARD_SIZE; j++)
		{
			printCase(((int8_t *)board[0][0])[3*i+j]);
		}
		for(j=0; j<BOARD_SIZE; j++)
		{
			printCase(((int8_t *)board[0][1])[3*i+j]);
		}
		printf("\n----------------------\n");
	}

	for(i=0; i<BOARD_SIZE; i++)
	{
		printf("|%d| ",i+BOARD_SIZE);
		for(j=0; j<BOARD_SIZE; j++)
		{
			printCase(((int8_t *)board[1][0])[3*i+j]);
		}
		for(j=0; j<BOARD_SIZE; j++)
		{
			printCase(((int8_t *)board[1][1])[3*i+j]);
		}
		printf("\n----------------------\n");
	}
}

void initBoard(void *board[BOARDS_SIZE][BOARDS_SIZE]){
	uint8_t i;
	uint8_t r[4];

	srandom(time(NULL));

	r[0] = rand()%4;
	r[1] = rand()%4;
	while(r[1]==r[0])
		r[1] = rand()%4;
	r[2] = rand()%4;
	while(r[2]==r[0]||r[2]==r[1])
		r[2] = rand()%4;

	r[3] = 6-r[0]-r[1]-r[2];

	for(i=0; i<4; i++)
	{
		switch(r[i])
		{
			case 0: board[i>>1][i&0x01] = board0;
					break;
			case 1: board[i>>1][i&0x01] = board1;
					break;
			case 2: board[i>>1][i&0x01] = board2;
					break;
			case 3: board[i>>1][i&0x01] = board3;
					break;
		}
	}

// #ifdef DEBUG
// 	for(i=0; i<4; i++)
// 	{
// 		printf("i:%d:r:%d:\n",i,r[i]);
// 		printf("i1:%d:i2:%d:i3:%d:i4:%d\n",i%1,i%2, i%3, i%4);
// 	}
// #endif


}

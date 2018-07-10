#ifndef __BOARD_H__
#define __BOARD_H__

/* Board parameters */

#define BOARD_SIZE	3
#define BOARDS_SIZE	2

#define EMPTY				0
#define SPAWN				1
#define WRECK				2
#define GREEN_PLANKTON		3
#define RED_PLANKTON		4
#define YELLOW_PLANKTON		5
#define BLUE_PLANKTON		6
#define INTERROGATION_POINT	7


/* Fishes parameters */

#define	ROSE_LITTLE_FISH	10
#define	ROSE_MIDDLE_FISH	11
#define ROSE_BIG_FISH		12

#define	ORANGE_LITTLE_FISH	13
#define ORANGE_MIDDLE_FISH	14
#define ORANGE_BIG_FISH		15

#define LITTLE_FISH_NUMBER	6
#define MIDDLE_FISH_NUMBER	4
#define BIG_FISH_NUMBER		2


/* Tokens */

#define TOKEN_FISHERMAN	0
#define TOKEN_PLANKTON	1
#define TOKEN_SPAWN		2
#define TOKEN_ROTATE	3

#define TOKEN_TYPES_NUMBER	4
#define TOKEN_NUMBERS_PER_TYPE	2
#define TOKEN_TOTAL (TOKEN_TYPES_NUMBER*TOKEN_NUMBERS_PER_TYPE)


/* Functions */

/* Initialise the board ramdomly */
void initBoard(void *board[BOARDS_SIZE][BOARDS_SIZE]);

/* Print the board */
void printBoard(void *board[BOARDS_SIZE][BOARDS_SIZE]);

#endif

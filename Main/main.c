#include "board.h"

void *board[BOARDS_SIZE][BOARDS_SIZE];

int main(void)
{
	initBoard(board);
	printBoard(board);
	return 0;
}

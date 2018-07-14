// Damien Narbais

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "../Board/board.h"
#include "../Communication/libFishIPC.h"


void *board[BOARDS_SIZE][BOARDS_SIZE];

void handler(int sig){
  if (sig==SIGINT){
    printf("On ne tue pas facilement un poisson!\n");
  }
    if (sig==SIGSEGV){
    printf("Un requin vient de renverser le plateau\n");
    exit(SIGSEGV);
  }
}

int main(void)
{
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");

  if(signal(SIGSEGV, handler) == SIG_ERR)
      printf("\ncan't catch SIGIN\n");

	// void hand(int sig);

	initBoard(board);
	printBoard(board);

  while(1){};
	return 0;
}

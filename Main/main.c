// Damien Narbais

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


#include "../Board/board.h"
#include "../Communication/libFishIPC.h"

#define MAX_JOUEURS 4
#define MAX_PARTIES 2

struct partie {
    int numero;
    int nb_joueur;
};

void *board[BOARDS_SIZE][BOARDS_SIZE];

void handler(int sig){
  if (sig==SIGINT){
    printf("On ne tue pas facilement un poisson!\n");
    fish_ipc_destroy_queue(msgid_cmd);
    fish_ipc_destroy_queue(msgid_ans);
  }
    if (sig==SIGSEGV){
    printf("Un requin vient de renverser le plateau\n");
    exit(SIGSEGV);
  }
}

int main(void)
{
  /***************SIGNAUX*************************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");

  if(signal(SIGSEGV, handler) == SIG_ERR)
      printf("\ncan't catch SIGIN\n");
  /****************************************/

  /*****************PLATEAU DE JEU***********************/
	initBoard(board);
	printBoard(board);
  /****************************************/

  /*****************GESTION DES PARTIES***********************/
  // struct partie parties[MAX_PARTIES];
  int partie_lancee=0;
  // int numero_partie=1;
  /****************************************/


  /*************FILE DE MESSAGE DE RECEPTION DES CMD***************************/
  // fish_ipc_destroy_queue(msgid_cmd);
  // fish_ipc_destroy_queue(msgid_ans);
  fish_ipc_create_queue_cmd();
  fish_ipc_create_queue_ans();
  // Msg_requete message;
  char message[TAILLE_MSG];
  char message_reponse[TAILLE_MSG];
  while(1){
    //lit message d'un joueur
    printf("DANS le while avant le if\n");
    fish_ipc_read(msgid_cmd , message );
    printf("On a recu : %s\n",message);
    if (!strcmp("create game",message)){
      printf("DANS le if\n");

        //renvoie le numero d'une partie
        partie_lancee=partie_lancee+1;
        message_reponse[0] = (char)partie_lancee;
        fish_ipc_send(msgid_ans , message_reponse);
    }
  };
  /****************************************/


	return 0;
}

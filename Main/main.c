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


void *board[BOARDS_SIZE][BOARDS_SIZE];

void handler(int sig){
  if (sig==SIGINT){
    printf("On ne tue pas facilement un poisson!\n");
    fish_ipc_destroy_queue(msgid_cmd);
    // fish_ipc_destroy_queue(msgid_ans);
  }
    if (sig==SIGSEGV){
    printf("Un requin vient de renverser le plateau\n");
    exit(SIGSEGV);
  }
}

void init_tab(int tab[MAX_PARTIES]){
  int i;
  for(i=0;i<MAX_PARTIES;i++){
    tab[i]=0;
  }
}

//ATTENTION il faudra faire =2 a un momement
int incrementation_tab_parties(int tab_parties[MAX_PARTIES]){
  int ajout=1;
  int i=0;
  while( (ajout==1)&&(i<MAX_PARTIES)){
    if(tab_parties[i]==0){
      tab_parties[i]=1;
      ajout=0;
      return i;
    }
    i++;
  }
  return -1;
}

int incrementation_tab_msgid_client(int tab_parties[MAX_JOUEURS], int msgid_client){
  int ajout=1;
  int i=0;
  while( (ajout==1)&&(i<MAX_JOUEURS)){
    if(tab_parties[i]==0){
      tab_parties[i]=msgid_client;
      ajout=0;
      return i;
    }
    i++;
  }
  return -1;
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
  int tab_parties[MAX_PARTIES];
  int tab_msgid_client[MAX_JOUEURS];
  init_tab(tab_parties);
  init_tab(tab_msgid_client);
  int msgid_client=-1;
  int numero_partie=-1;
  /****************************************/


  /*************FILE DE MESSAGE DE RECEPTION DES CMD***************************/
  fish_ipc_create_queue_cmd();
  // Msg_requete message;
  char message[TAILLE_MSG];
  while(1){
    //lit message d'un joueur
    printf("DANS le while avant le if\n");
    /********CHANGER*********/
    // fish_ipc_read(msgid_cmd , message );
    msgid_client=fish_ipc_read_from_client(msgid_cmd, message);
    printf("On a recu : %s\n",message);
    printf("Le msgid_client=%d\n",msgid_client);

    if (!strcmp("create game",message)){
        printf("DANS le if\n");
        //renvoie le numero d'une partie
        numero_partie=incrementation_tab_parties(tab_parties);
        message[0] = (char)numero_partie;
        /********CHANGER*********/
        fish_ipc_send(msgid_client , message);
    }
  };
  /****************************************/


	return 0;
}

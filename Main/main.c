// Damien Narbais

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


#include "../Board/board.h"
#include "../Communication/libFishIPC.h"

#define MAX_JOUEURS 2
#define MAX_PARTIES 2
#define PARTIE_EN_ATTENTE 1
#define PARTIE_COMPLETE 2

/*****************GESTION DES JOUEURS***********************/
struct info_client_partie {
  int msgid_client;
  int partie;
  int num_thread;
};

struct info_client_partie client_partie;

/*****************GESTION DES PARTIES***********************/
int tab_parties[MAX_PARTIES];
int tab_thread[MAX_PARTIES];
int tab_msgid_client[MAX_JOUEURS];
int compteur_fdm=0;

/*****************GESTION DU PLATEAU DE JEU*****************/
void *board[BOARDS_SIZE][BOARDS_SIZE];
/***********************************************************/

void handler(int sig){
  if (sig==SIGINT){
    printf("On ne tue pas facilement un poisson!\n");
    fish_ipc_destroy_queue(msgid_cmd);
    //penser a supprimer les fdm crées
    // int i=0;
    // for(i=0;i<compteur_fdm;i++){
    //   fish_ipc_destroy_queue(TAB?);
    // }
    exit(SIGINT);
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

int incrementation_tab_parties(int tab_parties[MAX_PARTIES], int * msgid_client){
  int i=0;
  while((i<MAX_PARTIES)&&((tab_parties[i]==PARTIE_EN_ATTENTE)||(tab_parties[i]==PARTIE_COMPLETE))){
    i++;
  }
  if(i==MAX_PARTIES){
    // printf("++++++ON EST LA +++++++\n");
    fish_ipc_send(*msgid_client,"Toutes les emplacements de parties sont pris\n");
    return -1;
  }
  else {
    tab_parties[i]=PARTIE_EN_ATTENTE;
    return i;
  }
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

void * thread_game(void * arg) {
  struct info_client_partie client_partie_thread  = * (struct info_client_partie *) arg;

  // printf("La partie commence\n");
  char message[TAILLE_MSG];
  message[0]=(char)client_partie_thread.partie;
  fish_ipc_send(client_partie_thread.msgid_client,"La partie commence\n");
  fish_ipc_send(client_partie_thread.msgid_client,message);
  pthread_exit(NULL);
}

void launch_thread_game(int * msgid_client){
  printf("On est dans la fonction du lancement du thread\n");

  int num_partie=incrementation_tab_parties(tab_parties, msgid_client);
  printf("Numéro de la partie lancée : %d\n",num_partie);

  if (num_partie!=-1){
    client_partie.msgid_client=*msgid_client;
    client_partie.partie=num_partie;

    static int c=1;
    pthread_t thread;
    client_partie.num_thread=c;
    c++;

    pthread_create(&thread,NULL,thread_game,&client_partie);
    // pthread_create(&thread,NULL,thread_game,NULL);
    pthread_detach(thread);
  }
  else {
    //On ne peut plus créer de nouvelles parties
    fish_ipc_send(*msgid_client,"Les parties sont complètes\n");
    // printf("++++++ON EST LA +++++++\n");
  }
}

void launch_nb_game_en_attente(int * msgid_client){
  printf("On est dans la fonction pour récup nb parties imcompletes\n");

  int i=0;
  int cpt=0;
  char message[TAILLE_MSG];
  char id_partie_attente[MAX_PARTIES];
  sprintf(message,"%s"," Parties en attente d'un joueur ");
  for(i=0;i<MAX_PARTIES;i++){
    if(tab_parties[i]==PARTIE_EN_ATTENTE){
      cpt++;
      sprintf(id_partie_attente,"%d",i);
      strcat(message,":\t");
      strcat(message,id_partie_attente);
    }
  }
  fish_ipc_send(*msgid_client,message);
}

int main(void)
{
  /**********************SIGNAUX*************************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");

  if(signal(SIGSEGV, handler) == SIG_ERR)
      printf("\ncan't catch SIGIN\n");

  /*****************INITIALISATION***********************/
	initBoard(board);
	printBoard(board);

  init_tab(tab_parties);
  init_tab(tab_thread);
  init_tab(tab_msgid_client);

  int msgid_client=-1;
  // int numero_partie=-1;

  char message[TAILLE_MSG];

  fish_ipc_create_queue_cmd();
  /******************************************************/

  /*************FILE DE MESSAGE DE RECEPTION DES CMD***************************/
  while(1){
    //lit message d'un joueur
    msgid_client=fish_ipc_read_from_client(msgid_cmd, message);
    printf("On a recu : %s\n",message);
    printf("Le msgid_client=%d\n",msgid_client);

    if (!strcmp("create game",message)){
      launch_thread_game(&msgid_client);
    }
    else if (!strcmp("get open games",message)){
      printf("Le client veut voir la liste des parties incomplètes\n");
      launch_nb_game_en_attente(&msgid_client);
    }
    printf("Fin du while\n\n");
  };
  /****************************************/


	return 0;
}

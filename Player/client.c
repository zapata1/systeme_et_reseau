// Damien Narbais

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>

#include "../Communication/libFishIPC.h"

int msgid_answer_server;

void handler(int sig){
  if (sig==SIGINT){
    printf("On ne tue pas facilement un poisson!\n");
    fish_ipc_destroy_queue(msgid_answer_server);
    exit(SIGINT);
  }
}

/**************CHOIX DES ACTIONS*************************/
int choix_action_joueur(int msgid_cmd, int etape){
  // etape = 0 --> Initialisation : le joueur vient d'arriver
  // etape = 1 --> Le joueur a choisi : create game
  // etape = 2 --> Le joueur a choisi : get open games
  // etape = 3 --> Le joueur a choisi : join game X
  unsigned char c;
  int i=0;
  char message[TAILLE_MSG];
  memset(message, 0, sizeof(message));
  fflush(stdin);

  printf("****************************************\n");
  printf("*** Entrez la commande que vous souhaitez : ***\n");
  printf("*** create game \t\t\t***\n");
  printf("*** get open games \t\t\t***\n");
  if (etape==2){
    printf("*** join game X \t\t\t***\n");
  }
  printf("****************************************\n");

  while( (c=getchar())!=10 ){
    message[i]=c;
    i++;
  }

  printf("On envoie : %s\n",message);
  printf("Avec comme msgid : %d\n",msgid_answer_server);
  sprintf(message,"%s;%d",message,msgid_answer_server);
  printf("on va envoyer au final : %s\n\n",message);
  fish_ipc_send(msgid_cmd, message);

  if (strcmp("create game",message)){
    return 2;
  }
  else if (strcmp("get open games",message)){
    return 1;
  }
  else if (strcmp("join game X",message)){
    return 3;
  }
  else {
    printf("Nous n'avons pas compris votre commande\n\n");
    return choix_action_joueur(msgid_cmd, etape);
  }
}
/*******************************************************/

int main(void) {
  /***************SIGNAUX*************************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");
  /***********************************************/

  /**********************INIT VARIABLE********************/
    int msgid_cmd;
    int etape=0;
    char message[TAILLE_MSG];
    msgid_cmd=fish_ipc_retrieve_queue_cmd_id();
    msgid_answer_server=fish_ipc_create_queue_answer();
    /****************************************************/

    etape=choix_action_joueur(msgid_cmd, etape);
    printf("etape = %d\n",etape);
    while(1){
      printf("on attend la réponse du serveur\n");
      /********lit sur la nouvelle file créée par le client*********/
      fish_ipc_read(msgid_answer_server, message);
      // int numero_partie=(int)message[0];
      // printf("Retour du serveur : on joue la partie %d\n",numero_partie);
      // fish_ipc_read(msgid_answer_server, message);
      printf("On a recu : %s\n\n\n",message);
      etape=choix_action_joueur(msgid_cmd, etape);
    }
    return 0;
}

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

#include "../Board/board.h"
#include "../Communication/libFishIPC.h"

int msgid_answer_server;
int msgid_thread;

void handler(int sig){
  if (sig==SIGINT){
    // #ifdef DEBUG
    printf("Suppression de la file de message\n");
    // #endif
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
  // etape = 10 --> Initialisation : le joueur vient d'arriver dans une partie
  // etape >= 10  --> Le joueur peut choisir : get map
  unsigned char c;
  int i=0;
  char message[TAILLE_MSG];
  memset(message, 0, sizeof(message));
  fflush(stdin);

  printf("***********************************************\n");
  printf("*** Entrez la commande que vous souhaitez : ***\n");
  if (etape!=1 && etape<10){
    printf("*** create game \t\t\t***\n");
    printf("*** get open games \t\t\t***\n");
  }
  if (etape==2 && etape<10){
    printf("*** join game X \t\t\t***\n");
  }
  if (etape>=10){
    printf("*** get map \t\t\t\t***\n");
  }
  printf("***********************************************\n");

  while( (c=getchar())!=10 ){
    message[i]=c;
    i++;
  }

  #ifdef DEBUG
  printf("On va envoyer la commande : %s\n",message);
  printf("Avec comme msgid : %d\n",msgid_answer_server);
  sprintf(message,"%s;%d",message,msgid_answer_server);
  printf("On envoie au serveur : %s\n\n",message);
  #endif
  fish_ipc_send(msgid_cmd, message);

  if (strncmp("create game",message,11)==0){
    return 1;
  }
  else if (strncmp("get open games",message,11)==0){
    return 2;
  }
  else if (strncmp("join game ",message,10)==0){
    return 3;
  }
  else if (strncmp("get map",message,7)==0){
    return 10;
  }
  else {
    printf("Nous n'avons pas compris votre commande\n\n");
    return choix_action_joueur(msgid_cmd, etape);
  }
}
/*******************************************************/

/**************LE JOUEUR EST DANS UNE PARTIE*************************/
void partie_en_cours(int * msgid_thread){
    #ifdef DEBUG
    printf("Vous êtes bien dans une partie\n");
    #endif
    int etape=10; //TODO dépend de si créé ou rejoint ?
    char message[TAILLE_MSG];
    //dire si c'est joueur 1 ou 2
    //tant que joueur a au moins 2 poissons et n'en a pas mangé 5
    //faire les actions
    while(1){
      etape=choix_action_joueur(*msgid_thread, etape);
      switch(etape) {
        case 10 :
         printf("Vous voulez voir le plateau de jeu\n" );   //le serveur doit nous renvoyer le plateau de jeu
         fish_ipc_read(msgid_answer_server, message);
         printf("On a recu : \n\n%s\n",message);
         break;

         case 11 :
          printf("Vous voulez vous déplacer\n" );   //le serveur doit nous renvoyer le plateau de jeu
          // fish_ipc_read(msgid_answer_server, message);
          // printBoard((void *)message);
          break;

      default :
         printf("ERROR commande non comprise\n" );
      }
    }
}
/********************************************************************/


int main(void) {
  /***************SIGNAUX*************************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");
  /***********************************************/

  /******************* INITIALISATION ********************/
    int msgid_cmd;
    int etape=0;
    char message[TAILLE_MSG];
    msgid_cmd=fish_ipc_retrieve_queue_cmd_id();
    msgid_answer_server=fish_ipc_create_queue_answer();
    /****************************************************/

    etape=choix_action_joueur(msgid_cmd, etape);
    #ifdef DEBUG
    printf("Etape = %d\n",etape);
    #endif
    while(1){
      switch(etape) {
        case 1 :
         printf("Vous voulez créer un jeu\n" );       //le serveur doit nous renvoyer le num de la partieet l'id de la fdm du thread
         fish_ipc_read(msgid_answer_server, message); //1er message d'information
         #ifdef DEBUG
         printf("On a recu : %s\n",message);
         #endif
         fish_ipc_read(msgid_answer_server, message); //recuperation du numéro de la partie
         int numero_partie=(int)message[0];
         printf("Retour du serveur : on joue la partie %d\n",numero_partie);
         fish_ipc_read(msgid_answer_server, message); //2eme message d'information pour la fdm du thread
         #ifdef DEBUG
         printf("On a recu : %s\n",message);
         #endif
         fish_ipc_read(msgid_answer_server, message); //recuperation de m'id de la fdm de la partie
         msgid_thread=atoi(message);
         printf("Le numéro du thread est : %d\n\n\n",msgid_thread);
         partie_en_cours(&msgid_thread); //on va pouvoir jouer
         break;

        case 2 :
          printf("Vous voulez la liste des parties\n" );  //le serveur doit nous renvoyer la liste des paties disponibles
          fish_ipc_read(msgid_answer_server, message);    //on doit lui renvoyer la partie que l'on veut jouer
          printf("On a recu : %s\n\n\n",message);
          break;

        case 3 :
         printf("Vous voulez rejoindre une partie\n" );   //le serveur doit nous renvoyer le num du thread
         fish_ipc_read(msgid_answer_server, message); //1er message d'information
         #ifdef DEBUG
         printf("On a recu : %s\n",message);
         #endif
         fish_ipc_read(msgid_answer_server, message); //2eme message d'information pour la fdm du thread
         msgid_thread=atoi(message);
         printf("Le numéro du thread est : %d\n\n\n",msgid_thread);

         // il faut annoncer au thread de la gestion de la parie qu'on a bien rejoint la partie
         sprintf(message,"Je suis le deuxieme joueur");
         sprintf(message,";%d",msgid_answer_server);
         printf("On envoie au serveur : %s\n\n",message);
         fish_ipc_send(msgid_thread, message);
         partie_en_cours(&msgid_thread); //on va pouvoir jouer
         break;

      default :
         printf("ERROR commande non comprise\n" );
      }
      etape=choix_action_joueur(msgid_cmd, etape);
    }
    return 0;
}

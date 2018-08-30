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
#include "../Actions/libActions.h"


#define ERREUR -1
// etape = 0 --> Initialisation : le joueur vient d'arriver
// etape = 1 --> Le joueur a choisi : create game
// etape = 2 --> Le joueur a choisi : get open games
// etape = 3 --> Le joueur a choisi : join game X
// etape = 10 --> Initialisation : le joueur vient d'arriver dans une partie
// etape >= 10  --> Le joueur peut choisir : get map
#define ARRIVEE_JOUEUR 0
#define CREATION_PARTIE 1
#define GET_PARTIES_ATTENTE 2
#define JOIN_GAME 3
#define DEBUT_PARTIE 10



int msgid_client;
int msgid_thread;

void handler(int sig){
  if (sig==SIGINT){
    #ifdef DEBUG
    printf("[Client %d]: Suppression de la file de message et deconnection du serveur\n",msgid_client);
    #endif
    char message[TAILLE_MSG]="deconnection";
    sprintf(message,"%s;%d",message,msgid_client);
    fish_ipc_destroy_queue(msgid_client);
    fish_ipc_send(msgid_cmd, message);
    exit(SIGINT);
  }
}

void affichage_menu(int * etape){
  printf("***************************************************\n");
  printf("*** Entrez la commande que vous souhaitez : \t***\n");
  if (*etape!=CREATION_PARTIE && *etape<DEBUT_PARTIE){
    printf("*** create game \t\t\t\t***\n");
    printf("*** get open games \t\t\t\t***\n");
  }
  if (*etape==GET_PARTIES_ATTENTE && *etape<DEBUT_PARTIE){
    printf("*** join game X \t\t\t\t***\n");
  }
  if (*etape>=DEBUT_PARTIE){
    printf("*** get map \t\t\t\t\t***\n");
  }
  printf("***************************************************\n");
}

int erreur_serveur(char * message){
  if(strncmp("ERREUR",message,6)==0){
    printf("Le serveur a renvoyé une erreur : %s",message);
    return ERREUR;
  }
  return 1;
}

int choix_action_joueur(int msgid_cmd, int etape){
  unsigned char c;
  int i=0;
  char message[TAILLE_MSG];
  memset(message, 0, sizeof(message));
  fflush(stdin);

  affichage_menu(&etape);

  //récupération de la demande du client
  while( (c=getchar())!=10 ){
    message[i]=c;
    i++;
  }

  #ifdef DEBUG
  printf("[Client %d]: On va envoyer la commande : %s\n",msgid_client,message);
  #endif
  sprintf(message,"%s;%d",message,msgid_client);
  #ifdef DEBUG
  printf("[Client %d]: Avec comme msgid : %d\n",msgid_client,msgid_client);
  printf("[Client %d]: On envoie au serveur : %s\n\n",msgid_client,message);
  #endif


  if ( (strncmp("create game",message,11)==0)  && (etape==ARRIVEE_JOUEUR || etape==GET_PARTIES_ATTENTE) ){
    fish_ipc_send(msgid_cmd, message);
    return CREATION_PARTIE;
  }
  else if ( (strncmp("get open games",message,11)==0) && (etape==ARRIVEE_JOUEUR || etape==GET_PARTIES_ATTENTE) ){
    fish_ipc_send(msgid_cmd, message);
    return GET_PARTIES_ATTENTE;
  }
  else if ( (strncmp("join game ",message,10)==0) && (etape==GET_PARTIES_ATTENTE) ){
    fish_ipc_send(msgid_cmd, message);
    return JOIN_GAME;
  }
  else if ( (strncmp("get map",message,7)==0) && (etape>=DEBUT_PARTIE) ){
    fish_ipc_send(msgid_cmd, message);
    return DEBUT_PARTIE;
  }
  else {
    printf("[Client %d]: Nous n'avons pas compris votre commande\n\n",msgid_client);
    return choix_action_joueur(msgid_cmd, etape);
  }
}

void partie_en_cours(int * msgid_thread){
    #ifdef DEBUG
    printf("[Client %d]: Vous êtes bien dans une partie\n",msgid_client);
    #endif
    int etape=DEBUT_PARTIE; //TODO dépend de si créé ou rejoint ?
    char message[TAILLE_MSG];
    //dire si c'est joueur 1 ou 2
    //tant que joueur a au moins 2 poissons et n'en a pas mangé 5
    //faire les actions
    while(1){
      etape=choix_action_joueur(*msgid_thread, etape);
      switch(etape) {
        case 10 :
         printf("[Client %d]: Vous voulez voir le plateau de jeu\n",msgid_client );   //le serveur doit nous renvoyer le plateau de jeu
         fish_ipc_read(msgid_client, message);
         printf("[Client %d]: On a recu : \n\n%s\n",msgid_client,message);
         break;

         case 11 :
          printf("[Client %d]: Vous voulez vous déplacer\n",msgid_client);   //le serveur doit nous renvoyer le plateau de jeu
          // fish_ipc_read(msgid_client, message);
          // printBoard((void *)message);
          break;

      default :
         printf("[Client %d]: ERROR commande non comprise\n",msgid_client );
      }
    }
}

void creation_partie(int * etape){
  char message[TAILLE_MSG];

  printf("[Client %d]: Vous voulez créer un jeu\n",msgid_client ); //le serveur doit nous renvoyer le num de la partie et l'id de la fdm du thread
  fish_ipc_read(msgid_client, message); //1er message d'information
  #ifdef DEBUG
  printf("[Client %d]: On a recu : %s\n",msgid_client,message);
  #endif
  if(erreur_serveur(message)!=ERREUR){
    fish_ipc_read(msgid_client, message); //recuperation du numéro de la partie
    if(erreur_serveur(message)!=ERREUR){
      int numero_partie=(int)message[0];
      printf("[Client %d]: Retour du serveur : on joue la partie %d\n",msgid_client,numero_partie);
      fish_ipc_read(msgid_client, message); //2eme message d'information pour la fdm du thread
      #ifdef DEBUG
      printf("[Client %d]: On a recu : %s\n",msgid_client,message);
      #endif
      if(erreur_serveur(message)!=ERREUR){
        fish_ipc_read(msgid_client, message); //recuperation de l'id de la fdm de la partie
        msgid_thread=atoi(message);
        printf("[Client %d]: Le numéro du thread est : %d\n\n\n",msgid_client,msgid_thread);
        partie_en_cours(&msgid_thread); //on va pouvoir jouer
      }
      else {
        *etape=0;
      }
    }
    else {
      *etape=0;
    }
  }
  else {
    *etape=0;
  }
}

void get_parties_attente(int * etape){
  char message[TAILLE_MSG];

  printf("[Client %d]: Vous voulez la liste des parties\n",msgid_client );  //le serveur doit nous renvoyer la liste des paties disponibles
  fish_ipc_read(msgid_client, message);
  if(erreur_serveur(message)!=ERREUR){
    printf("[Client %d]: On a recu : %s\n\n\n",msgid_client,message);
  }
  else {
    *etape=0;
  }
}

void join_game(int * etape){
  char message[TAILLE_MSG];

  printf("[Client %d]: Vous voulez rejoindre la partie demandée\n",msgid_client );   //le serveur doit nous renvoyer le num du thread
  fish_ipc_read(msgid_client, message); //1er message d'information
  #ifdef DEBUG
  printf("[Client %d]: On a recu : %s\n",msgid_client,message);
  #endif
  if(erreur_serveur(message)!=ERREUR){
    fish_ipc_read(msgid_client, message); //2eme message d'information pour la fdm du thread
    msgid_thread=atoi(message);
    #ifdef DEBUG
    printf("[Client %d]: Le numéro du thread est : %d\n\n\n",msgid_client,msgid_thread);
    #endif

    // il faut annoncer au thread de la gestion de la parie qu'on a bien rejoint la partie
    sprintf(message,"Je suis le deuxieme joueur");
    sprintf(message,";%d",msgid_client);
    #ifdef DEBUG
    printf("[Client %d]: On envoie au thread de la gestion de partie : %s\n\n",msgid_client,message);
    #endif
    fish_ipc_send(msgid_thread, message);
    partie_en_cours(&msgid_thread); //on va pouvoir jouer
  }
  else {
    *etape=0;
  }
}

int main(void) {
  /****************** SIGNAUX **********************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");
  /*************************************************/

  /******************* INITIALISATION ********************/
    int msgid_cmd;
    int etape=ARRIVEE_JOUEUR;

    msgid_cmd=fish_ipc_retrieve_queue_cmd_id();
    msgid_client=fish_ipc_create_queue_answer();
    /****************************************************/

    etape=choix_action_joueur(msgid_cmd, etape);
    #ifdef DEBUG
    printf("[Client %d]: Etape = %d\n",msgid_client,etape);
    #endif
    while(1){
      switch(etape) {
        case CREATION_PARTIE :
          creation_partie(&etape);
          break;

        case GET_PARTIES_ATTENTE :
          get_parties_attente(&etape);
          break;

        case JOIN_GAME :
          join_game(&etape);
          break;

      default :
         printf("[Client %d]: ERREUR commande non comprise\n",msgid_client );
      }
      etape=choix_action_joueur(msgid_cmd, etape);
    }
    return 0;
}

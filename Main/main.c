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
#define PARTIE_EN_ATTENTE 1
#define PARTIE_COMPLETE 2

/*****************GESTION DES JOUEURS***********************/
// struct info_clients_partie {
//   int msgid_client_1;
//   int msgid_client_2; //pas besoin
//   int partie;
//   int num_thread; //pas besoin
// };

struct info_client_partie {
  int msgid_client;
  int partie;
};

struct info_client_partie client_partie;

/*****************GESTION DES PARTIES***********************/
int tab_parties[MAX_PARTIES];
int tab_thread[MAX_PARTIES];
int tab_msgid_client[MAX_JOUEURS]; //TODO besoin ?

/*****************GESTION DU PLATEAU DE JEU*****************/
void *board[BOARDS_SIZE][BOARDS_SIZE];

/***********************************************************/

void handler(int sig){
  if (sig==SIGINT){
    printf("On ne tue pas facilement un poisson!\n");
    fish_ipc_destroy_queue(msgid_cmd);
    //penser a supprimer les fdm créées par les threads
    int i=0;
    for(i=0;i<MAX_PARTIES;i++){
      if(tab_thread[i]!=0){
        fish_ipc_destroy_queue(tab_thread[i]);
      }
    }
    exit(SIGINT);
  }
    if (sig==SIGSEGV){
    printf("Un requin vient de renverser le plateau\n");
    exit(SIGSEGV);
  }
}

void init_tab(int tab[MAX_JOUEURS]){
  int i;
  for(i=0;i<MAX_JOUEURS;i++){
    tab[i]=0;
  }
}

int incrementation_tab_parties(int tab_parties[MAX_PARTIES], int * msgid_client){
  int i=0;
  while((i<MAX_PARTIES)&&((tab_parties[i]==PARTIE_EN_ATTENTE)||(tab_parties[i]==PARTIE_COMPLETE))){
    i++;
  }
  if(i==MAX_PARTIES){
    fish_ipc_send(*msgid_client,"Toutes les emplacements de parties sont pris\n");
    return -1;
  }
  else {
    tab_parties[i]=PARTIE_EN_ATTENTE;
    return i;
  }
}

void * thread_game(void * arg) {
  // #ifdef DEBUG
  printf("On est dans le thread de la partie créée\n");
  // #endif
  void * board_thread[BOARDS_SIZE][BOARDS_SIZE];
  initBoard(board_thread);
	printBoard(board_thread);
  struct info_client_partie client_partie_thread  = * (struct info_client_partie *) arg;

  int partie=client_partie_thread.partie;
  int msgid_client_1=client_partie_thread.msgid_client;
  int msgid_client_2=0;
  int msgid_thread=0;

  printf("C'est la partie : %d\n", partie);
  char message[TAILLE_MSG];
  message[0]=(char)partie;
  // #ifdef DEBUG
  printf("On indique au client que la partie est créée et le numéro associé à cette partie\n");
  // #endif
  fish_ipc_send(msgid_client_1,"La partie est créée\n");
  fish_ipc_send(msgid_client_1,message);

  // #ifdef DEBUG
  printf("On lui envoie la nouvelle file de message créé pour la partie\n");
  // #endif
  msgid_thread=fish_ipc_create_queue_answer(); //creation de la nouvelle file de message //TODO
  tab_thread[partie]=msgid_thread; //on stocke le numero de le file de message dans le tableau global

  printf("L'id de la fdm pour le thread est : %d\n",msgid_thread);

  fish_ipc_send(msgid_client_1,"La nouvelle file de message a le numero : \n");
  sprintf(message,"%d",msgid_thread);
  fish_ipc_send(msgid_client_1,message);

  //Ici, le client connait la file de message du thread et peut donc communiquer l'autre jouer
  //On va maintenant attendre que le deuxieme client se connecte
  //TODO une nouvelle fonction à faire pour séparer code

  while(tab_parties[partie]!=PARTIE_COMPLETE){
    //On attend un nouveau message du client
    //Il peut venir des deux clients
    //On stocke donc l'id de sa fdm si c'est le client 2
    //Le client 1 peut demander le plateau du jeu en attendant qu'un autre joueur se connecte
    msgid_client_2=fish_ipc_read_from_client(msgid_thread, message);
    if(!strcmp(message,"get map")){
      fish_ipc_send(msgid_client_1,(char *)board_thread);
      printf("thread %d : [board_thread sent !] \n",partie);
    }
    bzero(message,TAILLE_MSG);
  }

  //Ici, les deux clients sont arrivés et communiquent avec le thread de gestion de partie
  //La partie va pouvoir commencer
  printf("Les deux joueurs sont arrivés\n\n");
  printf("*************************\n");
  printf("msgid J1 = %d\n",msgid_client_1);
  printf("msgid J2 = %d\n",msgid_client_2);
  printf("num fdm thread = %d\n",msgid_thread);
  printf("partie jouée = %d\n",partie);
  printf("*************************\n\n");

  fish_ipc_send(msgid_client_1,"La partie peut commencer, tous les joueurs sont la\n"); //TODO recevoir ces messages
  fish_ipc_send(msgid_client_2,"La partie peut commencer, tous les joueurs sont la\n");
  pthread_exit(NULL);
}

void launch_thread_game(int * msgid_client){
  // #ifdef DEBUG
  printf("On est dans la fonction du lancement du thread\n");
  // #endif

  int num_partie=incrementation_tab_parties(tab_parties, msgid_client);
  // #ifdef DEBUG
  printf("Numéro de la partie lancée : %d\n",num_partie);
  // #endif

  if (num_partie!=-1){
    client_partie.msgid_client=*msgid_client;
    // client_partie.msgid_client_2=0;
    client_partie.partie=num_partie;

    // static int c=1;
    pthread_t thread;
    // client_partie.num_thread=c;
    // c++;
    // // client_partie.num_thread=0;

    pthread_create(&thread,NULL,thread_game,&client_partie);
    pthread_detach(thread);
  }
  else {
    //On ne peut plus créer de nouvelles parties
    fish_ipc_send(*msgid_client,"Les parties sont complètes\n");
  }
}

void launch_nb_game_en_attente(int * msgid_client){
  // #ifdef DEBUG
  printf("On est dans la fonction pour récupérer l'id des parties imcompletes\n");
  // #endif

  int i=0;
  int cpt=0;
  char message[TAILLE_MSG];
  char id_partie_attente[MAX_PARTIES];
  sprintf(message,"%s"," Parties en attente d'un joueur ");
  for(i=0;i<MAX_PARTIES;i++){
    if(tab_parties[i]==PARTIE_EN_ATTENTE){
      cpt++;
      sprintf(id_partie_attente,"%d",i);
      strcat(message," : ");
      strcat(message,id_partie_attente);
    }
  }
  fish_ipc_send(*msgid_client,message);
}

void rejoindre_partie(char message[TAILLE_MSG], int * msgid_client){

  char * temp;
  int numero_partie=-1;
  int msgid_thread=-1;
  temp=strtok(message," ");
  temp=strtok(NULL," ");
  temp=strtok(NULL," ");
  printf("Le client veur rejoindre la partie : %s\n",temp);

  if(temp!=NULL){
    numero_partie=atoi(temp);
    printf("Il veut rejoindre la partie %d\n",numero_partie);

    if((numero_partie>MAX_PARTIES)||(numero_partie<0)){
      fish_ipc_send(*msgid_client,"La partie n'existe pas\n");
    }

    else if(tab_parties[numero_partie]!=1){
      fish_ipc_send(*msgid_client,"La partie demandée est déjà complète ou inexistante\n");
    }

    else if(tab_parties[numero_partie]==1){
      msgid_thread=tab_thread[numero_partie];
      sprintf(message,"%d",msgid_thread);
      // #ifdef DEBUG
      printf("L'id de la fdm pour le thread est : %d\n",msgid_thread);
      // #endif
      fish_ipc_send(*msgid_client,"L'id de la fdm pour le thread est :\n");
      fish_ipc_send(*msgid_client,message);
      tab_parties[numero_partie]=PARTIE_COMPLETE;
    }

    else{
      fish_ipc_send(*msgid_client,"ERROR\n");
    }
  }
  else{
    fish_ipc_send(*msgid_client,"ERROR : Nous n'avons pas compris, veuillez réessayer\n");
  }
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
	// printBoard(board);

  init_tab(tab_parties);
  init_tab(tab_thread);
  init_tab(tab_msgid_client); //TODO besoin ?

  int msgid_client=-1;
  int choix_client=-1;

  char message[TAILLE_MSG];

  fish_ipc_create_queue_cmd();
  /******************************************************/

  /*************FILE DE MESSAGE DE RECEPTION DES CMD***************************/
  while(1){
    //lit message d'un joueur
    msgid_client=fish_ipc_read_from_client(msgid_cmd, message);
    // #ifdef DEBUG
    printf("On a recu : %s\n",message);
    printf("Le msgid_client=%d\n",msgid_client);
    // #endif

    if (!strcmp("create game",message)){
      choix_client=1;
    }
    else if (!strcmp("get open games",message)){
      printf("Le client veut voir la liste des parties incomplètes\n");
      choix_client=2;
    }
    else if (strncmp("join game ",message,10)==0){
      choix_client=3;
    }

    switch(choix_client) {
      case 1 :
        launch_thread_game(&msgid_client);
        break;

      case 2 :
        launch_nb_game_en_attente(&msgid_client);
        break;

      case 3 :
        printf("Il veut rejoindre une partie\n" );
        rejoindre_partie(message, &msgid_client);
       break;

      default :
        printf("ERROR commande non comprise\n" );
    }
    // #ifdef DEBUG
    printf("Que veut faire le client ?\n\n" );
    // #endif
  };
  /****************************************/


	return 0;
}

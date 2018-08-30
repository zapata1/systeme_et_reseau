// Damien Narbais

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


#include "../Board/board.h"
#include "../Communication/libFishIPC.h"
#include "../Actions/libActions.h"

#define MAX_JOUEURS 10
#define MAX_PARTIES 1

#define PARTIE_EN_ATTENTE 1
#define PARTIE_COMPLETE 2

#define ERREUR -1

/*****************GESTION DES JOUEURS***********************/
struct info_client_partie {
  int msgid_client;
  int partie;
};
struct info_client_partie client_partie;

/*****************GESTION DES PARTIES***********************/
int tab_parties[MAX_PARTIES];
int tab_thread[MAX_PARTIES];
int tab_id_joueurs[MAX_JOUEURS];

/***********************************************************/

void supprimer_fdm(void){
  fish_ipc_destroy_queue(msgid_cmd);
  //Suppression des fdm créées par les threads
  int i=0;
  for(i=0;i<MAX_PARTIES;i++){
    if(tab_thread[i]!=0){
      fish_ipc_destroy_queue(tab_thread[i]);
    }
  }
}

void handler(int sig){
  if (sig==SIGINT){
    printf("[Serveur]: On ne tue pas facilement un poisson!\n");
    supprimer_fdm();
    exit(SIGINT);
  }
    if (sig==SIGSEGV){
    printf("[Serveur]: Un requin vient de renverser le plateau\n");
    supprimer_fdm();
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
    #ifdef DEBUG
    printf("[Serveur]: **SATURATION** : Tous les emplacements de parties sont pris\n\n\n");
    #endif
    fish_ipc_send(*msgid_client,"ERREUR : **SATURATION** : Toutes les emplacements de parties sont pris\n");
    return -1;
  }
  else {
    tab_parties[i]=PARTIE_EN_ATTENTE;
    return i;
  }
}

int incrementation_nb_joueurs(int * msgid_client){
  int i = 0;
  int existe = 0;
  //on regarde si le joueur n'est pas déjà connecté
  for(i=0;i<MAX_JOUEURS;i++){
    if(tab_id_joueurs[i]==*msgid_client){
      existe = 1;
    }
  }
  //s'il n'est pas connecté, on lui attribue une place s'il en reste une
  if(existe==0){
    int placement = 0;
    i=0;
    while(i<MAX_JOUEURS && placement==0){
      if(tab_id_joueurs[i]==0){
        tab_id_joueurs[i]=*msgid_client;
        placement=1;
      }
      i++;
    }
    if(placement==0){
      printf("[Serveur]: Plus de place pour un nouveau joueur %d\n\n\n",*msgid_client);
      fish_ipc_send(*msgid_client,"ERREUR : Désolé, il n y a plus de place pour un nouveau joueur\n");
      return ERREUR;
    }
  }
  return 0;
}

void decrementation_nb_joueurs(int * msgid_client){
  int i = 0;
  int existe = 0;
  //on regarde si le joueur est connecté
  for(i=0;i<MAX_JOUEURS;i++){
    if(tab_id_joueurs[i]==*msgid_client){
      existe = 1;
    }
  }
  //s'il est connecté, on libère sa place
  if(existe==1){
    int placement = 0;
    i=0;
    while(i<MAX_JOUEURS && placement==0){
      if(tab_id_joueurs[i]==*msgid_client){
        tab_id_joueurs[i]=0;
        placement=1;
      }
      i++;
    }
    #ifdef DEBUG
    int verif=0;
    for(i=0;i<MAX_JOUEURS;i++){
      if(tab_id_joueurs[i]==*msgid_client){
        verif=ERREUR;
      }
    }
    if(verif==ERREUR){
      printf("[Serveur]: Le joueur %d ne s'est pas bien déconnecté\n\n\n",*msgid_client);
    }
    else{
      printf("[Serveur]: Le joueur %d s'est bien déconnecté\n\n\n",*msgid_client);
    }
    #endif
  }
  else {
    printf("[Serveur]: Le client n'était pas enregistré\n\n\n");
  }
}

//TODO faire la fonction

// int attente_joueur_2(int * partie, int * msgid_client_1, int * msgid_thread, void * board_thread[BOARDS_SIZE][BOARDS_SIZE]){
//   int msgid_client_2;
//   char message[TAILLE_MSG];
//   printf("------------\n");
//   printf("Partie = %d",*partie);
//   printf("msgid_client_1 = %d",*msgid_client_1);
//   printf("msgid_thread = %d",*msgid_thread);
//   printf("------------\n");
//   while(tab_parties[*partie]!=PARTIE_COMPLETE){
//     //On attend un nouveau message du client
//     //Il peut venir des deux clients
//     //On stocke donc l'id de sa fdm si c'est le client 2
//     //Le client 1 peut demander le plateau du jeu en attendant qu'un autre joueur se connecte
//     msgid_client_2=fish_ipc_read_from_client(*msgid_thread, message);
//     if( (!strcmp(message,"get map")) && (msgid_client_2==*msgid_client_1) ){
//       printf("++++++\n");
//       // board_to_string(board_thread,message);
//       fish_ipc_send(*msgid_client_1,(char *)message);
//       printf("thread de la partie %d : plateau envoyé \n",*partie);
//       bzero(message,TAILLE_MSG);
//     }
//   }
//   return msgid_client_2;
// }

void * thread_game(void * arg) {
  void * board_thread[BOARDS_SIZE][BOARDS_SIZE];
  initBoard(board_thread);
	// printBoard(board_thread);
  struct info_client_partie client_partie_thread  = * (struct info_client_partie *) arg;

  int partie=client_partie_thread.partie;
  int msgid_client_1=client_partie_thread.msgid_client;
  int msgid_client_2=0;
  int msgid_thread=0;

  #ifdef DEBUG
  printf("[Partie %d]: On est dans le thread de la partie créée\n",partie);
  printf("[Partie %d]: C'est la partie : %d\n", partie, partie);
  #endif
  char message[TAILLE_MSG];
  message[0]=(char)partie;
  #ifdef DEBUG
  printf("[Partie %d]: On indique au client que la partie est créée et le numéro associé à cette partie\n",partie);
  #endif
  fish_ipc_send(msgid_client_1,"La partie est créée\n");
  fish_ipc_send(msgid_client_1,message);

  #ifdef DEBUG
  printf("[Partie %d]: On lui envoie la nouvelle file de message créé pour la partie\n",partie);
  #endif
  msgid_thread=fish_ipc_create_queue_answer();
  tab_thread[partie]=msgid_thread; //on stocke le numero de le file de message dans le tableau global

  #ifdef DEBUG
  printf("[Partie %d]: L'id de la fdm pour le thread est : %d\n",partie,msgid_thread);
  #endif
  fish_ipc_send(msgid_client_1,"La nouvelle file de message a le numero : \n");
  sprintf(message,"%d",msgid_thread);
  fish_ipc_send(msgid_client_1,message);

  //Ici, le client 1 connait la file de message du thread et peut donc communiquer avec le thread
  //On va maintenant attendre que le deuxieme client se connecte

  // msgid_client_2 = attente_joueur_2(&partie, &msgid_client_1, &msgid_thread, board_thread[BOARDS_SIZE][BOARDS_SIZE]);

  while(tab_parties[partie]!=PARTIE_COMPLETE){
    //On attend un nouveau message du client
    //Il peut venir des deux clients
    //On stocke donc l'id de sa fdm si c'est le client 2
    //Le client 1 peut demander le plateau du jeu en attendant qu'un autre joueur se connecte
    msgid_client_2=fish_ipc_read_from_client(msgid_thread, message);
    if( (!strcmp(message,"get map")) && (msgid_client_2==msgid_client_1) ){
      board_to_string(board_thread,message);
      fish_ipc_send(msgid_client_1,(char *)message);
      printf("[Partie %d]: thread de la partie %d : plateau envoyé \n",partie,partie);
      bzero(message,TAILLE_MSG);
    }
  }

  //Ici, les deux clients sont arrivés et communiquent avec le thread de gestion de partie
  //La partie va pouvoir commencer

  #ifdef DEBUG
  printf("[Partie %d]: Les deux joueurs sont arrivés. Voici le récapitulatif : \n\n",partie);
  printf("*************************\n");
  printf("[Partie %d]: msgid J1 = %d\n",partie,msgid_client_1);
  printf("[Partie %d]: msgid J2 = %d\n",partie,msgid_client_2);
  printf("[Partie %d]: num fdm thread = %d\n",partie,msgid_thread);
  printf("[Partie %d]: partie jouée = %d\n",partie,partie);
  printf("*************************\n\n");
  #endif

  fish_ipc_send(msgid_client_1,"La partie peut commencer, tous les joueurs sont la\n"); //TODO recevoir ces messages
  fish_ipc_send(msgid_client_2,"La partie peut commencer, tous les joueurs sont la\n");
  pthread_exit(NULL);
}

void launch_thread_game(int * msgid_client){
  #ifdef DEBUG
  printf("[Serveur]: On est dans la fonction du lancement du thread\n");
  #endif

  int num_partie=incrementation_tab_parties(tab_parties, msgid_client);
  #ifdef DEBUG
  printf("[Serveur]: Numéro de la partie lancée : %d\n",num_partie);
  #endif

  if (num_partie!=ERREUR){
    client_partie.msgid_client=*msgid_client;
    client_partie.partie=num_partie;

    pthread_t thread;
    pthread_create(&thread,NULL,thread_game,&client_partie);
    pthread_detach(thread);
  }
  else {
    //On ne peut plus créer de nouvelles parties
    #ifdef DEBUG
    printf("[Serveur]: Les parties sont complètes\n\n\n");
    #endif
  }
}

void launch_nb_game_en_attente(int * msgid_client){
  #ifdef DEBUG
  printf("[Serveur]: On est dans la fonction pour récupérer l'id des parties imcompletes\n");
  #endif

  int i=0;
  char message[TAILLE_MSG];
  char id_partie_attente[MAX_PARTIES];
  sprintf(message,"%s"," Parties en attente d'un joueur ");
  for(i=0;i<MAX_PARTIES;i++){
    if(tab_parties[i]==PARTIE_EN_ATTENTE){
      #ifdef DEBUG
      printf("[Serveur]: Partie %d en attente d'un joueur\n",i);
      #endif
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

  //récupération du numéro de la partie que le client veut rejoindre
  temp=strtok(message," ");
  temp=strtok(NULL," ");
  temp=strtok(NULL," ");

  #ifdef DEBUG
  printf("[Serveur]: Le client veur rejoindre la partie : %s\n",temp);
  #endif

  if(temp!=NULL){
    numero_partie=atoi(temp);

    if((numero_partie>=MAX_PARTIES)||(numero_partie<0)){
      #ifdef DEBUG
      printf("[Serveur]: La partie demandée n'existe pas\n\n\n");
      #endif
      fish_ipc_send(*msgid_client,"ERREUR : La partie n'existe pas\n");
    }

    else if(tab_parties[numero_partie]!=1){
      #ifdef DEBUG
      printf("[Serveur]: La partie demandée est déjà complète ou inexistante\n\n\n");
      #endif
      fish_ipc_send(*msgid_client,"ERREUR : La partie demandée est déjà complète ou inexistante\n");
    }

    else if(tab_parties[numero_partie]==1){
      msgid_thread=tab_thread[numero_partie];
      sprintf(message,"%d",msgid_thread);
      #ifdef DEBUG
      printf("[Serveur]: L'id de la fdm pour le thread est : %d\n",msgid_thread);
      #endif
      fish_ipc_send(*msgid_client,"L'id de la fdm pour le thread est :\n");
      fish_ipc_send(*msgid_client,message);
      tab_parties[numero_partie]=PARTIE_COMPLETE;
    }

    else{
      fish_ipc_send(*msgid_client,"ERREUR\n");
    }
  }
  else{
    fish_ipc_send(*msgid_client,"ERREUR : Nous n'avons pas compris, veuillez réessayer\n");
  }
}

int main(void){
  /**********************SIGNAUX*************************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");

  if(signal(SIGSEGV, handler) == SIG_ERR)
      printf("\ncan't catch SIGSEGV\n");

  /*****************INITIALISATION***********************/
  init_tab(tab_parties);
  init_tab(tab_thread);
  init_tab(tab_id_joueurs);

  int msgid_client=-1;
  int choix_client=-1;

  char message[TAILLE_MSG];

  fish_ipc_create_queue_cmd();
  /******************************************************/

  /*************FILE DE MESSAGE DE RECEPTION DES CMD***************************/
  printf("[Serveur]: On peut accueillir %d joueurs sur %d parties\n\n",MAX_JOUEURS,MAX_PARTIES);
  while(1){
    #ifdef DEBUG
    printf("--- Que veut faire le client ? ---\n\n" );
    #endif
    //lit message d'un joueur et récupère l'id de sa fdm
    msgid_client=fish_ipc_read_from_client(msgid_cmd, message);
    #ifdef DEBUG
    printf("[Serveur]: On a recu : %s\n",message);
    printf("[Serveur]: Le msgid_client=%d\n",msgid_client);
    #endif

    //interprete le message
    if (!strcmp("create game",message)){
      printf("[Serveur]: Le client %d veut créer une partie\n",msgid_client);
      choix_client=1;
    }
    else if (!strcmp("get open games",message)){
      printf("[Serveur]: Le client %d veut voir la liste des parties incomplètes\n",msgid_client);
      choix_client=2;
    }
    else if (strncmp("join game ",message,10)==0){
      printf("[Serveur]: Le client %d veut rejoindre une partie\n",msgid_client);
      choix_client=3;
    }
    else if (!strcmp("deconnection",message)){
      printf("[Serveur]: Le client : %d veut se deconnecter\n",msgid_client);
      choix_client=4;
    }

    switch(choix_client) {
      case 1 :
        if (incrementation_nb_joueurs(&msgid_client)!=ERREUR){
          launch_thread_game(&msgid_client);
        }
        break;

      case 2 :
        launch_nb_game_en_attente(&msgid_client);
        break;

      case 3 :
          if (incrementation_nb_joueurs(&msgid_client)!=ERREUR){
            rejoindre_partie(message, &msgid_client);
          }
       break;

       case 4 :
         decrementation_nb_joueurs(&msgid_client);
       break;

      default :
        printf("[Serveur]: ERREUR commande non comprise\n\n\n");
        fish_ipc_send(msgid_client,"ERREUR commande non comprise\n");
    }
  };
  /****************************************/


	return 0;
}

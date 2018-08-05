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
  }
}

int main(void) {
  /***************SIGNAUX*************************/
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("\ncan't catch SIGIN\n");
  /****************************************/

  /****************INIT VARIABLE************************/
    int msgid_cmd;
    char message[TAILLE_MSG];
    msgid_cmd=fish_ipc_retrieve_queue_cmd_id();
    msgid_answer_server=fish_ipc_create_queue_answer();

    /****************************************/

    /**************CREATION DU MESSAGE*************************/
    strcpy(message, "create game");
    printf("On envoie : %s\n",message);
    printf("Avec comme msgid : %d\n",msgid_answer_server);
    sprintf(message,"%s;%d",message,msgid_answer_server);
    printf("on va envoyer au final : %s\n",message);
    /****************************************/

    fish_ipc_send(msgid_cmd, message);

    while(1){
      printf("on attend la réponse du serveur\n");
      /********lit sur la nouvelle file créée par le client*********/
      fish_ipc_read(msgid_answer_server, message);
      // int numero_partie=(int)message[0];
      // printf("Retour du serveur : on joue la partie %d\n",numero_partie);
      // fish_ipc_read(msgid_answer_server, message);
      printf("On a recu : %s\n",message);
    }
    return 0;
}

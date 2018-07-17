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
    int msgid_cmd;
    char message[TAILLE_MSG];

    msgid_cmd=fish_ipc_retrieve_queue_cmd_id();
    /********CHANGER*********/
    msgid_answer_server=fish_ipc_create_queue_answer();
    strcpy(message, "create game");
    printf("On envoie : %s\n",message);
    printf("Avec comme msgid : %d\n",msgid_answer_server);
    // fish_ipc_send_cmd(msgid_answer_server, message);
    sprintf(message,"%s;%d",message,msgid_answer_server);
    printf("on va envoyer au final : %s\n",message);
    // strcpy(message.texte, mess);
    fish_ipc_send(msgid_cmd, message);
    // msgsnd(msgid_cmd, message, TAILLE_MSG,0);
    // if(msgsnd(msgid_cmd, &message, TAILLE_MSG,0)<0){
    //   perror("msgsnd");
    //   exit(1);
    // };

    while(1){
      printf("on est dans le while\n");
      /********CHANGER*********/
      fish_ipc_read(msgid_answer_server, message);
      int numero_partie=(int)message[0];
      printf("Retour du serveur : on joue la partie %d\n",numero_partie);
    }
    return 0;
}

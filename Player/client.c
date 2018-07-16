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



int main(void) {
    int msgid_cmd;
    // int numero_partie;
    char message[TAILLE_MSG];

    msgid_cmd=fish_ipc_retrieve_queue_cmd_id();
    msgid_ans=fish_ipc_retrieve_queue_ans_id();
    strcpy(message, "create game");
    printf("On envoie : %s\n",message);
    // 1 pour client 1
    fish_ipc_send(msgid_cmd, message);

    while(1){
      printf("on est dans le while\n");
      fish_ipc_read(msgid_ans, message);
      int numero_partie=(int)message[0];
      printf("Retour du serveur : %d\n",numero_partie);
    }
    return 0;
}

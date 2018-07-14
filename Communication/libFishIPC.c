// Damien Narbais

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>

#include "libFishIPC.h"


//crée la file de messages de réception des commandes
int fish_ipc_create_queue_cmd(void){
  if ((msgid_cmd = msgget(CLE_MSG_CMD, IPC_CREAT|IPC_EXCL|0666))==-1){
    perror("msgget");
    exit(1);
  }
    return 0;
}

//recupère l'id de la file de message des commandes
// TODO
int fish_ipc_retrieve_queue_cmd_id(void){
  return 0;
}

//crée une file de messages pour les réponses
int fish_ipc_create_queue_answer(void){
  if ((msgid_ans = msgget(CLE_MSG_ANS, IPC_CREAT|IPC_EXCL|0666))==-1){
    perror("msgget");
    exit(1);
  }
    return 0;
}

//détruit une file de message
int fish_ipc_destroy_queue(int msgid)
{
   msgctl(msgid, IPC_RMID, 0);
   return 0;
}

//envoie un message sur la file de commandes
int fish_ipc_send(int numero_type,char * mess){
    Msg_requete message;
    message.type=numero_type;
    strcpy(message.texte, mess);
    msgsnd(msgid_cmd, &message, TAILLE_MSG,0);
    return 0;
}

//lit un message sur une file
int fish_ipc_read(int msgid ,char * (mess)){
    msgrcv(msgid, &mess, TAILLE_MSG,-20,0);
    return (int) * mess;
}

// Damien Narbais

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "libFishIPC.h"


//crée la file de messages de réception des commandes
int fish_ipc_create_queue_cmd(void){
  if ((msgid_cmd = msgget(CLE_MSG_CMD, IPC_CREAT|IPC_EXCL|0666))==-1){
    perror("msgget");
    exit(1);
  }
    return msgid_cmd;
}

int fish_ipc_create_queue_ans(void){
  if ((msgid_ans = msgget(CLE_MSG_ANS, IPC_CREAT|IPC_EXCL|0666))==-1){
    perror("msgget");
    exit(1);
  }
    return msgid_ans;
}

//recupère l'id de la file de message des commandes
int fish_ipc_retrieve_queue_cmd_id(void){
  if ((msgid_cmd = msgget(CLE_MSG_CMD, 0666)) == -1) {
      perror ("msgget");
      exit(1);
  }
  return msgid_cmd;
}

int fish_ipc_retrieve_queue_ans_id(void){
  if ((msgid_ans = msgget(CLE_MSG_ANS, 0666)) == -1) {
      perror ("msgget");
      exit(1);
  }
  return msgid_ans;
}

//crée une file de messages pour les réponses
int fish_ipc_create_queue_answer(void){
  static int cle_msg_ans=1;

  if ((msgid_ans = msgget(cle_msg_ans, IPC_CREAT|IPC_EXCL|0666))==-1){
    perror("msgget");
    exit(1);
  }
  cle_msg_ans ++;
  return msgid_ans;
}

//détruit une file de message
int fish_ipc_destroy_queue(int msgid){
   msgctl(msgid, IPC_RMID, 0);
   return 0;
}

//envoie un message sur la file de commandes
//avec l'ajout en fin du message de l'id de la file crée par le client
int fish_ipc_send_cmd(int msgid_client,char * mess){
    Msg_requete message;
    message.type=TYPE_MESSAGE;

    sprintf(message.texte,"%s;%d",mess,msgid_client);
    // strcpy(message.texte, mess);
    if(msgsnd(msgid_cmd, &message, TAILLE_MSG,0)<0){
      perror("msgsnd");
      exit(1);
    };
    return 0;
}

//envoie un message sur une file choisie
int fish_ipc_send(int msgid,char * mess){
    Msg_requete message;
    message.type=TYPE_MESSAGE;
    sprintf(message.texte,"%s",mess);
    // strcpy(message.texte, mess);

    if(msgsnd(msgid, &message, TAILLE_MSG,0)<0){
      perror("msgsnd");
      exit(1);
    };
    return 0;
}


//lit un message sur une file
int fish_ipc_read(int msgid ,char * mess){
    Msg_requete message;

    if(msgrcv(msgid, &message, TAILLE_MSG,TYPE_MESSAGE,0)<0){
      perror("msgrcv");
      exit(1);
    }

    strcpy(mess, message.texte);
    return 0;
}

//Retourne l'id de la file crée par le msgid_client
//Pour que le serveur réponde au client
int fish_ipc_read_from_client(int msgid ,char * (mess)){
  int msgid_client=-1;
  Msg_requete message;

  if(msgrcv(msgid, &message, TAILLE_MSG,TYPE_MESSAGE,0)<0){
    perror("msgrcv");
    exit(1);
  }

  strcpy(mess, message.texte);

  char * charcarcters;
  charcarcters = strtok(mess, ";");
  while (charcarcters != NULL) {
    // printf("%s\n", charcarcters);
    msgid_client = atoi(charcarcters);
    // printf("Message ID : %d,\n", msgid_client);
    charcarcters = strtok(NULL, " ,");
  }

  return msgid_client;
}

// Damien Narbais
#ifndef __LIBFISHIPC_H__
#define __LIBFISHIPC_H__


#define CLE_MSG_CMD  (key_t)1000
#define TAILLE_MSG  1024
#define TYPE_MESSAGE 2

typedef struct  {
  long type;
  char texte[TAILLE_MSG];
} Msg_requete;


int msgid_cmd;
int msgid_answer;

//crée la le de messages de réception des commandes
int fish_ipc_create_queue_cmd(void);

//recupère l'id de la file de message des commandes
int fish_ipc_retrieve_queue_cmd_id(void);

//recupère l'id de la file de message des reponses du serveur
int fish_ipc_retrieve_queue_ans_id(void);

//crée une le de messages pour les réponses
int fish_ipc_create_queue_answer(void);

//détruit une le de message
int fish_ipc_destroy_queue(int msgid);

//envoie un message sur la le de commandes
int fish_ipc_send_cmd(int numero_type, char * (mess));

//envoie un message sur une file choisie
int fish_ipc_send(int msgid,char * mess);

//lit un message sur une le
int fish_ipc_read(int msgid ,char * (mess));

//Retourne l'id de la file de message créé par l'envoyeur
//Pour que le serveur réponde au client ou puisse connaitre la file de message du client
int fish_ipc_read_from_client(int msgid ,char * (mess));

#endif

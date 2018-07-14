// Damien Narbais

#define CLE_MSG_CMD  (key_t)1000
#define CLE_MSG_ANS  (key_t)1002
#define TAILLE_MSG  80

typedef struct  {
  long type;
  char texte[TAILLE_MSG];
} Msg_requete;


int msgid_cmd;
int msgid_ans;


//crée la le de messages de réception des commandes
int fish_ipc_create_queue_cmd(void);

//recupère l'id de la file de message des commandes
int fish_ipc_retrieve_queue_cmd_id(void);

//crée une le de messages pour les réponses
int fish_ipc_create_queue_answer(void);

//détruit une le de message
int fish_ipc_destroy_queue(int msgid);

//envoie un message sur la le de commandes
int fish_ipc_send(int numero_type, char * (mess));

//lit un message sur une le
int fish_ipc_read(int msgid ,char * (mess));

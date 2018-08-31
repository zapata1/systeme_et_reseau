/** fichier serveur.h **/
#ifndef __SERVER_H__
#define __SERVER_H__

/*********************************************/
/* Structures necessaires pour le serveur    */
/*********************************************/
typedef struct udp_parameter{
    int size;
    unsigned char packet[1];
} udp_parameter;

/** Functions prototypes  **/
char* verif_param(int argc, char *argv[]);
void print_usage();

void hand(int sig);

void *threadTraitementTCP (void* arg);
void traitementTCP (int s);
void *startTCP(void* arg);

#endif

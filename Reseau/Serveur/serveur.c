/** Fichier serveur.c **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../Communication/libcom.h"
#include "../Threads/libthrd.h"
#include "serveur.h"
#include "http.h"

#define   ATTENTE 10
/** Constantes **/
static bool stop = false;
static struct sigaction action;

/* Analyse des arguments */
void print_usage() {
    printf("Usage: -p <port> ou --port <port>\n");
}

char* verif_param(int argc, char *argv[]) {

    int opt= 0;
    static struct option long_options[] = {
        {"port",    required_argument, 0,  'p' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"p:",
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 'p' : return optarg;
             default: print_usage();
                 exit(EXIT_FAILURE);
        }
    }
	if (optind >= argc){
		print_usage();
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/* Signals hand function */
void hand(int sig) {
	if (sig == SIGINT) {
		printf("Stopping servers...\n");
		arret_servers();
		stop = true;
	} else perror("Signal inconnu");
}

/* TCP functions */
void* threadTraitementTCP (void* arg) {
	int s = *((int *)arg);
	#ifdef DEBUG
		fprintf(stderr, "Lancement d'un nouveau thread TCP sur la socket: %d\n", s);
	#endif
	reponseHTTP(s);
	return 0;
}

void traitementTCP (int s) {
	if (lanceThread(threadTraitementTCP, (void *) &s, sizeof(int)) < 0) {
		perror("traitementTCP.lanceThread"); exit(EXIT_FAILURE);
	}
}

void* startTCP(void* arg) {
	serveurTCP((char*)arg, &traitementTCP);
	return 0;
}


/** Procedure principale **/
int main(int argc,char *argv[]){

     // Analyse des arguments
     char *serviceTCP = verif_param(argc,argv);

     /*initialization des signal*/
	   action.sa_handler = hand;
	   sigaction(SIGINT, &action, NULL);

     /*initialization semaphores */
	   init_sem(10, 1);

	  /* initialization server TCP*/
	  lanceThread(startTCP, (void*)serviceTCP, sizeof(serviceTCP));

    /*main seleep tant que les servers functionnent*/
    while (!stop) sleep(1);

    /* Antendre la liberation des threads */
	  int attente = 0;
	  while (getThreads() != 0 && attente < ATTENTE) { sleep(1); attente++; }
	  if (attente == ATTENTE) printf("Servers Stopped\n");

	 /* liberation des semaphores */
	 free_sem();
	return 0;
}

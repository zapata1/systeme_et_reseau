/** Ce fichier contient des fonctions  concernant les sockets  **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "libcom.h"

static bool stop = false;

void arret_servers(){
	stop = true;
}

/**** Fonctions pour le serveur TCP ****/
int initialisationServeur(char *service) {
	#ifdef DEBUG
		fprintf(stderr, "Creation serveur TCP sur le port: %s\n", service);
	#endif
	struct addrinfo precisions, *resultat;
	int status;
	int s;

	/* Construction de la structure adresse */
	memset(&precisions, 0, sizeof precisions);
	precisions.ai_family = AF_UNSPEC;
	precisions.ai_socktype = SOCK_STREAM;
	precisions.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, service, &precisions, &resultat);
	if (status < 0) { perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }

	/* Creation d'une socket */
	s = socket(resultat->ai_family, resultat->ai_socktype, resultat->ai_protocol);
	if (s < 0) { perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }

	/* Options utiles */
	int vrai = 1;
	status = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &vrai, sizeof(vrai));
	if (status < 0) { perror("initialisationServeur.setsockopt (REUSEADDR)"); exit(EXIT_FAILURE); }
	status = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &vrai, sizeof(vrai));
	if (status < 0) { perror("initialisationServeur.setsockopt (NODELAY)"); exit(EXIT_FAILURE); }

	/* Specification de l'adresse de la socket */
	status = bind(s, resultat->ai_addr, resultat->ai_addrlen);
	if (status < 0) { perror("initialisationServeur.bind"); exit(EXIT_FAILURE); }

	/* Liberation de la structure d'informations */
	freeaddrinfo(resultat);

	/* Taille de la queue d'attente */
	status = listen(s, CONNEXIONS);
	if (status < 0) { perror("initialisationServeur.listen"); exit(EXIT_FAILURE); }
	#ifdef DEBUG
		fprintf(stderr, "Serveur TCP créé sur sock: %d\n", s);
	#endif
	return s;
}

int boucleServeur(int s, void (*traitement)(int)) {
	#ifdef DEBUG
		fprintf(stderr, "Boucle serveur TCP...\n");
	#endif
	 int fd;
         struct sockaddr_in client;
         socklen_t clientlen;
	while (!stop) {
		if ((fd = accept(s, (struct sockaddr *)&client, &clientlen)) >= 0) {

		   #ifdef DEBUG
		    fprintf(stderr,"------------------ Request from: %s ------------------\n", inet_ntoa(client.sin_addr));
	           #endif

		   traitement(fd);
                }
		else { perror("boucleServeur.accept");return (-1); }
	}
	#ifdef DEBUG
		fprintf(stderr, "Fermeture du serveur TCP\n");
	#endif
	return 0;
}

void serveurTCP(char *service, void (*traitement)(int)) {
	#ifdef DEBUG
		fprintf(stderr, "Lancement server TCP sur le port: %s\n", service);
	#endif
	int s = initialisationServeur(service);
	#ifdef DEBUG
		fprintf(stderr, "Initilisation serveur TCP socket: %d\n", s);
	#endif
	boucleServeur(s,traitement);
}

/** fichier libcom.h **/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/

#ifndef __LIBCOM_H__
#define __LIBCOM_H__

/**** Constantes ****/
#define UDP_PACKET_SIZE 9
#define CONNEXIONS 50

void arret_servers();

/**** Fonctions pour le serveur TCP ****/
int initialisationServeur(char *service);
int boucleServeur(int s, void (*traitement)(int));
void serveurTCP(char *service, void (*traitement)(int));

#endif

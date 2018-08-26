//Damien Narbais

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/msg.h>
#include <string.h>
#include <assert.h>

#include "actions.h"
#include "../Board/board.h"
#include "../Communication/libFishIPC.h"


/**************************************** SERVEUR *****************************************************/

//Fonction qui permet de placer les 3 poissons du coté du joueur 1 ou 2
//On suppose que le joueur 1 commence toujours et se place en haut du PLATEAU
//Le joueur 2 placera donc ses poissons en bas du PLATEAU
//prend le plateau initial en parametre

char * emplacement_libre(int * joueur, void * board_thread[BOARDS_SIZE][BOARDS_SIZE]){
  if(*joueur==1){
    //Trouver emplacement libre pour J1
  }
  else if (*joueur==2){
    //Trouver emplacement libre pour J2

  }
  else {
    printf("ERROR : il ne peut y avoir que 2 joueurs");
  }
}

void placement_poissons_init(int * numero_joueur, int * msgid_joueur, void * board_thread[BOARDS_SIZE][BOARDS_SIZE]){
  int nb_placement=3;
  printf("Vous pouvez placer vos poissons en :");

}

//Fonction qui gère le déplacement
//prend le tableau de jeu en parametre + n°joueur pour la couleur

//Fonction qui renvoie le Plateau de jeu avec les poissons
//Il prend en compte le plateau de jeu initiale et le tableau ou les poissons se situent


/**************************************** CLIENT *****************************************************/

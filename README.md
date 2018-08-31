# *systeme et réseau :*
# Réalisation de la version informatique du jeu de plateau Little Big Fish
## [GitHub](`https://github.com/zapata1/systeme_et_reseau`) (pour un README plus lisible)
________________________________
## Faire marcher le programme

1.  Dans le répertoire global contenant le README et le Makefile : tapez `make all` ou `make debug` selon que vous voulez afficher les détails de l'exécution ou non.  
2. Ouvrez un terminal (terminal S). Allez dans le répertoire *Main* : lancer le serveur avec la commande :  `./serveur`
3. Ouvrez un autre terminal (terminal J1). Allez dans le répertoire *Player* : lancer le client/joueur avec la commande `./client`
4. Répétez le point n°3 autant de fois que nécessaire pour pouvoir jouer une partie. Vous serez guidé par les instructions que vous pouvez réaliser.
5. *(Partie en cours de réalisation)* Vous pourrez visualiser une partie en cours en ouvrant une page en localhost, en suivant le lien suivant : [Visualisation des parties](`http://localhost:8080/`)  
Pour le moment, le serveur n'est lancé que depuis *Reseau/Serveur*` ./serveur -p 8080`. Et n'affiche pas les infos des parties.
6. Pour quitter les différents terminaux, il suffit de faire la commande : `ctrl + c`. Il faut d'abord arrêter les processus des joueurs avant d'arrêter celui du serveur.
7. Si l'arrêt ne s'est pas bien passé et que certaines files de messages ne sont pas correctement fermées, il vous sera impossible de relancer le serveur ou de créer un client. Il vous faudra avant, lancer la commande `ipcrm -a`.  
Vous pouvez reprendre du point n°2.
________________________________

## Etape de Réalisation

### Etape 0

* Faire les Makefiles : ok

### Etape 1 :

* Tester `SIGINT.`: ok  
* Tester `SIGSEGV.`: ok

### Etape 2

* Architecture : posée
* Fonctions : ok
* Test simplifié : ok
* Test en situation : ok

### Etape 3

* Lancer un thread par partie créée : ok
* Renvoyer "La partie commence" : ok

### Etape 4

* Renvoyer la liste des parties en attente d'un second joueur : ok
* Affecter un jouer à une partie en attente : ok

### Etape 5

* Créer une bibliothèque réseau pour créer un serveur web : *(le code n'est pas de moi)* non terminé
* Elle devra permettre de :
  * choisir une partie en cours
  * visualiser le plateau correspondant avec MAJ automatique

### Etape 6

* Coder le moteur du jeu : non terminé

________________________________

## Structure du programme

### Les acteurs

Le projet est consititué de **3 grandes entités** : le **serveur**, le **client** et le **serveur web**.  
Le **serveur** est celui qui gère les demandes des clients.  
Les **clients** correspondent aux joueurs.  
Le **serveur web** fait référence à l'affichage d'une partie en cours, sur une page web.

### La communication entre les entités

Les entités communiquent grâce à des files de messages, excepté le serveur web qui va communiquer grâce aux sockets.  
Ces derniers, avec les files de messages, sont représentés par des "boites" sur le schéma global.

### Le schéma global

| ![Schéma 1](/Images/Schema_partie_1.png) |
|:--:|
| *Etape 1 : Le premier client se connecte au serveur et veut créer une partie.* |

| ![Schéma 2](/Images/Schema_partie_2.png) |
|:--:|
| *Etape 2 : Le serveur lance le thread qui va gérer la nouvelle partie. Le client 1 "se connecte" grâce à la file de message du thread* |

| ![Schéma 3](/Images/Schema_partie_3.png) |
|:--:|
| *Etape 3 : Le client 2 rejoint la partie.* |

| ![Schéma Final](/Images/Schema_globale.png) |
|:--:|
| *Schéma Global (avec le serveur)* |

### Le principe (voir schéma)

On suppose que le serveur est lancé et qu'il a lancé sa file de message (FDM). Cette FDM va lui permettre de recevoir les demandes des clients/joueurs.  
En parallèle, il lance le serveur web.
1. Un client arrrive et créé lui aussi, sa propre file de message. Il envoie au Serveur 'create game', suivit de l'id de sa FDM. Ainsi, le serveur pourra lui répondre.
2. Le serveur traite la demande. Il lance un thread qui gèrera une partie. Ce dernier aussi créé une FDM pour recevoir les demandes des joueurs.
3. Il envoie ensuite au client, le numero de la partie jouée, et l'id de la FDM du thread.
4. Le client reçoit le message du serveur et 'se connecte' sur la FDM du thread. Il attend le second joueur.
5. Le deuxième client (client2) arrive. Il créé sa FDM. Et comme pour le premier client, il envoie un message suivi de l'id de sa FDM. Sauf qu'il envoie 'get open game'.
6. Le serveur récupère le message et le traite.
7. Il renvoie le numéro des parties en attente d'un second joueur au client2.
8. Le client2 choisit le numéro d'une partie incomplète. Il envoie au serveur : 'join game X' (ou X correspond au numéro de la partie qu'il veut rejoindre).
9. Le serveur récupère la demande.
10. Ce dernier, renvoie au client2 : le numéro de la partie qu'il va jouer et l'id de la FDM du thread.
11. Le client2 peut alors "se connecter" sur la FDM du thread.
12. Le thread est au courant que les deux clients sont bien arrivés. La partie commence.

### Le rôle de chaque dossier

**Actions** : Ce dossier devrait comporter les fonctions nécessaires pour réaliser les étapes d'une partie (gestion des mouvements d'un joueur, prise en compte des éléments du plateau de jeu ...)   
**Board** : Ce dossier contient tout ce qui permet de gérer le plateau de jeu. (création, affichage ...)  
**Communication** : Celui-ci permet de manipuler les files de messages (les créer, les supprimer, envoyer un message, le recevoir ...)  
**Main** : C'est le serveur qui lance les parties, le serveur web et répond aux demandes des clients   
**Player** : C'est le joueur. Il permet d'envoyer et recevoir les informations nécessaire pour jouer une partie.  
**Reseau** : doit permettre de lancer le serveur web pour l'affichage des parties. Pour l'instant le serveur web se lance depuis *Reseau/Serveur* et non depuis le main

________________________________
## Travail restant

1. Afficher les informations sur les parties en cours grâce à un serveur web
  * Je compte réaliser un serveur avec du javascript
  * Le serveur web enverra une requête (grâce aux sockets) au serveur, qui lui renverra les informations demandées
    * Dans un soucis de compatibilité entre les sockets et websockets C et javascript, je voulais finalement utiliser un fichier contenant les informations de la partie. Et cela pour charque partie.
      * Le fichier contiendra les informations du plateau de jeu : le board (qu'on pourra stocker sur la première ligne car invariable), et le tableau des poissons qui sera stocké sur une ligne du fichier (la dernière ligne étant la plus récente).
  * Pour l'instant, le serveur ne se lance pas depuis la main mais depuis *Reseau/Serveur*
2. Finir de coder le moteur du jeu
  * Je continue de développer le répertoire *Actions*
  * Il faut encore définir la gestion des tours
  * Les poissons des joueurs ne seront pas placé sur le board, mais dans un autre tableau
    * Ainsi on garde les informations contenu sous les poissons
    * Pour afficher le plateau, il suffira alors de comparer les deux tableaux et d'afficher les poisssons en priorité

________________________________
## Diffucultés rencontrées

1. Création des Makefiles
2. Gestion du serveur web et communication avec le serveur du jeu
3. Simplification du code pour la gestion du moteur du jeu
________________________________

## Commandes utiles lors du développement
`ipcrm -m` affiche toutes les files de messages  
`ipcrm -a` supprime toutes les files de messages

# *systeme et réseau :*
# Réalisation de la version informatique du jeu de plateau Little Big Fish

### Commandes utiles
`ipcrm -m` affiche toutes les files de messages

`ipcrm -a` supprime toutes les files de messages

## Etape 0

Il faut encore faire le  `make debug`

## Etape 1

Tester `SIGSEGV.`

## Etape 2

* Architecture : posée

* Fonctions : ok

* Test simplifié : ok

* Test en situation : ok

## Etape 3

* Lancer un thread par partie créée : ok
* Renvoyer "La partie commence" : ok

## Etape 4

* Renvoyer la liste des parties en attente d'un second joueur : ok
* Affecter un jouer à une partie en attente : ok

## Etape 5

* Créer une bibliothèque réseau pour créer un serveur web :
* Elle devra permettre de :
  * choisir une partie en cours
  * visualiser le plateau correspondant avec MAJ automatique

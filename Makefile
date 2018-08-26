#
# Makefile du jeu Little Big Fish
#

#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export CLIB = ar cq
# FINAL
# export CFLAGS = -Wall -Werror -pedantic -g -DDEBUG
# Pour debug
export CFLAGS = -Wall -Wextra -pedantic -g -DDEBUG
export FLAGS = -g -DDEBUG

#
# Constantes liees au projet
#

# FONCTIONNE
# DIRS=Board Communication Main Player
# FONCTIONNE moy
DIRS=Board Communication Actions Main Player Reseau/Communication Reseau/Threads Reseau/Serveur

#
# La cible generale
#

all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && make

#
# La cible de debug
#

debug: $(patsubst %, _debug_%, $(DIRS))

$(patsubst %,_debug_%,$(DIRS)):
	cd $(patsubst _debug_%,%,$@) && make debug

#
# La cible de nettoyage
#

clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && make clean

#
# Makefile du jeu Little Big Fish
#

#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -Wall -Werror -pedantic
export FLAGS = -g -DDEBUG

#
# Constantes liees au projet
#

DIRS=Board Main

#
# La cible generale
#

all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && make

#
# La cible de debug
#

# debug: $(patsubst %, _debug_%, $(DIRS))
#
# $(patsubst %,_debug_%,$(DIRS)):
# 	cd $(patsubst _debug_%,%,$@) && make debug

#
# La cible de nettoyage
#

clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && make clean

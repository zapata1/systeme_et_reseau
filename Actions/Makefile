# Damien Narbais
# Makefile du jeu Little Big Fish
# pour générer la bibliotheque libFishIPC
#

# CC = gcc
# CLIB = ar cq
# CFLAGS = -Wall -Werror -pedantic -g -DDEBUG
# FLAGS = -g -DDEBUG

#
# La cible generale
#

all: actions.a

# actions.a: actions.o
# 	rm -f $@
# 	$(CLIB) $@ $+
#
# actions.o: actions.c actions.h

#
# La cible de debug
#

# debug: actions.c
# 	$(CC) $< -o actions $(FLAGS) $(CFLAGS)

debug: actions.a

actions.a: actions.o
	rm -f $@
	$(CLIB) $@ $+

actions.o: actions.c actions.h
	$(CC) -c $^ $(CFLAGS) $(FLAGS)

#
# La cible de nettoyage
#
clean:
	rm -f core *.o actions.a *~

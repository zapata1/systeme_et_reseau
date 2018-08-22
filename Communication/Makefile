# Damien Narbais
# Makefile du jeu Little Big Fish
# pour générer la bibliotheque libFishIPC
#

#
# La cible generale
#

all: libFishIPC.a

libFishIPC.a: libFishIPC.o
	rm -f $@
	$(CLIB) $@ $+

libFishIPC.o: libFishIPC.c libFishIPC.h

#
# La cible de debug
#

debug: libFishIPC.c
	$(CC) $< -o libFishIPC $(FLAGS) $(CFLAGS)

#
# La cible de nettoyage
#
clean:
	rm -f core *.o libFishIPC.a *~

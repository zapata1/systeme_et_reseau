# Damien Narbais
# Makefile du jeu Little Big Fish
#


#
# La cible generale
#

all: libboard.a
	$(CLIB) $@ $+

#
# La cible pour la bibliothèque
#

libboard.a: board.o
	rm -f $@
	$(CLIB) $@ $+

#
# La cible de debug
#

# debug: board.c
# 	$(CC) $< -o board $(FLAGS) $(CFLAGS)

#
# La cible de nettoyage
#
clean:
	rm -f core *.o libboard.a rm *~

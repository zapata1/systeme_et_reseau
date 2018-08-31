# Damien Narbais
# Makefile du jeu Little Big Fish
# pour générer la bibliotheque libthrd
#

SRC = libthrd.c
SRCH = libthrd.h
OBJS = libthrd.o
OBJS_DBG = libthrd_debug.o
LIB_DBG = libthrd_debug.a
LIB = libthrd.a

#
# La cible generale
#

all: $(LIB)

$(LIB): $(OBJS)
	rm -f $@
	$(CLIB) $@ $+

$(OBJS): $(SRC) $(SRCH)

#
# La cible de debug
#

debug: $(LIB_DBG)

$(LIB_DBG): $(OBJS_DBG)
	rm -f $(LIB)
	$(CLIB) $(LIB) $+

$(OBJS_DBG): $(SRC) $(SRCH)
	$(CC) -c $< -o $@ $(CFLAGS) $(FLAGS)

#
# La cible de nettoyage
#

clean:
	rm -f core *.o $(LIB) *~

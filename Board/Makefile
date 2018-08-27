# Damien Narbais
# Makefile du jeu Little Big Fish
# pour générer la bibliotheque libboard
#

SRC = board.c
SRCH = board.h
OBJS = board.o
OBJS_DBG = board_debug.o
LIB_DBG = libboard_debug.a
LIB = libboard.a

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

# Damien Narbais
# Makefile du jeu Little Big Fish
#

#
# Constantes pour la compilation des programmes
#

EXEC = main
SRC = main.c
OBJS = main.o
OBJS_DBG = main_debug.o
LIBS = -lpthread

LOCAL_LIBS = -L ../Board -L ../Communication -L ../Actions
LOCAL_LIBS += -lboard -lFishIPC -lActions


#
# La cible generale
#

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LOCAL_LIBS)

$(OBJS): $(SRC)
	$(CC) -c $^ $(CFLAGS)

#
# La cible de debug
#

debug: $(OBJS_DBG)
	$(CC) -o $(EXEC) $(OBJS_DBG) $(LIBS) $(LOCAL_LIBS) $(CFLAGS)

$(OBJS_DBG): $(SRC)
	$(CC) -c $^ -o $(OBJS_DBG) $(CFLAGS) $(FLAGS)

#
# La cible de nettoyage
#

clean:
	rm -f core *.o $(EXEC) *~

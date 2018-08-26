# Damien Narbais
# Makefile du jeu Little Big Fish
#

#
# Constantes pour la compilation des programmes
#

SRC=client.c
OBJS=client.o
LIBS = -lpthread
# TODO
# LOCAL_LIBS = -L ../Board -L ../Communication -L ../Actions
# LOCAL_LIBS += -lboard -lFishIPC -lactions
LOCAL_LIBS  = -L ../Board -L ../Communication
LOCAL_LIBS += -lboard -lFishIPC
CFLAGS     += -I ../Communication


#
# La cible generale
#

all: client

client: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LOCAL_LIBS)

# client.o: client.c
# 	$(CC) -c $(SRC) $(CFLAGS)


#
# La cible de debug
#

debug: $(OBJS)
	$(CC) -o client $(OBJS) $(LIBS) $(LOCAL_LIBS) $(CFLAGS)

client.o: client.c
	$(CC) -c $(SRC) $(CFLAGS) $(FLAGS)

#
# La cible de nettoyage
#
clean:
	rm -f core *.o client *~

# Damien Narbais
# Makefile du jeu Little Big Fish
#

#
# Constantes pour la compilation des programmes
#

SRC=client.c
OBJS=client.o
LIBS = -lpthread
LOCAL_LIBS  = -L ../Communication
LOCAL_LIBS += -lFishIPC
CFLAGS     += -I ../Communication


#
# La cible generale
#

all: client

client: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LOCAL_LIBS)

client.o: client.c
	$(CC) -c $(SRC) $(CFLAGS)


#
# La cible de debug
#

# debug: $(OBJS)
# 	$(CC) $(FLAGS) -o client $(OBJS) $(LIBS) $(LOCAL_LIBS) $(CFLAGS)



#
# La cible de nettoyage
#
clean:
	rm -f core *.o client *~

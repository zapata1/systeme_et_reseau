#
# Makefile du jeu Little Big Fish
#

#
# Constantes pour la compilation des programmes
#

SRC = main.c
OBJS = main.o
LIBS = -lpthread
LOCAL_LIBS = -L ../Board -L ../Communication
LOCAL_LIBS += -lboard -lFishIPC

#
# La cible generale
#

all: main

main: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LOCAL_LIBS)

main.o: main.c
	$(CC) -c $(SRC) $(CFLAGS)

#
# La cible de debug
#

# debug: $(OBJS)
# 	$(CC) $(FLAGS) -o main $(OBJS) $(LIBS) $(LOCAL_LIBS) $(CFLAGS)

#
# La cible de nettoyage
#

clean:
	rm -f core *.o main *~

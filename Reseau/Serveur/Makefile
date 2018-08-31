#
# Makefile pour generer le serveur
#

EXEC = serveur
EXEC_DBG = serveur_debug
SRC = serveur.c http.c
SRCH = serveur.h http.h

SRC1 = serveur.c
SRC2 =http.c
SRCH1 = serveur.h
SRCH2 =http.h

OBJS = serveur.o http.o
OBJS_DBG = serveur_debug.o http_debug.o
OBJ1_DBG = serveur_debug.o
OBJ2_DBG = http_debug.o

LIBS = -lpthread
LOCAL_LIBS  = -L ../Communication -L ../Threads
LOCAL_LIBS += -lcom -lthrd
CFLAGS     += -I ../Communication -I ../Threads

#
# La cible generale
#
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $^ $(LOCAL_LIBS) $(LIBS)

$(OBJS): $(SRC) $(SRCH)

#
# La cible de debug
#
debug: $(OBJS_DBG)
	$(CC) $(CFLAGS) $(FLAGS) -o $(EXEC) $^ $(LOCAL_LIBS) $(LIBS)

$(OBJ1_DBG): $(SRC1) $(SRCH1)
	$(CC) -c $< -o $@ $(CFLAGS) $(FLAGS)

$(OBJ2_DBG): $(SRC2) $(SRCH2)
	$(CC) -c $< -o $@ $(CFLAGS) $(FLAGS)


#
# La cible de nettoyage
#
clean:
	rm -f core *.o serveur

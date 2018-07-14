// Damien Narbais

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>

#include "../Communication/libFishIPC.h"



int main(void) {
    int msgid;
    int numero_partie;
    char message[TAILLE_MSG];

    if ((msgid = msgget(CLE_MSG_CMD, 0666)) == -1) {
        perror ("msgget");
        exit(1);
    }

    strcpy(message, "create game\n");

    // 1 pour client 1
    fish_ipc_send(1, message);

    numero_partie=fish_ipc_read (msgid , message);
    printf("%d",numero_partie);
    return 0;
}

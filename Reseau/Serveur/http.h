#ifndef __HTTP_H__
#define __HTTP_H__

/*** Constantes ***/
#define WEB_DIR  "./www"
#define MAX_BUFFER 1024

/** Functions **/
void reponseHTTP(int s);
void cerror(FILE *client, char *cause, char *errno,
	    char *shortmsg, char *longmsg);
#endif

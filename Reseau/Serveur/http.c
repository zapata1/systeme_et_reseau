#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <libthrd.h>

#include "http.h"

void reponseHTTP(int s) {

  char buffer[MAX_BUFFER];
  char cmd[MAX_BUFFER];
  char page[MAX_BUFFER];
  char proto[MAX_BUFFER];
  char path[MAX_BUFFER];
  char type[MAX_BUFFER];
  FILE* client = NULL;
  FILE* webpage = NULL;
  char* p;

  #ifdef DEBUG
		printf("create a new http client, on socket %d\n", s);
	#endif

  client = fdopen(s, "r+");
	if (client == NULL) {
		fprintf(stderr, "failed to open socket %d\n", s );
		return ;
	}

	if (fgets(buffer, MAX_BUFFER, client) == NULL) {
		fprintf(stderr, "client connected on socket %d not sent data\n", s );
		return;
	}

	if (sscanf(buffer, "%s %s %s", cmd, page, proto) != 3) {
    cerror(client, proto, "400", "Bad Request",
	     "Your browser sent a request that this server could not understand");
	}

	while (fgets(buffer, MAX_BUFFER, client) != NULL)
		if (strcmp(buffer,"\r\n") == 0) break;

	if (strcmp(cmd, "GET") == 0) {
		if (strcmp(page, "/") == 0)
			sprintf(page, "/index.html");

		struct stat fstat;
		sprintf(path, "%s%s", WEB_DIR, page);
    if (stat(path, &fstat) < 0) {
      page[0] = ' ';
      cerror(client, page, "404", "Not found",
	     "Server SYR couldn't find this file");
    }
    else{
    strcpy(type, "text/html");
		char *end = page + strlen(page);
		if (strcmp(end - 4, ".png") == 0) strcpy(type, "image/png");
		if (strcmp(end - 4, ".jpg") == 0) strcpy(type, "image/jpg");
		if (strcmp(end - 4, ".gif") == 0) strcpy(type, "image/gif");
		fprintf(client, "HTTP/1.1 200\n");
		fprintf(client, "Server: SyR Web Server\n");
		fprintf(client, "Content-type: %s\n", type);
		fprintf(client, "Content-length: %d\n",(int)fstat.st_size);
		fprintf(client, "\r\n");
		fflush(client);

     int fd = open(path, O_RDONLY);
      p = mmap(0, fstat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
      fwrite(p, 1, fstat.st_size, client);
      munmap(p, fstat.st_size);

    if (strcmp(page,"/listeParties.html") == 0) P(VALEURS_INDEX); //TODO a modifier
		else if (strcmp(page,"/partieChoisie.html") == 0) P(GRAPHES_INDEX); //TODO a modifier
    webpage = fopen(path, "r");
        unsigned char byte;
  			byte = fgetc(webpage);
  			while (!feof(webpage))
  			{
  				fputc(byte, client);
  				byte = fgetc(webpage);
  			}
  			fclose(webpage);
  			fprintf(client, "\r\n");
  			fflush(client);
		}
    if (strcmp(page,"/listeParties.html") == 0) V(VALEURS_INDEX); //TODO a modifier
		else if (strcmp(page,"/partieChoisie.html") == 0) V(GRAPHES_INDEX); //TODO a modifier

	} else {
    cerror(client, cmd, "501", "Not Implemented",
	     "Server SYR does not implement this method");
  }

	#ifdef DEBUG
		printf("----------Request sent to client, closing communication-----------\n");
	#endif
  fclose(client);
  close(s);
}

void cerror(FILE *stream, char *cause, char *errno,
	    char *shortmsg, char *longmsg) {
  fprintf(stream, "HTTP/1.1 %s %s\n", errno, shortmsg);
  fprintf(stream, "Content-type: text/html\n");
  fprintf(stream, "\n");
  fprintf(stream, "<html><title>SYR Error</title>");
  fprintf(stream, "<body bgcolor=""ffffff"">\n");
  fprintf(stream, "%s: %s\n", errno, shortmsg);
  fprintf(stream, "<p>%s: %s\n", longmsg, cause);
  fprintf(stream, "<hr><em>SYR Web server</em>\n");
}

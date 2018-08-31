/** Ce fichier contient des fonctions  concernant les threads  **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#include "libthrd.h"

static int nbrThreads = 0;
static int semid;

typedef union semun
{
  int val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
}semun;

void init_sem(int nb, unsigned short val)
{
	semun args;
	unsigned short semivals[nb];

	#ifdef DEBUG
		fprintf(stderr, "Creation de %d semaphores\n", nb);
	#endif

	if((semid = semget(IPC_PRIVATE, nb, IPC_EXCL |0666 | IPC_CREAT)) < 0)
	{
		perror("libthrd.sem_init.semget");
	}

	memset(semivals, val, nb * sizeof(unsigned short));
	args.array = semivals;

 if(semctl (semid, 0, SETALL, args) < 0 )
 {
	 perror("libthrd.sem_init.semctl");
 }
}

void free_sem() {
	#ifdef DEBUG
		fprintf(stderr, "Free all semaphores\n");
	#endif
	if(semctl (semid, 1, IPC_RMID, NULL) < 0){
    perror ("libthrd.sem_free.semctl");
  }
}

void P(int index) {

  struct sembuf p_op;
	p_op.sem_num = index;
	p_op.sem_op = -1; /* Give UP COntrol of track */
	p_op.sem_flg = 0;
  if(semop(semid, &p_op, 1) < 0)
  {
    perror ("libthrd.P");
  }
}

void V(int index) {

  struct sembuf p_op;
	p_op.sem_num = index;
	p_op.sem_op = 1; /* increment semaphore -- take control of track */
	p_op.sem_flg = 0;
  if(semop(semid, &p_op, 1) < 0)
  {
    perror ("libthrd.V");
  }
}


void* thread_function(void *arg) {
	/* Copy of argument */
	thread_parameter *params = arg;
	/* call function with argument */
	params->function(params->argument);
	/* free memory */
	free(params->argument);
	free(params);

	nbrThreads--;
	#ifdef DEBUG
		fprintf(stderr, "Thread terminated, thread running: %d\n", nbrThreads);
	#endif

	pthread_exit(NULL);
}

/*** lance thread ***/
int lanceThread(void *(*func)(void *), void *arg, int size) {

	thread_parameter* params = (thread_parameter*) malloc(sizeof(thread_parameter));
	if(params == NULL) {perror("lanceThread.params.malloc"); return (-1);}
	params->function = func;
	params->argument = malloc(size);
	if(params->argument == NULL) {perror("lanceThread.params.argument.malloc"); return (-2);}

	memcpy(params->argument, arg, (size_t)size);

	pthread_attr_t attr;
	pthread_t thread;

	if(pthread_attr_init (&attr)!=0){
      perror("lanceThread.pthread_attr_init");
	  return (-3);
	}

	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)!=0){
		perror("lanceThread.pthread_attr_setdetachstate");
	    return (-4);
    }
	if(pthread_create(&thread, &attr, &thread_function,params)!=0){
		perror("lanceThread.pthread_create");
		return (-5);
	}
	nbrThreads ++;
	#ifdef DEBUG
		fprintf(stderr, "Thread started, threads running: %d\n", nbrThreads);
	#endif
	return 0;
}

int getThreads() {
	return nbrThreads;
}

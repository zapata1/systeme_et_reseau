#ifndef __LIBTHRD_H__
#define __LIBTHRD_H__

/* semaphores indexes */
#define GROUPES_INDEX 0
#define GRAPHES_INDEX 8
#define VALEURS_INDEX 9

typedef struct thread_parameter {
	void *(*function)(void *);
	void *argument;
} thread_parameter;

/* Threads */
void* thread_function(void *arg);
int lanceThread(void *(*func)(void *), void *arg, int size);
int getThreads();

/* semaphores */
void init_sem(int nb, unsigned short val);
void free_sem();
void P(int index);
void V(int index);

#endif

/*
 * kaonsys_mutex.c
 *
 *  Created on: Sep 17, 2010
 *      Author: buttonfly
 */


#include <pthread.h>
#include "osl.h"
#include "osldbg.h"
#include <string.h>

osl_mutex osl_mutex_create (void)
{
	pthread_mutex_t* lck;
	pthread_mutexattr_t mutexAttr;;
	lck = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	KASSERT(lck);
	memset(lck, 0, sizeof(pthread_mutex_t));
#if 1
	pthread_mutexattr_init(&mutexAttr);

	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_FAST_NP);

	pthread_mutex_init(lck, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
#else
	pthread_mutex_init(lck, NULL);
#endif
	return (osl_mutex) lck;
}

void osl_mutex_lock (osl_mutex mutex)
{
	KASSERT(mutex);
	pthread_mutex_lock((pthread_mutex_t*)mutex);
}

void osl_mutex_unlock (osl_mutex mutex)
{
	KASSERT(mutex);
	pthread_mutex_unlock((pthread_mutex_t*)mutex);
}

void osl_mutex_close (osl_mutex mutex)
{
	KASSERT(mutex);
	pthread_mutex_destroy((pthread_mutex_t*)mutex);
	free(mutex);
}


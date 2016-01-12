/*
 * osl_spin.c
 *
 *  Created on: Sep 2, 2013
 *      Author: buttonfly
 */




/*
 * kaonsys_spin.c
 *
 *  Created on: Sep 17, 2010
 *      Author: buttonfly
 */


#include <pthread.h>
#include "osl.h"
#include "osldbg.h"
#include <string.h>

osl_spin osl_spin_create (void)
{
	pthread_spinlock_t* lck;
	lck = (pthread_spinlock_t*) malloc(sizeof(pthread_spinlock_t));
	memset(lck, 0, sizeof(pthread_spinlock_t));
	pthread_spin_init(lck, 0);
	return (osl_spin) lck;
}

void osl_spin_lock (osl_spin lck)
{
	KASSERT(lck);
	pthread_spin_lock((pthread_spinlock_t*)lck);
}

void osl_spin_unlock (osl_spin lck)
{
	KASSERT(lck);
	pthread_spin_unlock((pthread_spinlock_t*)lck);
}

void osl_spin_close (osl_spin lck)
{
	KASSERT(lck);
	pthread_spin_destroy((pthread_spinlock_t*)lck);
	free(lck);
}


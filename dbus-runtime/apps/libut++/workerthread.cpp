/*
 * workerthread.cpp
 *
 *  Created on: May 31, 2013
 *      Author: buttonfly
 */

#include "include/workerthread.hpp"

#include "runnable.h"

namespace k {

workerthread::workerthread() :
		m_initialized(false), m_uninitialized(false), m_closed(false) {
}

workerthread::~workerthread() {
	uninitialize();
}

void workerthread::initialize(int num) {
	if (!m_initialized) {
		pthread_attr_init(&m_attri);
	    pthread_attr_setscope(&m_attri, PTHREAD_SCOPE_SYSTEM);
//	    pthread_attr_setscope(&attri, PTHREAD_SCOPE_PROCESS);
		pthread_attr_setdetachstate(&m_attri, PTHREAD_CREATE_JOINABLE);

		pthread_mutex_init(&m_lck, NULL);
		pthread_cond_init(&m_cond, NULL);

		pthread_t thread;
		for (int i = 0; i < (int) num; i++) {
			pthread_create(&thread, &m_attri, (void*(*)(void*))workerthread::work, (void*)this);
			m_threads.push_back(thread);
		}
		m_initialized = true;
	}
}

void workerthread::uninitialize(void) {
    if(!m_uninitialized) {
        pthread_mutex_lock(&m_lck);
        m_closed = true;
        pthread_cond_broadcast(&m_cond);
        pthread_mutex_unlock(&m_lck);

        pthread_t thread;
        for(auto  pos = m_threads.begin(); pos != m_threads.end(); pos++) {
            pthread_join(*pos, NULL);
        }

        m_runnables.clear();

        pthread_mutex_destroy(&m_lck);
        pthread_cond_destroy(&m_cond);
		pthread_attr_destroy(&m_attri);

        m_uninitialized = true;
    }
}

void workerthread::invoke(runnable* runnable) {
    pthread_mutex_lock(&m_lck);
    m_runnables.push_back(runnable);
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_lck);
}

runnable* workerthread::get_runnable(void) {
	pthread_mutex_lock(&m_lck);
	for(;;) {
		if(m_runnables.size() > 0)
			break;

		pthread_cond_wait(&m_cond, &m_lck);
		if(m_closed) {
			pthread_mutex_unlock(&m_lck);
			pthread_exit((void*)NULL);
		}
	}

	runnable* o = NULL;
	o = m_runnables.front();
	m_runnables.pop_front();
	pthread_mutex_unlock(&m_lck);
	return o;
}

void* workerthread::work(workerthread* worker) {
	for(;!worker->m_closed;) {
		runnable* runnable = worker->get_runnable();
		if(runnable) runnable->run();
	}
	pthread_exit((void*) NULL);
	return NULL;
}

} /* namespace k */

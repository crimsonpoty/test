/*
 * workerthread.h
 *
 *  Created on: May 31, 2013
 *      Author: buttonfly
 */

#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_

#include "runnable.h"
#include <sys/types.h>
#include <list>
#include <pthread.h>

using namespace std;

namespace k {

class workerthread {
public:
	workerthread();
	virtual ~workerthread();

	/**
	 * @param num the number of thread to create
	 */
	void initialize(int num);
	void uninitialize(void);

	void invoke(runnable* runnable);

private:
	static void* work(workerthread* worker);
	runnable* get_runnable(void);

	list<runnable*> m_runnables;
    list<pthread_t>		m_threads;
    pthread_mutex_t		m_lck;
    pthread_cond_t		m_cond;
	pthread_attr_t m_attri;

    bool		m_initialized;
    bool		m_uninitialized;
    bool		m_closed;
};

} /* namespace k */
#endif /* WORKERTHREAD_H_ */

/*
 * servicemanager.h
 *
 *  Created on: May 31, 2013
 *      Author: buttonfly
 */

#ifndef SERVICEMANAGER_H_
#define SERVICEMANAGER_H_

#include "service.h"
#include <list>
#include <pthread.h>

#include "workerthread.hpp"

using namespace std;

namespace k {

class servicemanager {
public:
	static servicemanager& get_instance() {
		static servicemanager inst;
		return inst;
	}
	void invoke(service* s);
	void remove(service* s);

	service* get_service(const char* id);

private:
	servicemanager();
	virtual ~servicemanager();

	workerthread m_workerthread;
	list<service*> m_services;
	pthread_mutex_t		m_lck;
};

} /* namespace k */
#endif /* SERVICEMANAGER_H_ */

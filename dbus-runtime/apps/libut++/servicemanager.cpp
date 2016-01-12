/*
 * servicemanager.cpp
 *
 *  Created on: May 31, 2013
 *      Author: buttonfly
 */

#include "servicemanager.h"
#include <string.h>

namespace k {

servicemanager::servicemanager() {
	pthread_mutex_init(&m_lck, NULL);
	//TODO : 20!
	m_workerthread.initialize(20);
}

servicemanager::~servicemanager() {
	m_workerthread.uninitialize();
	pthread_mutex_destroy(&m_lck);
	//TODO : pop all services
}

service* servicemanager::get_service(const char* id) {
	pthread_mutex_lock(&m_lck);
	for(list<service*>::iterator pos = m_services.begin(); pos != m_services.end(); pos++) {
		service* o = dynamic_cast<service*>(*pos);
		if(o && (strncmp(o->get_id(), id, strlen(id))==0)) {
			pthread_mutex_unlock(&m_lck);
			return o;
		}
	}
	pthread_mutex_unlock(&m_lck);
}

void servicemanager::remove(service* s) {
	pthread_mutex_lock(&m_lck);
	m_services.remove(s);
	pthread_mutex_unlock(&m_lck);
}

void servicemanager::invoke(service* s) {
	// TODO: each instance should have a unique id.
	pthread_mutex_lock(&m_lck);
	m_workerthread.invoke(s);
	m_services.insert(m_services.end(), s);
	pthread_mutex_unlock(&m_lck);
}

} /* namespace k */

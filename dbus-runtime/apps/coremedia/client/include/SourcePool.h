/*
 * SourcePool.h
 *
 *  Created on: Dec 16, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_SOURCEPOOL_H_
#define APPS_COREMEDIA_SOURCEPOOL_H_

#include "Source.h"
#include "base.h"

namespace k {

class SourcePool {
public:
	static SourcePool &shared() {
		static SourcePool pool;
		return pool;
	}

	auto request(DataSource &dataSource)->Source *;
	void release(Source *dataSource);

private:
	SourcePool();
	virtual ~SourcePool();

private:
	static void signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata);
	static void propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata);
};

} /* namespace k */

#endif /* APPS_COREMEDIA_SOURCEPOOL_H_ */

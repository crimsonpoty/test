/*
 * SinkPool.h
 *
 *  Created on: Dec 17, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_CLIENT_SINKPOOL_H_
#define APPS_COREMEDIA_CLIENT_SINKPOOL_H_

#include "Sink.h"
#include "DataSink.h"

namespace k {

class SinkPool {
public:
	static SinkPool &shared() {
		static SinkPool inst;
		return inst;
	}

	auto request(DataSink &dataSink)->Sink *;
	void release(Sink *dataSink);

private:
	SinkPool();
	virtual ~SinkPool();

private:
	static void signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata);
	static void propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata);
};

} /* namespace k */

#endif /* APPS_COREMEDIA_CLIENT_SINKPOOL_H_ */

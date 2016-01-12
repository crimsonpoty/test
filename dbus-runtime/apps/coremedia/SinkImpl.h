/*
 * SinkImpl.h
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_SINKIMPL_H_
#define APPS_COREMEDIA_SINKIMPL_H_

extern "C" {
#include "gdbus/gdbus-Coremedia-generated.h"
}

namespace k {

class SinkImpl {
public:
	SinkImpl(CoremediaSinkPool *object, GDBusMethodInvocation *invocation, const char *dataSink);
	virtual ~SinkImpl();

	static gboolean on_sink_prepare(CoremediaSink *object, GDBusMethodInvocation *invocation, gpointer user_data);
};

} /* namespace k */

#endif /* APPS_COREMEDIA_SINKIMPL_H_ */

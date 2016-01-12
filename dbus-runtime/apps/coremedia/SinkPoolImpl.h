/*
 * SinkPoolImpl.h
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_SINKPOOLIMPL_H_
#define APPS_COREMEDIA_SINKPOOLIMPL_H_
extern "C" {
#include "gdbus/gdbus-Coremedia-generated.h"
}

namespace k {

class SinkPoolImpl {
public:
	static SinkPoolImpl &shared() {
		static SinkPoolImpl inst;
		return inst;
	}

private:
	SinkPoolImpl();
	virtual ~SinkPoolImpl();

	static gboolean on_request_sink (
		    CoremediaSinkPool *object,
		    GDBusMethodInvocation *invocation,
			const gchar *arg_dataSource,
			gpointer user_data);

	static gboolean on_release_sink (
			CoremediaSinkPool *object,
		    GDBusMethodInvocation *invocation, const gchar *arg_dataSource
			,gpointer user_data);
};

} /* namespace k */

#endif /* APPS_COREMEDIA_SINKPOOLIMPL_H_ */

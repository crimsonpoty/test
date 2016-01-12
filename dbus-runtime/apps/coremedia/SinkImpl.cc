/*
 * SinkImpl.cc
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#include "SinkImpl.h"
extern "C" {
#include "gdbus/gdbus-Coremedia-generated.h"
}
#include "DBusObjectManagerServer.h"
#include <assert.h>
#include "def.h"
#include "osldbg.h"
#include <string>
#include <algorithm>
#include  <functional>

namespace k {

SinkImpl::SinkImpl(CoremediaSinkPool *object, GDBusMethodInvocation *invocation, const char *dataSink) {
	S_;
	m_("%s", dataSink);
	std::size_t hashcode = std::hash<std::string>{}(dataSink);

	std::string path(GDBUS_COREMEDIA_SINK_POOL_OBJECT_PATH);
	path.append("/");
	path.append(std::to_string((long unsigned int)hashcode));
	m_("path: %s", path.c_str());

	auto localObject = coremedia_object_skeleton_new(path.c_str());
	assert(localObject);

	auto skeleton = coremedia_sink_skeleton_new ();
	coremedia_object_skeleton_set_sink(localObject, skeleton);
	g_object_unref(skeleton);

	g_signal_connect(skeleton, "handle-prepare",	G_CALLBACK(on_sink_prepare), this); /* user_data */

	auto server = k::DBusObjectManagerServer::shared().server();
	assert(server);
	g_dbus_object_manager_server_export(server, G_DBUS_OBJECT_SKELETON(localObject));
	g_object_unref(localObject);
}

SinkImpl::~SinkImpl() {
	// TODO Auto-generated destructor stub
}

gboolean SinkImpl::on_sink_prepare (
		CoremediaSink *object,
	    GDBusMethodInvocation *invocation, gpointer user_data) {
	S_;

	coremedia_sink_complete_prepare(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}


} /* namespace k */

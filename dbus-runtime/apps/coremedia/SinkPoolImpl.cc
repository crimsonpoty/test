/*
 * SinkPoolImpl.cc
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#include "SinkPoolImpl.h"
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
#include "SinkImpl.h"

namespace k {

SinkPoolImpl::SinkPoolImpl() {
	auto server = k::DBusObjectManagerServer::shared().server();
	assert(server);

	auto object = coremedia_object_skeleton_new(GDBUS_COREMEDIA_SINK_POOL_OBJECT_PATH);
	assert(object);

	auto skeleton = coremedia_sink_pool_skeleton_new ();
	coremedia_object_skeleton_set_sink_pool(object, skeleton);
	g_object_unref(skeleton);

	g_signal_connect(skeleton, "handle-request",	G_CALLBACK(on_request_sink), this); /* user_data */
	g_signal_connect(skeleton, "handle-release",	G_CALLBACK(on_release_sink), this); /* user_data */

	g_dbus_object_manager_server_export(server, G_DBUS_OBJECT_SKELETON(object));
	g_object_unref(object);
}

SinkPoolImpl::~SinkPoolImpl() {
	// TODO Auto-generated destructor stub
}

gboolean SinkPoolImpl::on_request_sink (
	    CoremediaSinkPool *object,
	    GDBusMethodInvocation *invocation,
		const gchar *arg_dataSource,
		gpointer user_data) {
	S_;

	// TODO:
	new SinkImpl(object, invocation, arg_dataSource);

	coremedia_sink_pool_complete_request(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

gboolean SinkPoolImpl::on_release_sink (
		CoremediaSinkPool *object,
	    GDBusMethodInvocation *invocation,
		const gchar *arg_dataSource,
		gpointer user_data) {
	S_;
	m_("%s", arg_dataSource);
	coremedia_sink_pool_complete_release(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

} /* namespace k */

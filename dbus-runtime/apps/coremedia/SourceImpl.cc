/*
 * SourceImpl.cc
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#include "SourceImpl.h"
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

SourceImpl::SourceImpl(CoremediaSourcePool *object, GDBusMethodInvocation *invocation, const char *dataSource) {
	S_;
	m_("%s", dataSource);

	std::size_t hashcode = std::hash<std::string>{}(dataSource);

	std::string path(GDBUS_COREMEDIA_SOURCE_POOL_OBJECT_PATH);
	path.append("/");
	path.append(std::to_string((long unsigned int)hashcode));
	m_("path: %s", path.c_str());

	auto localObject = coremedia_object_skeleton_new(path.c_str());
	assert(localObject);

	auto skeleton = coremedia_source_skeleton_new ();
	coremedia_object_skeleton_set_source(localObject, skeleton);
	g_object_unref(skeleton);

	g_signal_connect(skeleton, "handle-prepare",	G_CALLBACK(on_source_prepare), this); /* user_data */
	g_signal_connect(skeleton, "handle-start",	G_CALLBACK(on_source_start), this); /* user_data */
	g_signal_connect(skeleton, "handle-stop",	G_CALLBACK(on_source_stop), this); /* user_data */
	g_signal_connect(skeleton, "handle-request-pid-channel",	G_CALLBACK(on_source_request_pid_channel), this); /* user_data */
	g_signal_connect(skeleton, "handle-release-pid-channel",	G_CALLBACK(on_source_release_pid_channel), this); /* user_data */


	g_signal_connect(skeleton, "interface-proxy-properties-changed", G_CALLBACK(propertiesChanged), this); /* user_data */
	g_signal_connect(skeleton, "interface-proxy-signal", G_CALLBACK(signalReceived), this); /* user_data */

	auto server = k::DBusObjectManagerServer::shared().server();
	assert(server);
	g_dbus_object_manager_server_export(server, G_DBUS_OBJECT_SKELETON(localObject));
	g_object_unref(localObject);

}

SourceImpl::~SourceImpl() {
	// TODO Auto-generated destructor stub
}


gboolean SourceImpl::on_source_prepare (
		CoremediaSource *object,
	    GDBusMethodInvocation *invocation, gpointer user_data) {
	S_;

	coremedia_source_complete_prepare(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

gboolean SourceImpl::on_source_start (
		CoremediaSource *object,
	    GDBusMethodInvocation *invocation, gpointer user_data) {
	S_;

	coremedia_source_complete_start(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

gboolean SourceImpl::on_source_stop (
		CoremediaSource *object,
	    GDBusMethodInvocation *invocation, gpointer user_data) {
	S_;

	coremedia_source_complete_stop(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

gboolean SourceImpl::on_source_request_pid_channel (
		CoremediaSource *object,
	    GDBusMethodInvocation *invocation, gpointer user_data) {
	S_;

	// TODO: open a pidchannel handle. or if a pidchannel

	coremedia_source_complete_request_pid_channel(object, invocation, (guint64)0);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

gboolean SourceImpl::on_source_release_pid_channel (
		CoremediaSource *object,
	    GDBusMethodInvocation *invocation, gpointer user_data) {
	S_;

	// TODO: open a pidchannel handle. or if a pidchannel

	coremedia_source_complete_release_pid_channel(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

void SourceImpl::setDataSource(std::string DataSource) {

}

auto SourceImpl::parseDataSource()->int {

	return 0;
}



void SourceImpl::signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata) {
	S_;
	KASSERT(userdata);
	SourceImpl *self = (SourceImpl *) userdata;
	g_print("Signal received on %s:\n", g_dbus_object_get_object_path(G_DBUS_OBJECT((GDBusObjectProxy *)objProxy)));

	GVariantIter iter;
	const gchar *key;
	GVariant *value;

	gchar *parameters_str = g_variant_print ((GVariant *) parameters, TRUE);
	g_print (" *** Received Signal: %s: %s\n",
			  signalName,
			   parameters_str);
	g_free (parameters_str);
}

void SourceImpl::propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata) {
	S_;
	KASSERT(userdata);
	SourceImpl *self = (SourceImpl *) userdata;

	g_print("Properties Changed on %s:\n", g_dbus_object_get_object_path(G_DBUS_OBJECT((GDBusObjectProxy *)objProxy)));

	GVariantIter iter;
	const gchar *key;
	GVariant *value;
	gchar *s;

	g_variant_iter_init(&iter, (GVariant *) changedProperties);
	while (g_variant_iter_next(&iter, "{&sv}", &key, &value)) {
		s = g_variant_print((GVariant *) value, TRUE);
		g_print("  %s -> %s\n", key, s);
		g_variant_unref((GVariant *) value);
		g_free(s);
	}
}



} /* namespace k */

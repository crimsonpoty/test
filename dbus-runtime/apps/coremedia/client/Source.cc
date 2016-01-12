/*
 * Source.cc
 *
 *  Created on: Dec 16, 2015
 *      Author: buttonfly
 */

#include "Source.h"
extern "C" {
#include "gdbus-Coremedia-generated.h"
}
#include "osldbg.h"
#include "../def.h"
#include  <functional>
#include <assert.h>
#include <string>
#include "DBusObjectManager.h"

namespace k {

Source::Source()
{
	S_;
	DBusObjectManager::shared().signal_connect("interface-proxy-properties-changed", G_CALLBACK(propertiesChanged), this);
	DBusObjectManager::shared().signal_connect("interface-proxy-signal", G_CALLBACK(signalReceived), this);
}

Source::~Source() {
	S_;
}

void Source::setDataSource(DataSource &dataSource) {
	S_;
	_dataSource = dataSource;

	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);

	coremedia_source_set_data_source ((CoremediaSource *) interface, _dataSource.baseUri().c_str());
}

const DataSource &Source::dataSource() {
	return _dataSource;
}

auto Source::prepare()->int{
	S_;

	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);

	GError *error = NULL;
	coremedia_source_call_prepare_sync((CoremediaSource *) interface, NULL, &error);
	if(error) {
		g_printerr("Error getting object manager client: %s", error->message);
		g_error_free(error);
	}
	return 0;
}

auto Source::isPrepared()->bool {
	S_;
	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);
	return 0;
}

auto Source::start()->int {
	S_;
	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);

	GError *error = NULL;
	coremedia_source_call_start_sync((CoremediaSource *) interface, NULL, &error);
	if(error) {
		g_printerr("Error getting object manager client: %s", error->message);
		g_error_free(error);	//TODO:
	}
	return 0;
}

auto Source::stop()->int {
	S_;
	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);

	GError *error = NULL;
	coremedia_source_call_stop_sync((CoremediaSource *) interface, NULL, &error);
	if(error) {
		g_printerr("Error getting object manager client: %s", error->message);
		g_error_free(error);	//TODO:
	}

	return 0;
}

auto Source::requestPidChannel(unsigned short pid)->NativeHandle {
	S_;
	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);

	GError *error = NULL;
	guint64 pidChannelHandle;
	coremedia_source_call_request_pid_channel_sync((CoremediaSource *) interface, pid, &pidChannelHandle, NULL, &error);
	if(error) {
		g_printerr("Error getting object manager client: %s", error->message);
		g_error_free(error);	//TODO:
	}
	return (NativeHandle) pidChannelHandle;
}

void Source::releasePidChannel(NativeHandle pidChannelHandle) {
	S_;
	GDBusInterface *interface = (GDBusInterface *)dbusInterface();
	assert(interface);

	GError *error = NULL;
	coremedia_source_call_release_pid_channel_sync((CoremediaSource *) interface,  (guint64) pidChannelHandle, NULL, &error);
	if(error) {
		g_printerr("Error getting object manager client: %s", error->message);
		g_error_free(error);	//TODO:
	}
}


auto Source::dbusInterface()->void * {
	S_;
	std::string uri = _dataSource.baseUri();
	std::size_t hashcode = std::hash<std::string>{}(uri);

	std::string path(GDBUS_COREMEDIA_SOURCE_POOL_OBJECT_PATH);
	path.append("/");
	path.append(std::to_string((long unsigned int)hashcode));
	m_("path: %s", path.c_str());

	DBusObjectManager &dbusObjectManager = DBusObjectManager::shared();
	auto dbusObject = dbusObjectManager.dbusObject(path);
	assert(dbusObject);
	auto interface = dbusObjectManager.dbusInterface(dbusObject, GDBUS_COREMEDIA_SOURCE_INTERFACE_NAME);
	assert(interface);
	return interface;
}

void Source::signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata) {
	S_;
	KASSERT(userdata);
	Source *self = (Source *) userdata;
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

void Source::propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata) {
	S_;
	KASSERT(userdata);
	Source *self = (Source *) userdata;

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

/*
 * SourcePool.cc
 *
 *  Created on: Dec 16, 2015
 *      Author: buttonfly
 */

#include "SourcePool.h"
extern "C" {
#include "gdbus-Coremedia-generated.h"
}
#include "osldbg.h"
#include "../def.h"
#include  <functional>
#include <assert.h>
#include <string>
#include "DBusObjectManager.h"

extern "C" {
#include "gdbus-Coremedia-generated.h"
}
#include "osldbg.h"

namespace k {

SourcePool::SourcePool() {

	S_;
	DBusObjectManager &dbusObjectManager = DBusObjectManager::shared();
	DBusObjectManager::shared().signal_connect("interface-proxy-properties-changed", G_CALLBACK(propertiesChanged), this);
	DBusObjectManager::shared().signal_connect("interface-proxy-signal", G_CALLBACK(signalReceived), this);
}

SourcePool::~SourcePool() {
	S_;
}

auto SourcePool::request(DataSource &dataSource)->Source * {
	S_;
	Source *source = NULL;
	source = new Source;

	std::size_t hashcode = std::hash<std::string>{}(dataSource.baseUri());

	std::string path(GDBUS_COREMEDIA_SOURCE_POOL_OBJECT_PATH);
	path.append("/");
	path.append(std::to_string((long unsigned int)hashcode));
	m_("path: %s", path.c_str());

	DBusObjectManager &dbusObjectManager = DBusObjectManager::shared();
	GDBusObject *dbusObject = dbusObjectManager.dbusObject(path);
	if(dbusObject == NULL) {
		m_("cannot find object!!");
		// TODO: create

		auto interface = dbusObjectManager.dbusInterface(GDBUS_COREMEDIA_SOURCE_POOL_OBJECT_PATH, GDBUS_COREMEDIA_SOURCE_POOL_INTERFACE_NAME);

		assert(interface);
		GError *error = NULL;
		coremedia_source_pool_call_request_sync((CoremediaSourcePool *) interface, dataSource.baseUri().c_str(), NULL, &error);
		if(error) {
			g_printerr("Error getting object manager client: %s", error->message);
			g_error_free(error);
		}
	}
	assert(source);

	m_("%s", dataSource.baseUri().c_str());
	source->setDataSource(dataSource);
	return source;
}

void SourcePool::release(Source *dataSource) {
	S_;
}

void SourcePool::signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata) {
	S_;
	KASSERT(userdata);
	SourcePool *self = (SourcePool *) userdata;
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

void SourcePool::propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata) {
	S_;
	KASSERT(userdata);
	SourcePool *self = (SourcePool *) userdata;

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

/*
 * DBusObjectManager.cc
 *
 *  Created on: Dec 17, 2015
 *      Author: buttonfly
 */

#include "DBusObjectManager.h"
#include "osldbg.h"
#include "../def.h"
#include <assert.h>

namespace k {

DBusObjectManager::DBusObjectManager()
: _manager(NULL)
{

}

DBusObjectManager::~DBusObjectManager() {
}

auto DBusObjectManager::initWith(const char *name, const char *objectPath)->DBusObjectManager & {
	S_;
	if(_manager == NULL) {
		GError *error = NULL;
		_manager = coremedia_object_manager_client_new_for_bus_sync(
				G_BUS_TYPE_SESSION,
				G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
				name,
				objectPath,
				NULL, /* GCancellable */
				&error);
		if (_manager == NULL) {
			g_printerr("Error getting object manager client: %s", error->message);
			g_error_free(error);
	//		goto out;
		}
	}
	return *this;
}

auto DBusObjectManager::signal_connect(const gchar *detailed_signal, GCallback  c_handler, gpointer data)->gulong {
	S_;
	assert(_manager != NULL);
	return g_signal_connect(_manager, detailed_signal, G_CALLBACK(c_handler), data);
}

auto DBusObjectManager::signal_disconnect_by_data(gpointer data)->guint {
	S_;
	assert(_manager != NULL);
	return g_signal_handlers_disconnect_by_data(_manager, data);
}

auto DBusObjectManager::isInitialized()->bool {
	return (_manager != NULL);
}

auto DBusObjectManager::dbusObject(std::string path)->GDBusObject * {
	S_;
	assert(_manager != NULL);
	return g_dbus_object_manager_get_object(_manager, path.c_str());
}

auto DBusObjectManager::dbusInterface(std::string path,  std::string interface)->GDBusInterface * {
	S_;
	assert(_manager != NULL);
	return g_dbus_object_manager_get_interface(_manager, path.c_str(), interface.c_str());
}

auto DBusObjectManager::dbusInterface(GDBusObject *dbusObject, std::string interface)->GDBusInterface * {
	S_;
	std::string path = g_dbus_object_get_object_path(G_DBUS_OBJECT(dbusObject));
	return dbusInterface(path, interface);
}

auto DBusObjectManager::hasDbusObject(std::string path)->bool {
	return (dbusObject(path)!=NULL);
}

} /* namespace k */

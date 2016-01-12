/*
 * DBusObjectManager.h
 *
 *  Created on: Dec 17, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_CLIENT_DBUSOBJECTMANAGER_H_
#define APPS_COREMEDIA_CLIENT_DBUSOBJECTMANAGER_H_

extern "C" {
#include "gdbus-Coremedia-generated.h"
}

#include <string>

namespace k {

class DBusObjectManager {
public:
	static DBusObjectManager &shared() {
		static DBusObjectManager inst;
		return inst;
	}

	auto initWith(const char *name, const char *objectPath)->DBusObjectManager &;
	auto isInitialized()->bool;

	auto native()->GDBusObjectManager * {
		return _manager;
	}

	auto signal_connect(const gchar *detailed_signal, GCallback	  c_handler, gpointer data)->gulong;
	auto signal_disconnect_by_data(gpointer data)->guint;

	auto dbusObject(std::string path)->GDBusObject *;
	auto dbusInterface(std::string path, std::string interface)->GDBusInterface *;
	auto dbusInterface(GDBusObject *dbusObject, std::string interface)->GDBusInterface *;
	auto hasDbusObject(std::string path)->bool;

private:
	DBusObjectManager();
	virtual ~DBusObjectManager();

private:
	GDBusObjectManager *_manager;
};

} /* namespace k */

#endif /* APPS_COREMEDIA_CLIENT_DBUSOBJECTMANAGER_H_ */

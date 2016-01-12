/*
 * DBusObjectManagerServer.cc
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#include "DBusObjectManagerServer.h"
#include "def.h"

namespace k {

DBusObjectManagerServer::DBusObjectManagerServer() {
	_server = g_dbus_object_manager_server_new(GDBUS_COREMEDIA_OBJECT_PATH);
}

DBusObjectManagerServer::~DBusObjectManagerServer() {
	// TODO Auto-generated destructor stub
}

} /* namespace k */

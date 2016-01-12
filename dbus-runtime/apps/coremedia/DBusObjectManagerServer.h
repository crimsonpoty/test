/*
 * DBusObjectManagerServer.h
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_DBUSOBJECTMANAGERSERVER_H_
#define APPS_COREMEDIA_DBUSOBJECTMANAGERSERVER_H_

extern "C" {
#include "gdbus/gdbus-Coremedia-generated.h"
}

#include <stdio.h>


namespace k {

class DBusObjectManagerServer {
public:
	static DBusObjectManagerServer &shared() {
		static DBusObjectManagerServer inst;
		return inst;
	}

	auto server()->GDBusObjectManagerServer * {return _server;}

private:
	DBusObjectManagerServer();
	virtual ~DBusObjectManagerServer();

private:
	GDBusObjectManagerServer *_server = NULL;
};

} /* namespace k */

#endif /* APPS_COREMEDIA_DBUSOBJECTMANAGERSERVER_H_ */

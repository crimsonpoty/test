
/*
 * main.cc
 *
 *  Created on: Nov 23, 2015
 *      Author: buttonfly
 */

extern "C" {
#include "../coremedia/gdbus/gdbus-Coremedia-generated.h"
}
#include "../coremedia/def.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <spawn.h>
#include <sys/wait.h>
#include <string>
#include <algorithm>
#include "osldbg.h"
#include  <functional>
#include <assert.h>
#include <string>

#include "DBusObjectManagerServer.h"
#include "SourcePoolImpl.h"
#include "SinkPoolImpl.h"


// #1
static void on_bus_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data) ;
static void on_name_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data) ;
static void on_name_lost(GDBusConnection *connection, const gchar *name, gpointer user_data);

// #2
#if 0
static std::string absolutePath(const char* file) {
#define  BUFF_SIZE   1024
	char cmd[BUFF_SIZE] = {0};
	char buf[BUFF_SIZE] = {0};
	sprintf(cmd, "/bin/which %s", file);
	printf("input : %s\n", cmd);
	auto p = popen(cmd, "r");
	assert(p);
	while(fgets(buf, BUFF_SIZE, p)) {
		printf("%s", buf);
	}
	pclose(p);

	std::string path = buf;
    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '\n'), '\0');

	return path;
}
#endif

int main(int argc, char* argv[]) {

	auto loop = g_main_loop_new (NULL, FALSE);

	auto id = g_bus_own_name(G_BUS_TYPE_SESSION,
			GDBUS_COREMEDIA_NAME,
			(GBusNameOwnerFlags)(G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
					G_BUS_NAME_OWNER_FLAGS_REPLACE),
					on_bus_acquired,
					on_name_acquired,
					on_name_lost,
					loop, NULL);

	g_main_loop_run(loop);
	g_bus_unown_name(id);
	g_main_loop_unref(loop);

	return 0;
}

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
	g_print("Acquired a message bus connection\n");

	k::SourcePoolImpl::shared();
	k::SinkPoolImpl::shared();

	auto server = k::DBusObjectManagerServer::shared().server();
	assert(server);
	g_dbus_object_manager_server_set_connection(server, connection);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  g_print ("Acquired the name %s\n", name);
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  g_print ("Lost the name %s\n", name);
}


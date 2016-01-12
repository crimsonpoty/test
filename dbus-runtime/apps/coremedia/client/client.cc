
/*
 * main.c
 *
 *  Created on: Sep 12, 2012
 *      Author: buttonfly
 */

extern "C" {
#include "gdbus-Coremedia-generated.h"
}

#include "../def.h"
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "osldbg.h"
#include <string>
#include <iostream>
#include "DBusObjectManager.h"
#include "base.h"
#include <assert.h>

/* ---------------------------------------------------------------------------------------------------- */
#define GDBUS_NAME								GDBUS_COREMEDIA_NAME
#define GDBUS_OBJECT_PATH				GDBUS_COREMEDIA_OBJECT_PATH
#define GDBUS_INTERFACE_NAME		GDBUS_COREMEDIA_INTERFACE_NAME

typedef struct _CustomData {
  GMainLoop *main_loop;  /* GLib's Main Loop */
  GThread *thread;
  GMainContext *main_context;
  GMutex lck;
  GCond cond;
} LocalData;

static LocalData _data;

static void on_notify_name_owner(GObject *object, GParamSpec *pspec, gpointer user_data);
static void on_object_removed(GDBusObjectManager *manager, GDBusObject *object, gpointer user_data);
static void on_object_added(GDBusObjectManager *manager, GDBusObject *object, gpointer user_data);
static void print_objects(GDBusObjectManager *manager);

static gpointer
loop_func (gpointer data) {
	S_;
  GMainLoop *loop = (GMainLoop*) data;
  KASSERT(loop);

  g_mutex_lock (&_data.lck);
  g_cond_wait (&_data.cond, &_data.lck);
  g_mutex_unlock (&_data.lck);

  g_main_loop_run (loop);

  return NULL;
}

// TODO: disconnect

void __attribute__((constructor)) module_init( void ) {
	S_;
	memset(&_data, 0, sizeof(LocalData));

	g_mutex_init(&_data.lck);
	g_cond_init(&_data.cond);
	_data.main_loop = g_main_loop_new(NULL, FALSE);

	_data.thread = g_thread_create ((void*(*)(void*)) loop_func, _data.main_loop, TRUE, NULL);

	GError *error = NULL;

	k::DBusObjectManager &dbusObjectManager = k::DBusObjectManager::shared();
	if(dbusObjectManager.isInitialized()==NO) {
		dbusObjectManager.initWith(GDBUS_NAME, GDBUS_OBJECT_PATH);
	}

	auto manager = dbusObjectManager.native();
	assert(manager);

	auto name_owner = g_dbus_object_manager_client_get_name_owner(
			G_DBUS_OBJECT_MANAGER_CLIENT(manager));
	g_print("name-owner: %s\n", name_owner);
	g_free(name_owner);

	print_objects(manager);

	dbusObjectManager.signal_connect("notify::name-owner", G_CALLBACK(on_notify_name_owner), NULL);
	dbusObjectManager.signal_connect("object-added", G_CALLBACK(on_object_added), NULL);
	dbusObjectManager.signal_connect("object-removed", G_CALLBACK(on_object_removed), NULL);

	g_cond_signal(&_data.cond);
}

static void print_objects(GDBusObjectManager *manager) {
	S_;
	GList *objects;
	GList *l;

	g_print("Object manager at %s\n",
			g_dbus_object_manager_get_object_path(manager));
	objects = g_dbus_object_manager_get_objects(manager);
	for (l = objects; l != NULL; l = l->next) {
		auto object = COREMEDIA_OBJECT(l->data);
		GList *interfaces;
		GList *ll;
		g_print(" - Object at %s\n",
				g_dbus_object_get_object_path(G_DBUS_OBJECT(object)));

		interfaces = g_dbus_object_get_interfaces(G_DBUS_OBJECT(object));
		for (ll = interfaces; ll != NULL; ll = ll->next) {
			GDBusInterface *interface = G_DBUS_INTERFACE(ll->data);
			g_print("   - Interface %s\n",
					g_dbus_interface_get_info(interface)->name);

			/* Note that @interface is really a GDBusProxy instance - and additionally also
			 * an ExampleAnimal or ExampleCat instance - either of these can be used to
			 * invoke methods on the remote object. For example, the generated function
			 *
			 *  void example_animal_call_poke_sync (ExampleAnimal  *proxy,
			 *                                      gboolean        make_sad,
			 *                                      gboolean        make_happy,
			 *                                      GCancellable   *cancellable,
			 *                                      GError        **error);
			 *
			 * can be used to call the Poke() D-Bus method on the .Animal interface.
			 * Additionally, the generated function
			 *
			 *  const gchar *example_animal_get_mood (ExampleAnimal *object);
			 *
			 * can be used to get the value of the :Mood property.
			 */
		}
		g_list_foreach(interfaces, (GFunc) g_object_unref, NULL);
		g_list_free(interfaces);
	}
	g_list_foreach(objects, (GFunc) g_object_unref, NULL);
	g_list_free(objects);
}

static void on_object_added(GDBusObjectManager *manager, GDBusObject *object, gpointer user_data) {
	S_;
	gchar *owner = g_dbus_object_manager_client_get_name_owner(
			G_DBUS_OBJECT_MANAGER_CLIENT(manager));
	g_print("Added object at %s (owner %s)\n",
			g_dbus_object_get_object_path(object), owner);
	g_free(owner);

	print_objects(manager);
}

static void on_object_removed(GDBusObjectManager *manager, GDBusObject *object, gpointer user_data) {
	S_;
	gchar *owner = g_dbus_object_manager_client_get_name_owner(
			G_DBUS_OBJECT_MANAGER_CLIENT(manager));
	g_print("Removed object at %s (owner %s)\n",
			g_dbus_object_get_object_path(object), owner);
	g_free(owner);
	print_objects(manager);
}

static void on_notify_name_owner(GObject *object, GParamSpec *pspec, gpointer user_data) {
	S_;
	auto manager = G_DBUS_OBJECT_MANAGER_CLIENT(object);
	auto name_owner = g_dbus_object_manager_client_get_name_owner(manager);
	g_print("name-owner: %s\n", name_owner);
	g_free(name_owner);
}


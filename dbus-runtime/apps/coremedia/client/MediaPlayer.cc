
/*
 * MediaPlayer.cc
 *
 *  Created on: Nov 20, 2015
 *      Author: buttonfly
 */


extern "C" {
#include "gdbus-mediakit-generated.h"
}

#include "../../coremedia/client/include/MediaPlayer.h"

#include "osldbg.h"

typedef void(*SignalCallback)(GDBusObjectManagerClient *manager, GDBusObjectProxy *object_proxy, GDBusProxy *interface_proxy, const gchar *sender_name, const gchar *signal_name, GVariant *parameters, void *userdata);
typedef void(*PropertiesChangedCallback)(GDBusObjectManagerClient *, GDBusObjectProxy *, GDBusProxy *, GVariant *, const gchar * const *, void *);

extern "C"
int client_main(int argc, char *argv[]);

extern "C"
GDBusInterface *busInterface();

extern "C"
void connect_proxy_signal(void(*fn)(GDBusObjectManagerClient *, GDBusObjectProxy *, GDBusProxy *, const gchar *, const gchar *, GVariant *, void *), void *userdata);

extern "C"
void connect_proxy_properties_changed(void(*fn)(GDBusObjectManagerClient *, GDBusObjectProxy *, GDBusProxy *, GVariant *, const gchar * const *, void *), void *userdata);



static void mediakit_call_play_result (GObject *source_object, GAsyncResult *res, gpointer user_data) {
	S_;
	GError *error = NULL;
//	bf_com_halkamalka_gdbus_object_manager_mediakit_call_play_finish((BFComHalkamalkaGDBusObjectManagerMediakit *)source_object,  res, &error);
//	if(error) {
//		g_printerr("Error getting object manager client: %s", error->message);
//		g_error_free(error);
//	}
}

static void mediakit_call_prepare_result(GObject *source_object, GAsyncResult *res, gpointer user_data) {
	S_;
	GError *error = NULL;
//	bf_com_halkamalka_gdbus_object_manager_mediakit_call_prepare_finish((BFComHalkamalkaGDBusObjectManagerMediakit *)source_object,  res, &error);
//	if(error) {
//		g_printerr("Error getting object manager client: %s", error->message);
//		g_error_free(error);
//	}
}

bool k::MediaPlayer::isInit = false;

namespace k {

MediaPlayer::MediaPlayer()
{
	if(isInit == false) {
		client_main(0, NULL);
		isInit = true;
	}

	// connect signal, propertiesChanged
	connect_proxy_signal ((SignalCallback) signalReceived, (void *)this);
	connect_proxy_properties_changed((PropertiesChangedCallback) propertiesChanged, (void *) this);
}

MediaPlayer::~MediaPlayer() {
	// TODO Auto-generated destructor stub
	// disconnect
}

void MediaPlayer::setDataSource(std::string &uri) {
	DataSource dataSource(uri);
	setDataSource(dataSource);
}

void MediaPlayer::setDataSource(DataSource &dataSource) {
	S_;
	m_("%s", dataSource.uri().c_str());
	auto interface = busInterface();
	mediakit_interface_call_create_source((mediakitInterface *)interface, NULL, NULL, NULL);
	mediakit_interface_call_create_sink((mediakitInterface *)interface, NULL, NULL, NULL);
//	void mediakit_interface_call_create_source (
//	    mediakitInterface *proxy,
//	    GCancellable *cancellable,
//	    GAsyncReadyCallback callback,
//	    gpointer user_data);
//	bf_com_halkamalka_gdbus_object_manager_mediakit_set_data_source((BFComHalkamalkaGDBusObjectManagerMediakit *)interface,  (const gchar *) dataSource.uri().c_str());
	_dataSource = dataSource;
}

auto MediaPlayer::dataSource() -> const DataSource & {
	S_;
	auto interface = busInterface();
//	const char *data_source = bf_com_halkamalka_gdbus_object_manager_mediakit_get_data_source ((BFComHalkamalkaGDBusObjectManagerMediakit *)interface);
//	_dataSource.initWith(data_source);
	return _dataSource;
}

auto MediaPlayer::prepare() -> int {
	S_;
	auto interface = busInterface();
//	bf_com_halkamalka_gdbus_object_manager_mediakit_call_prepare((BFComHalkamalkaGDBusObjectManagerMediakit *)interface, NULL, bf_com_halkamalka_gdbus_object_manager_mediakit_call_prepare_result, NULL);
	return OK;
}

auto MediaPlayer::play() -> int {
	S_;
//	auto interface = busInterface();
//#if !ASYNC_CALL
//	bf_com_halkamalka_gdbus_object_manager_mediakit_call_play((BFComHalkamalkaGDBusObjectManagerMediakit *)interface, NULL, bf_com_halkamalka_gdbus_object_manager_mediakit_call_play_result, NULL);
//#else // SYNC
//	GError *error = NULL;
//	bf_com_halkamalka_gdbus_object_manager_mediakit_call_play_sync((BFComHalkamalkaGDBusObjectManagerMediakit *)interface, (GCancellable *) NULL, &error);
//	if(error) {
//		g_printerr("Error getting object manager client: %s", error->message);
//		g_error_free(error);
//	}
//#endif
	return OK;
}

auto MediaPlayer::stop() -> int {
	S_;
//	auto interface = busInterface();
//	GError *error = NULL;
//	bf_com_halkamalka_gdbus_object_manager_mediakit_call_stop_sync((BFComHalkamalkaGDBusObjectManagerMediakit *)interface, NULL, &error);
//	if(error) {
//		g_printerr("Error getting object manager client: %s", error->message);
//		g_error_free(error);
//	}
	return OK;
}

void MediaPlayer::signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata) {
	S_;
	KASSERT(userdata);
//	MediaPlayer *self = (MediaPlayer *) userdata;
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

void MediaPlayer::propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata) {
	S_;
	KASSERT(userdata);
//	MediaPlayer *self = (MediaPlayer *) userdata;

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

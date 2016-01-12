/*
 * Sink.cc
 *
 *  Created on: Dec 16, 2015
 *      Author: buttonfly
 */

#include "Sink.h"
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

Sink::Sink()
: _dataSink("")
{
	// TODO Auto-generated constructor stub

}

Sink::~Sink() {
	// TODO Auto-generated destructor stub
}

void Sink::setDataSink(DataSink &dataSink) {
	_dataSink = dataSink;

	m_("addr: %s", _dataSink.uri().c_str());
	std::string uri = _dataSink.uri();
	std::size_t hashcode = std::hash<std::string>{}(uri);

	std::string path(GDBUS_COREMEDIA_SINK_POOL_OBJECT_PATH);
	path.append("/");
	path.append(std::to_string((long unsigned int)hashcode));
	m_("path: %s", path.c_str());

	DBusObjectManager &dbusObjectManager = DBusObjectManager::shared();
	auto dbusObject = dbusObjectManager.dbusObject(path);
	assert(dbusObject);
	auto interface = dbusObjectManager.dbusInterface(path.c_str(), GDBUS_COREMEDIA_SINK_INTERFACE_NAME);
	assert(interface);

	coremedia_sink_set_data_sink ((CoremediaSink *) interface, _dataSink.uri().c_str());
}

auto Sink::dataSink()->const DataSink &{

	return _dataSink;
}


auto Sink::prepare()->int{
	S_;
//	m_("addr: %s", _dataSource.baseUri().c_str());
//	std::string uri = _dataSource.baseUri();
//	std::size_t hashcode = std::hash<std::string>{}(uri);
//
//	std::string path(GDBUS_COREMEDIA_SOURCE_POOL_OBJECT_PATH);
//	path.append("/");
//	path.append(std::to_string((long unsigned int)hashcode));
//	m_("path: %s", path.c_str());
//
//	DBusObjectManager &dbusObjectManager = DBusObjectManager::shared();
//	auto dbusObject = dbusObjectManager.dbusObject(path);
//	assert(dbusObject);
//	auto interface = dbusObjectManager.dbusInterface(path.c_str(), GDBUS_COREMEDIA_SOURCE_INTERFACE_NAME);
//	assert(interface);
//
//	GError *error = NULL;
//	coremedia_source_call_prepare_sync((CoremediaSource *) interface, NULL, &error);
//	if(error) {
//		g_printerr("Error getting object manager client: %s", error->message);
//		g_error_free(error);	//TODO:
//	}
	return 0;
}

auto Sink::isPrepared()->bool {
	S_;
	return 0;
}

auto Sink::start()->int {
	S_;
	return 0;
}

auto Sink::stop()->int {
	S_;
	return 0;
}



} /* namespace k */

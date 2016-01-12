/*
 * json_websocket.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: buttonfly
 */

#include "json_websocket.h"

namespace k {

json_websocket::json_websocket(struct mg_connection* conn) : websocket(conn) {
	// TODO Auto-generated constructor stub

}

json_websocket::~json_websocket() {
	// TODO Auto-generated destructor stub
}

int json_websocket::write(Json::Value& response) {
	Json::FastWriter writer;
	std::string msg = writer.write(response);
#if 0
	std::cerr << msg << std::endl;
#endif
	return websocket::write(msg);
}

} /* namespace k */

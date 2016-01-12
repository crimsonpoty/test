/*
 * json_websocket.h
 *
 *  Created on: Jul 10, 2013
 *      Author: buttonfly
 */

#ifndef JSON_WEBSOCKET_H_
#define JSON_WEBSOCKET_H_

#include "websocket.h"
#include <jsonrpc/jsonrpc.h>

namespace k {

class json_websocket : public websocket{
public:
	json_websocket(struct mg_connection* conn);
	virtual ~json_websocket();

	int write(Json::Value& response);
};

} /* namespace k */
#endif /* JSON_WEBSOCKET_H_ */

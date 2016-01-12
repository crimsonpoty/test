/*
 * websocket.h
 *
 *  Created on: Jun 10, 2013
 *      Author: buttonfly
 */

#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

#include "mongoose.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include "osl.h"

namespace k {

class websocket {
public:
	websocket(struct mg_connection* conn);
	virtual ~websocket();

	virtual std::string read();
	virtual int write(std::string& msg);
	virtual int write(const char* buf, size_t len);

	std::string get_remote_ip(void* conn);

protected:
	const struct mg_connection* m_conn;
};

} /* namespace k */

#if 0
class websocket {
public:
	websocket();
	virtual ~websocket();
};
#endif

#endif /* WEBSOCKET_H_ */


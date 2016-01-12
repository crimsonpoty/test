/*
 * mgsocket.h
 *
 *  Created on: Feb 28, 2015
 *      Author: buttonfly
 */

#ifndef APPS_LIBWEBSERVER_MGSOCKET_H_
#define APPS_LIBWEBSERVER_MGSOCKET_H_


#include "mongoose.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include "osl.h"

namespace k {

class mgsocket {
public:
	mgsocket(struct mg_connection* conn);
	virtual ~mgsocket();

	virtual std::string read();
	virtual int write(std::string& msg);
	virtual int write(const char* buf, size_t len);

	void set_header(const char* name, const char* v);

	std::string get(std::string name);

	std::string get_remote_ip(void* conn);

protected:
	const struct mg_connection* _conn;
};

} /* namespace k */

#endif /* APPS_LIBWEBSERVER_MGSOCKET_H_ */

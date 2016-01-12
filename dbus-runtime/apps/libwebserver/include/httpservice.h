/*
 * httpservice.h
 *
 *  Created on: Feb 27, 2015
 *      Author: buttonfly
 */

#ifndef APPS_LIBWEBSERVER_HTTPSERVICE_H_
#define APPS_LIBWEBSERVER_HTTPSERVICE_H_

#include <map>
#include <string>
#include "httpservice_handler.h"

namespace k {

class httpservice {
public:
#if old
//	virtual int bind(std::map<std::string, httpservice*>& handler)=0;
//	virtual int unbind(std::map<std::string, httpservice*>& handler)=0;
//	virtual int invoke(void* conn)=0;
#endif
	virtual int bind(httpservice_handler &handler)=0;
	virtual int unbind(httpservice_handler &handler)=0;
};

} /* namespace k */

#endif /* APPS_LIBWEBSERVER_HTTPSERVICE_H_ */


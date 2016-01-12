/*
 * httpservice_handler.cc
 *
 *  Created on: Oct 2, 2015
 *      Author: buttonfly
 */

#include "httpservice_handler.h"

namespace k {

void httpservice_handler::add(std::string url, httpservice_func func, void *userdata) {
	_services[url] = httpservice_handler::service(func, userdata);
}

void httpservice_handler::remove(std::string url) {
	service_map::iterator pos = _services.find(url);
	if(pos!=_services.end()) _services.erase(pos);
}

int httpservice_handler::exec(std::string url, void *conn) {
	service_map::iterator pos = _services.find(url);
	if(pos==_services.end()) return -1;
	return 	pos->second.func()(pos->second.userdata(), conn);
}

} /* namespace k */

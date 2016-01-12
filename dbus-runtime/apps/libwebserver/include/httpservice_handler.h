/*
 * httpservice_handler.h
 *
 *  Created on: Oct 2, 2015
 *      Author: buttonfly
 */

#ifndef LIBWEBSERVER_HTTPSERVICE_HANDLER_H_
#define LIBWEBSERVER_HTTPSERVICE_HANDLER_H_

#include <string>
#include <map>

namespace k {

//typedef std::string (*httpservice_func)(void* userdata, void *conn);
typedef int (*httpservice_func)(void* userdata, void *conn);

class webservice;
class httpservice_handler {
public:
	friend class webservice;
	class service {
	public:
		service() : _func(NULL), _userdata(NULL){}
		service(httpservice_func func, void *userdata) : _func(func), _userdata(userdata) {}
		virtual ~service() {}
		httpservice_func func() {
			return _func;
		}
		void *userdata() {
			return _userdata;
		}

	private:
		httpservice_func _func;
		void *_userdata;
	};

	void add(std::string url, httpservice_func func, void *userdata);
	void remove(std::string url);

private:
	int exec(std::string url, void *conn);

private:
	typedef std::map<std::string, k::httpservice_handler::service> service_map;
	service_map _services;
};

} /* namespace k */

#endif /* LIBWEBSERVER_HTTPSERVICE_HANDLER_H_ */

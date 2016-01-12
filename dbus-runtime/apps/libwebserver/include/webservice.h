/*
 * webservice.h
 *
 *  Created on: Jun 8, 2013
 *      Author: buttonfly
 */

#ifndef WEBSERVICE_H_
#define WEBSERVICE_H_

#define REST_SUPPORT	1

#include <string>
#include <map>
#include <json/json.h>
#include <jsonrpc/jsonrpc.h>
#include "plugin.h"
#if REST_SUPPORT
#include "httpservice.h"
#endif
#include "osl.h"
#include "jsonrpc/jsonrpc_handler_ex.h"
#include <list>

#include "httpservice_handler.h"
#include "workerthread.hpp"

namespace k {

class webservice : public virtual runnable {
public:
	webservice();
	virtual ~webservice();

	//@deprecated
	int start(void);
	int start(const char *options[]);
	int wait(void);
	void wait_async(void);
	int stop(void);

    int add(plugin* p);
    int remove(plugin* p);

    // @implement runnable
    void run();

    int add(httpservice* s);
    int remove(httpservice* s);
#if deprecated //REST_SUPPORT
    int add(std::string uri, httpservice* p);
    int remove(std::string uri);
#endif

	std::string process(std::string& msg, void* arg0);
	int response(void *conn, std::string& msg, uint8_t key[4]);

	int lock(void);
	int unlock(void);

private:
	static int event_handler(void *conn, int event);
	static int iterate_callback(void *c, int event);

	int fire_sck_closed(void* conn);

	k::workerthread *workerthread();

	void* m_context;
#if 0
	Json::Rpc::Handler m_jsonHandler;
#else
	k::HandlerEx m_jsonHandler;
#endif
	osl_mutex m_lck;
	std::list<plugin*> m_plugins;

#if REST_SUPPORT
	std::map<std::string, httpservice*> _httpservices;
	httpservice_handler m_httpserviceHandler;
#endif

	k::workerthread *_workerthread;
};

} /* namespace k */
#endif /* WEBSERVICE_H_ */

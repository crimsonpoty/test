/*
 * webservice.cc
 *
 *  Created on: Jun 8, 2013
 *      Author: buttonfly
 */

#include "webservice.h"
#include "websocket.h"

#include <stdio.h>
#include <string.h>
#include "mongoose.h"
#include <assert.h>
#include <string>
#include <iostream>

#include "osl.h"
#include "osldbg.h"

#include <jsonrpc/jsonrpc.h>
#include "jsonrpc/jsonrpc_handler_ex.h"


#include <arpa/inet.h>

#if 0
#define S_
#define M_
#endif
#define SYSLOG printf

using namespace std;

static void ___s(void* arg) {

	unsigned char buf[40];
	buf[0] = 0x81;
	buf[1] = snprintf((char *) buf + 2, sizeof(buf) - 2, "%s", "server ready");

	for(;;) {
		mg_write((struct mg_connection *)arg, buf, 2 + buf[1]);
		sleep(1);
	}
}



namespace k {

webservice::webservice() :
		m_context(NULL)
, _workerthread(NULL)
{
	m_lck = osl_mutex_create();
}

webservice::~webservice() {
	if(_workerthread) {
		_workerthread->uninitialize();
		delete _workerthread;
		_workerthread = NULL;
	}

	osl_mutex_close(m_lck);
}

int webservice::start(void) {

//	struct mg_callbacks callbacks;
//	const char *options[] = { "listening_ports", "8080", "document_root",
//			"webapp", NULL };
//
//	memset(&callbacks, 0, sizeof(callbacks));
//
////	callbacks.websocket_ready = websocketReady;
//	callbacks.websocket_data = websocket_data;
//	callbacks.websocket_connect = websocket_connect;
//	callbacks.log_message = log;
////	callbacks.begin_request = beginRequest;
//
//	m_context = mg_start(&callbacks, this, options);
	return 0;
}

int webservice::wait(void) {
	time_t current_timer = 0, last_timer = time(NULL);
	for (;;) {
		mg_poll_server((struct mg_server *)m_context, 1000);
		current_timer = time(NULL);
		if (current_timer - last_timer > 0) {
			last_timer = current_timer;
			mg_iterate_over_connections((struct mg_server *)m_context, (int (*)(mg_connection *, enum mg_event))iterate_callback, &current_timer);
		}
	}
	return 0;
}

void webservice::wait_async(void) {
	this->workerthread()->invoke(this);
}

void webservice::run() {
	wait();
}

k::workerthread *webservice::workerthread() {
	if(_workerthread==NULL) {
		_workerthread = new k::workerthread;
		_workerthread->initialize(1);
	}
	return _workerthread;
}

int webservice::start(const char *options[]) {
	m_context = mg_create_server(this, (int (*)(mg_connection *, enum mg_event))webservice::event_handler);
	KASSERT(m_context);
	for(int i = 0; options[i] != NULL; i+=2) {
		printf("{ %s : %s }\n", options[i], options[i+1]);
		const char* msg = mg_set_option((struct mg_server *)m_context, options[i],  options[i+1]);
		if(msg) fprintf(stderr, "W: %s\n", msg);
	}
	printf("Started on port %s\n", mg_get_option((struct mg_server *)m_context, "listening_port"));

	return 0;
}

int webservice::stop(void) {
//	mg_stop((struct mg_context*) m_context);
	mg_destroy_server((struct mg_server **)&m_context);

	return 0;
}

int webservice::add(plugin* p) {
	m_plugins.insert(m_plugins.end(), p);
	p->bind(m_jsonHandler);
	return 0;
}

int webservice::remove(plugin* p) {
	p->unbind(m_jsonHandler);
	m_plugins.remove(p);
	return 0;
}

int webservice::add(httpservice* s) {
//	s->bind(_httpservices);
	s->bind(m_httpserviceHandler);
	return 0;
}

int webservice::remove(httpservice* s) {
//	s->unbind(_httpservices);
	s->unbind(m_httpserviceHandler);
	return 0;
}

#if 0
std::string webservice::process(std::string& msg) {
	Json::Value response;
	bool r = m_jsonHandler.Process(msg, response);
	if(!r) {
		std::cerr << msg << std::endl;
		// TODO
	}

    if(response != Json::Value::null) {
      return m_jsonHandler.GetString(response);
//      std::cerr << rep << std::endl;
    }
	return "";
}
#else
std::string webservice::process(std::string& msg, void* arg0) {
	Json::Value response;
	bool r = m_jsonHandler.Process(msg, response, arg0);

#if 0
	if(!r) {
		ERR("%s", msg.c_str());
		// TODO
		return "";
	}
#endif

    if(!response.isNull()) {
      return m_jsonHandler.GetString(response);
//      std::cerr << rep << std::endl;
    }
	return "";
}
#endif

int webservice::response(void *conn, std::string& msg, uint8_t key[4]) {

	// TODO
	int len = msg.length();
	if(len<=0)
		return -1;

	unsigned char* buf = new unsigned char[len+1];
	memset(buf, 0, len + 1);
	buf[0] = 0x81;  // text, FIN set
	buf[1] = len & 0x7F;

	for (int i = 0; i < len; i++) {
		buf[i + 2] = msg.c_str()[i];
	}

	mg_write((struct mg_connection*) conn, buf, 2 + len);

	delete[] buf;
	return 0;
}

int webservice::lock(void) {
	osl_mutex_lock(m_lck);
	return 0;
}

int webservice::unlock(void) {
	osl_mutex_unlock(m_lck);
	return 0;
}

int webservice::event_handler(void *c, int e) {
	struct mg_connection * conn = (struct mg_connection *)c;
	enum mg_event event = (enum mg_event) e;
	if(event == MG_CONNECT) {
		SYSLOG("sck : [%s/%s] connect", conn->local_ip, conn->remote_ip);
		return MG_TRUE;
	}
	else if (event == MG_REQUEST) {
		if (conn->is_websocket) {
			k::websocket websocket(conn);
			std::string req = websocket.read();

			k::webservice* context = (k::webservice*) conn->server_param;
			KASSERT(context);
			if(req.length() == 0) {
				SYSLOG("[%s/%s] req cannot be 0. simply ignore it.", conn->local_ip, conn->remote_ip);
				return MG_FALSE;
			}
			std::string res = context->process(req, (void*) conn);
			int n = websocket.write(res);
			return (n==4 && !res.compare("exit")) ? MG_FALSE : MG_TRUE;
		}
		else {
#if REST_SUPPORT
			k::webservice* context = (k::webservice*) conn->server_param;
			if(context->m_httpserviceHandler.exec(conn->uri, conn) == 0) {
				return MG_TRUE;
			}
			else {
				//TODO://
			}
#else
		    mg_send_header(conn, "Content-Type", "text/html");
			mg_send_data(conn, index_html, index_size);
			SYSLOG("This is not a websocket");
#endif

			// It must be false. if not, it doesn't work.
		    return MG_FALSE;
		}
	}
	else if (event == MG_AUTH) {
//		SYSLOG("auth...");
		return MG_TRUE;
	}
	else if (event == MG_CLOSE) {
		k::webservice* context = (k::webservice*) conn->server_param;
		if (conn->is_websocket) {
			SYSLOG("websocket  [%s/%s] closed", conn->local_ip, conn->remote_ip);
			context->fire_sck_closed(conn);
			return MG_TRUE; // It's gonna be ignore, anyway.
		}
	}

	return MG_FALSE;
}

int webservice::iterate_callback(void *c, int e) {
	struct mg_connection * conn = (struct mg_connection *)c;
	enum mg_event event = (enum mg_event) e;

	if (event == MG_POLL && conn->is_websocket) {
		char buf[20];
		int len = snprintf(buf, sizeof(buf), "%lu",
				(unsigned long) * (time_t *) conn->callback_param);
		mg_websocket_write(conn, 1, buf, len);
	}
	return MG_TRUE;
}

int webservice::fire_sck_closed(void* conn) {
	for(std::list<plugin*>::iterator pos = m_plugins.begin(); pos != m_plugins.end(); ++pos) {
		plugin* p = (plugin*)*pos;
		if(p) {
			p->sck_closed(conn);
		}
	}
	return 0;
}

} /* namespace k */

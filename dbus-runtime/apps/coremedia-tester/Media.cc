/*
 * Media.cc
 *
 *  Created on: Oct 25, 2015
 *      Author: buttonfly
 */

#include "Media.h"
#include "httpservice.h"
#include "httpservice_handler.h"
#include "osldbg.h"
#include <string>

#include "HttpQuery.h"
#include "DataSource.h"
#include "DataSink.h"
#include "SourcePool.h"
#include "SinkPool.h"
#include <assert.h>
#ifndef YES
#define YES true
#endif

#ifndef NO
#define NO false
#endif

#define OK		0
#define NOT_OK	-1

using namespace std;

namespace k {

Media::Media()
: _source(NULL), _sink(NULL)
{
	// TODO Auto-generated constructor stub
}

Media::~Media() {
	// TODO Auto-generated destructor stub
}

auto Media::bind(httpservice_handler &handler) -> int {
	handler.add("/media", (httpservice_func) RESTAPI_media, this);
	return OK;
}

auto  Media::unbind(httpservice_handler &handler) -> int{
	handler.remove("/media");
	return OK;
}

auto Media::RESTAPI_media(void *userdata, struct mg_connection * conn) -> int {
	S_;
	k::Media *self = (k::Media *) userdata;

	std::lock_guard<std::mutex> guard(self->_lck);

	Json::Value root;
	Json::Value res;
	if(HttpQuery::REST_parse(root, conn) != YES) {
		res["result"] = -1;
		HttpQuery::REST_response(res, conn);
		return OK;
	}

	std::string method(conn->request_method);
	if(method.compare("GET")==0) {
		//TODO:
	}
	else if(method.compare("PUT")==0) {
		auto uri = root["DataSource"].asString();
		m_("uri: %s", uri.c_str());
		k::DataSource dataSource(uri);

		uri = root["DataSink"].asString();
		if(uri.empty()) {
			// default
			uri = "video://0";
		}

		//TODO: record check
		if(self->_source) {
			SourcePool::shared().release(self->_source);
			self->_source = NULL;
		}

		Source *source = SourcePool::shared().request(dataSource);
		self->_source = source;
		assert(self->_source);

		DataSink dataSink(uri);
		if(self->_sink) {
			SinkPool::shared().release(self->_sink);
			self->_sink = NULL;
		}
		self->_sink = SinkPool::shared().request(dataSink);
		assert(self->_sink);

//		source->setDataSource(dataSource);
		source->prepare();
		self->_sink->prepare();

		NativeHandle pidChannel = source->requestPidChannel(10);
		source->releasePidChannel(pidChannel);

		res["pid"] = (unsigned int)pidChannel;
		res["result"] = 0;
	}
	else if(method.compare("POST")==0) {
		//TODO
	}
	else if(method.compare("DELETE")==0) {
		//TODO:
	}

	HttpQuery::REST_response(res, conn);
	return OK;
}

} /* namespace k */

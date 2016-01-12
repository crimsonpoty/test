/*
 * foo.cc
 *
 *  Created on: Feb 27, 2015
 *      Author: buttonfly
 */

#include "foo.h"
#include "osldbg.h"
#include "mongoose.h"
#include <string.h>
#include <json/json.h>
#include "mgsocket.h"

#define SYSLOG printf

namespace k {

foo::foo() {
	// TODO Auto-generated constructor stub

}

foo::~foo() {
	// TODO Auto-generated destructor stub
}

int foo::bind(std::map<std::string, httpservice*>& handler) {
	S_;
	handler["/api/foo/hello"] = this;
	handler["/api/foo/hello2"] = this;
	handler["/api/foo/hello3"] = this;

	return 0;
}

 int foo::unbind(std::map<std::string, httpservice*>& handler) {
	 S_;
	 handler.erase("/api/foo/hello");
	 handler.erase("/api/foo/hello2");
	 handler.erase("/api/foo/hello3");
	 return 0;
 }

int foo::invoke(void* c) {
	S_;

	//			struct mg_connection {
	//			  const char *request_method; // "GET", "POST", etc
	//			  const char *uri;            // URL-decoded URI
	//			  const char *http_version;   // E.g. "1.0", "1.1"
	//			  const char *query_string;   // URL part after '?', not including '?', or NULL
	//
	//			  char remote_ip[48];         // Max IPv6 string length is 45 characters
	//			  char local_ip[48];          // Local IP address
	//			  unsigned short remote_port; // Client's port
	//			  unsigned short local_port;  // Local port number
	//
	//			  int num_headers;            // Number of HTTP headers
	//			  struct mg_header {
	//			    const char *name;         // HTTP header name
	//			    const char *value;        // HTTP header value
	//			  } http_headers[30];
	//
	//			  char *content;              // POST (or websocket message) data, or NULL
	//			  size_t content_len;         // Data length
	//
	//			  int is_websocket;           // Connection is a websocket connection
	//			  int status_code;            // HTTP status code for HTTP error handler
	//			  int wsbits;                 // First byte of the websocket frame
	//			  void *server_param;         // Parameter passed to mg_add_uri_handler()
	//			  void *connection_param;     // Placeholder for connection-specific data
	//			  void *callback_param;       // Needed by mg_iterate_over_connections()
	//			};



//	 char n1[100], n2[100];
////	// Get form variables
//	mg_get_var(conn, "n1", n1, sizeof(n1));
//	mg_get_var(conn, "n2", n2, sizeof(n2));
//	mg_printf_data(conn, "{ \"result\": %lf }", strtod(n1, NULL) + strtod(n2, NULL));

	struct mg_connection * conn = (struct mg_connection *) c;
//	H_(conn->content, conn->content_len);
	// TODO
	// http://localhost:8081/api/hello?id=1
	SYSLOG("%s\n", conn->request_method); // GET
	SYSLOG("%s\n", conn->uri); // id=1
	if(conn->query_string) SYSLOG("%s\n", conn->query_string); // id
	if(conn->content) SYSLOG("%s\n", conn->content);


	Json::FastWriter writer;
	Json::Value req;

	if(strcmp(conn->uri, "/api/foo/hello")==0) {
		req["on"] = true;
		req["test"] = "this is json test";
		std::string body = writer.write(req);

	}
	else if(strcmp(conn->uri, "/api/foo/hello2")==0) {
		req["on"] = true;
		req["test"] = "this is json test2";
		std::string body = writer.write(req);

	}
	else if(strcmp(conn->uri, "/api/foo/hello3")==0) {
		req["on"] = true;
		req["test"] = "this is json test3";
		std::string body = writer.write(req);

	}

	mgsocket socket(conn);
	std::string v = socket.get("id");
	printf("#################### %s\n", v.c_str());


    socket.set_header("Access-Control-Allow-Origin", "*");
	socket.write(body);
	return 0;
}

} /* namespace k */

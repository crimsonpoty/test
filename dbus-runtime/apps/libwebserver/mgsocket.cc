/*
 * mgsocket.cc
 *
 *  Created on: Jun 10, 2013
 *      Author: buttonfly
 */

#include "mgsocket.h"

#include <stdio.h>
#include <string.h>
#include "mongoose.h"
#include <assert.h>
#include <string>
#include <iostream>
#include <stdint.h>
#include <arpa/inet.h>

namespace k {

mgsocket::mgsocket(struct mg_connection* conn)
: _conn(conn)
{
}

mgsocket::~mgsocket() {
	// TODO Auto-generated destructor stub
}

std::string mgsocket::read() {
	struct mg_connection * conn = (struct mg_connection *)_conn;
	return std::string(conn->content, conn->content_len);
}

/**
 * @return 0: is 0, -1: please disconnect
 */

int mgsocket::write(std::string& msg) {
//	return mg_websocket_write((struct mg_connection *)m_conn, 1, msg.c_str(), msg.length());
	return write(msg.c_str(), msg.length());
}

int mgsocket::write(const char* buf, size_t len) {
//	return mg_websocket_write((struct mg_connection *)m_conn, 1, buf, len);
	mg_send_data((struct mg_connection *)_conn,  (const void*) buf, (int) len);
	return len;
}

std::string mgsocket::get_remote_ip(void* c) {
	struct mg_connection * conn = (struct mg_connection *) c;
	return std::string(conn->remote_ip);
}

void mgsocket::set_header(const char* name, const char* v) {
	mg_send_header((struct mg_connection *) _conn, name, v);
}

std::string mgsocket::get(std::string name) {
	char tmp[128] = {0,};
	mg_get_var((struct mg_connection *) _conn, name.c_str(), tmp, sizeof(tmp));
	return std::string(tmp);
}

} /* namespace k */

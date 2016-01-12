/*
 * websocket.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: buttonfly
 */

#include "websocket.h"

#include <stdio.h>
#include <string.h>
#include "mongoose.h"
#include <assert.h>
#include <string>
#include <iostream>
#include <stdint.h>
#include <arpa/inet.h>
#include "osldbg.h"
#include "ioutil.h"

namespace k {

websocket::websocket(struct mg_connection* conn)
: m_conn(conn)
{
}

websocket::~websocket() {
	// TODO Auto-generated destructor stub
}

std::string websocket::read() {
	struct mg_connection * conn = (struct mg_connection *)m_conn;
	return std::string(conn->content, conn->content_len);
}

/**
 * @return 0: is 0, -1: please disconnect
 */

int websocket::write(std::string& msg) {
	return write(msg.c_str(), msg.length());
}

int websocket::write(const char* buf, size_t len) {
	return mg_websocket_write((struct mg_connection *)m_conn, 1, buf, len);
}

std::string websocket::get_remote_ip(void* c) {
	struct mg_connection * conn = (struct mg_connection *) c;
	return std::string(conn->remote_ip);
}


} /* namespace k */

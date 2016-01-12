/*
 * IPlugin.h
 *
 *  Created on: Jun 9, 2013
 *      Author: buttonfly
 */

#ifndef IPLUGIN_H_
#define IPLUGIN_H_

#include <jsonrpc/jsonrpc.h>
#include "jsonrpc/jsonrpc_handler_ex.h"
namespace k {

class plugin {
public:
#if 0
	virtual int bind(Json::Rpc::Handler& handler)=0;
	virtual int unbind(Json::Rpc::Handler& handler)=0;
#else
	virtual int bind(k::HandlerEx& handler)=0;
	virtual int unbind(k::HandlerEx& handler)=0;
	virtual void sck_closed(void* conn)=0;
#endif
};

} /* namespace k */
#endif /* IPLUGIN_H_ */

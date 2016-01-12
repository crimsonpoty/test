/*
 * Media.h
 *
 *  Created on: Oct 25, 2015
 *      Author: buttonfly
 */

#ifndef APPS_CATCAST_MEDIA_H_
#define APPS_CATCAST_MEDIA_H_

#include "httpservice.h"
#include "httpservice_handler.h"
#include <map>
#include <json/json.h>
#include "mgsocket.h"
#include <thread>
#include <mutex>

#include "Source.h"
#include "Sink.h"
#include "MediaPlayer.h"

namespace k {

class Media : public k::httpservice{
public:
	Media();
	virtual ~Media();

	//@ httpservice
	int bind(httpservice_handler &handler) override final;
	int unbind(httpservice_handler &handler) override final;

	static auto RESTAPI_media(void *userdata, struct mg_connection * conn) -> int;

private:
	typedef std::map<std::string, std::string> Dictionary;
	Dictionary _playlst;

	std::mutex _lck;
//	k::MediaPlayer _player;
	k::Source *_source;
	k::Sink *_sink;
};

}

#endif /* APPS_CATCAST_MEDIA_H_ */

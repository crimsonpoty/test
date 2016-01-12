/*
 * MediaPlayer.h
 *
 *  Created on: Nov 20, 2015
 *      Author: buttonfly
 */

#ifndef APPS_MEDIAKIT_MEDIAPLAYER_H_
#define APPS_MEDIAKIT_MEDIAPLAYER_H_

#include <string>
#include <stdlib.h>

#include "DataSource.h"

namespace k {

class MediaPlayer {
public:
	MediaPlayer();
	virtual ~MediaPlayer();

	void setDataSource(std::string &uri);
	void setDataSource(DataSource &dataSource);

	auto dataSource() -> const DataSource &;

	auto prepare() -> int;
	auto play() -> int;
	auto stop() -> int;

private:
	static void signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata);
	static void propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata);

private:
	static bool isInit;
	DataSource _dataSource;
};

} /* namespace k */

#endif /* APPS_MEDIAKIT_MEDIAPLAYER_H_ */

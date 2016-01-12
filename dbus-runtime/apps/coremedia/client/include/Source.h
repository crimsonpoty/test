/*
 * Source.h
 *
 *  Created on: Dec 16, 2015
 *      Author: buttonfly
 */

#ifndef APPS_MEDIAKIT_CLIENT_SOURCE_H_
#define APPS_MEDIAKIT_CLIENT_SOURCE_H_

#include "DataSource.h"
#include "base.h"

namespace k {

class SourcePool;
class Source {
public:
	friend SourcePool;
	void setDataSource(DataSource &dataSource);
	const DataSource &dataSource();

	auto prepare()->int;
	auto isPrepared()->bool ;
	auto start()->int;
	auto stop()->int;
	auto requestPidChannel(unsigned short pid)->NativeHandle;
	void releasePidChannel(NativeHandle pidChannelHandle);

private:
	auto dbusInterface()->void *;

protected:
	DataSource _dataSource;

private:
	Source();
	virtual ~Source();

private:
	static void signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata);
	static void propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata);
};

} /* namespace k */

#endif /* APPS_MEDIAKIT_CLIENT_SOURCE_H_ */

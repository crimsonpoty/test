/*
 * Sink.h
 *
 *  Created on: Dec 16, 2015
 *      Author: buttonfly
 */

#ifndef APPS_MEDIAKIT_CLIENT_SINK_H_
#define APPS_MEDIAKIT_CLIENT_SINK_H_

#include "DataSink.h"

namespace k {

class SinkPool;
class Sink {
public:
	friend SinkPool;
	void setDataSink(DataSink &dataSink);
	auto dataSink()->const k::DataSink &;

	auto prepare()->int;
	auto isPrepared()->bool ;
	auto start()->int;
	auto stop()->int;

protected:
	DataSink _dataSink;

private:
	Sink();
	virtual ~Sink();
};

} /* namespace k */

#endif /* APPS_MEDIAKIT_CLIENT_SINK_H_ */

/*
 * DataSink.h
 *
 *  Created on: Aug 28, 2015
 *      Author: buttonfly
 */

#ifndef COREMEDIA_DATASINK_H_
#define COREMEDIA_DATASINK_H_

#include <string>

#include "base.h"

namespace k {

class DataSink {
public:
	typedef enum {
		VIDEO,
		AUDIO,
		FILE,
		FIFO,
		FAKE,
		RTP,
		HTTP,
		BUF,
		UNKNOWN
	} SinkProtocol;

	DataSink(std::string uri);
	DataSink(const char *uri);
	virtual ~DataSink();

	DataSink(const DataSink &rhs);

	DataSink &operator=(const DataSink &rhs);
	void copy(const DataSink &rhs);


	bool operator == (const DataSink &rhs) const;
	bool operator != (const DataSink &rhs) const;
	bool operator < (const DataSink &rhs) const;
	bool operator > (const DataSink &rhs) const;
	bool operator <= (const DataSink &rhs) const;
	bool operator >= (const DataSink &rhs) const;

	DataSink::SinkProtocol protocol() const;
	const std::string &uri() const;
	void parse();

	// display
	const std::string &valueForKey(std::string key);
	void setValue(std::string key, std::string value);
	bool hasKey(std::string &key);

private:
	void init(const char *uri);
	void init(std::string uri);

	void parseForFileSink();
	void parseForFifoSink();
	void parseForBufSink();
	void parseForVideoSink();
	void parseForAudioSink();

private:
	std::string _uri;
	SinkProtocol _protocol;
	Properties _properties;
};

} /* namespace k */

#endif /* COREMEDIA2_DATASINK_H_ */

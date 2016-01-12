/*
 * DataSource.h
 *
 *  Created on: Aug 27, 2015
 *      Author: buttonfly
 */

#ifndef COREMEDIA_DATASOURCE_H_
#define COREMEDIA_DATASOURCE_H_

#include <string>
#include <map>
#include <list>

#include "base.h"
#include "osl.h"
//#include "Dictionary.h"

namespace k {

class DataSource {
public:
	typedef enum {
		RTP=0,
		UDP,
		RTSP,
		FILE,
		FIFO,
		HTTP,
		HTTPS,
		QAM,
		UNKNOWN
	} SourceProtocol;

	DataSource();
	DataSource(const char *uri);
	DataSource(std::string uri);
	DataSource(const DataSource &rhs);

	void initWith(const char *uri);
	void initWith(std::string uri);

	DataSource &operator=(const DataSource &rhs);
	void copy(const DataSource &rhs);

	virtual ~DataSource();

	bool operator == (const DataSource &rhs) const;
	bool operator != (const DataSource &rhs) const;
	bool operator < (const DataSource &rhs) const;
	bool operator > (const DataSource &rhs) const;
	bool operator <= (const DataSource &rhs) const;
	bool operator >= (const DataSource &rhs) const;

	SourceProtocol protocol() const;
	const std::string &uri() const;
	const std::string &baseUri() const;

	void parse();
	std::string pack();
	const std::string &valueForKey(std::string key);
	void setValue(std::string key, std::string value);
	// The type of the argument 'value' will be changed to 'std::string'
	void setValue(std::string key, int value);
	std::list<std::string> keys();
	bool hasKey(std::string key);
	bool hasKey(std::string key) const;

private:
	void parseForQamSource();
	void parseForFileSource();
	void parseForFifoSource();
	void parseForNetworkSource();
	void parseForHttpSource();
	void parseForRtpSource();

	std::string packForQamSource();
	std::string packForFileSource();
	std::string packForFifoSource();
	std::string packForNetworkSource();
	std::string packForHttpSource();
	std::string packForRtpSource();

private:
	std::string _uri;
	std::string _baseUri;
	SourceProtocol _protocol;
	Properties _properties;
};

} /* namespace k */

#endif /* COREMEDIA_DATASOURCE_H_ */

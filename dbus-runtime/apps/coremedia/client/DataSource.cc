/*
 * DataSource.cc
 *
 *  Created on: Aug 27, 2015
 *      Author: buttonfly
 */

#include "../../coremedia/client/include/DataSource.h"

#include <uriparser/Uri.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include "osl.h"
#include "osldbg.h"

#define S_
#define m_
#define Q_

namespace k {

DataSource::DataSource()
: _protocol(DataSource::UNKNOWN)
{

}

DataSource::DataSource(std::string uri)
: _protocol(DataSource::UNKNOWN)
{
	S_;
	initWith(uri);
}

DataSource::DataSource(const char *uri)
: _protocol(DataSource::UNKNOWN)
{
	S_;
	initWith(uri);
}

DataSource::DataSource(const DataSource &rhs) {
	copy(rhs);
}

DataSource &DataSource::operator=(const DataSource &rhs) {
	if (this == &rhs) return *this;
	copy(rhs);
	return *this;
}

DataSource::~DataSource() {
	S_;
}

DataSource::SourceProtocol DataSource::protocol() const {
	S_;
	if(_protocol!=UNKNOWN) return _protocol;

	else {
		SourceProtocol type  = DataSource::UNKNOWN;

	    UriParserStateA state;
	    UriUriA uri;

	    state.uri = &uri;
	    if (uriParseUriA(&state, _uri.c_str()) != URI_SUCCESS) {
	            /* Failure */
			uriFreeUriMembersA(&uri);
			fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
			return DataSource::UNKNOWN;
	    }

	    if(memcmp("rtp", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::RTP;
	    }
	    else if(memcmp("udp", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::UDP;
	    }
	    else if(memcmp("rtsp", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::RTSP;
	    }
	    else if(memcmp("file", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::FILE;
	    }
	    else if(memcmp("fifo", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::FIFO;
	    }
	    // workaround
	    else if(memcmp("xxx", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::FILE;
	    }
	    else if((memcmp("http", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0)) {
	    	type = DataSource::HTTP;
	    }
	    else if((memcmp("https", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0)) {
	    	type = DataSource::HTTPS;
	    }
	    else if(memcmp("dvb", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::QAM;
	    }
	    else if(memcmp("qam", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSource::QAM;
	    }
	    else {
	    	W_("Unknown type: %d", type);
	    }

	    uriFreeUriMembersA(&uri);
		return type;
	}

//	SourceProtocol type  = DataSource::UNKNOWN;
//	if(_protocol==UNKNOWN)
//	{
//	    UriParserStateA state;
//	    UriUriA uri;
//
//	    state.uri = &uri;
//	    if (uriParseUriA(&state, _uri.c_str()) != URI_SUCCESS) {
//	            /* Failure */
//			uriFreeUriMembersA(&uri);
//			fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
//			return DataSource::UNKNOWN;
//	    }
//
//	    if(memcmp("rtp", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::RTP;
//	    }
//	    else if(memcmp("udp", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::UDP;
//	    }
//	    else if(memcmp("rtsp", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::RTSP;
//	    }
//	    else if(memcmp("file", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::FILE;
//	    }
//	    // workaround
//	    else if(memcmp("xxx", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::FILE;
//	    }
//	    else if((memcmp("http", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0)) {
//	    	type = DataSource::HTTP;
//	    }
//	    else if((memcmp("https", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0)) {
//	    	type = DataSource::HTTPS;
//	    }
//	    else if(memcmp("dvb", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::QAM;
//	    }
//	    else if(memcmp("qam", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
//	    	type = DataSource::QAM;
//	    }
//	    else {
//	    	W_("Unknown type: %d", type);
//	    }
//
//	    uriFreeUriMembersA(&uri);
//	}
//	return type;
}

void DataSource::initWith(const char *uri) {
	S_;
	initWith(std::string(uri));
}

void DataSource::initWith(std::string uri) {
	S_;
	_properties.clear();

	_uri = uri;
	_protocol = protocol();
	parse();
	m_("uri: %s ,protocol: %d", _uri.c_str(), _protocol);
}

const std::string &DataSource::uri() const {
	return _uri;
};

const std::string &DataSource::baseUri() const {
	return _baseUri;
};

const std::string &DataSource::valueForKey(std::string key) {
	return _properties[key];
}

void DataSource::setValue(std::string key, std::string value) {
	_properties[key] = value;
}

void DataSource::setValue(std::string key, int value) {
	setValue(key, toString(value));
}

std::list<std::string> DataSource::keys() {
	std::list<std::string> keys;
	for(Properties::iterator pos = _properties.begin(); pos != _properties.end(); ++pos) {
		keys.insert(keys.end(), pos->first);
	}
	return keys;
}

bool DataSource::hasKey(std::string key) {
	for(Properties::iterator pos = _properties.begin(); pos != _properties.end(); ++pos) {
		if(key.compare(pos->first)==0) return true;
	}
	return false;
}

bool DataSource::hasKey(std::string key) const{
	return hasKey(key);
}

bool DataSource::operator == (const DataSource &rhs) const{
	if(rhs.protocol() != _protocol) {
		return false;
	}

#if 0
	if(protocol()==DataSource::QAM) {
		return (_baseUri.compare(rhs.baseUri())==0);
	}
	else {
		return (_uri.compare(rhs.uri()) == 0);
	}
	return false;
#else
	return (_baseUri.compare(rhs.baseUri())==0);
#endif
}

bool DataSource::operator != (const DataSource &rhs) const{
	return !operator == (rhs);
}

bool DataSource::operator < (const DataSource &rhs) const{
	return (_uri.compare(rhs.uri()) < 0);
}

bool DataSource::operator > (const DataSource &rhs) const{
	return operator < (rhs);
}

bool DataSource::operator <= (const DataSource &rhs) const{
	return !operator > (rhs);
}

bool DataSource::operator >= (const DataSource &rhs) const{
	return !operator < (rhs);
}

void DataSource::parse() {
	S_;
	DataSource::SourceProtocol prot = protocol();
	switch(prot) {
	case DataSource::QAM: {
		parseForQamSource();
		break;
	}
	case DataSource::FIFO: {
		parseForFifoSource();
		break;
	}
	case DataSource::FILE: {
		parseForFileSource();
		break;
	}
	case DataSource::HTTP:
	case DataSource::HTTPS: {
		parseForHttpSource();
		break;
	}
	default: {
		W_("Unknown protocol, It can happen if uri is not set., %s", _uri.c_str());
		KASSERT(1);
		break;
	}
	}
}

std::string DataSource::pack() {
	S_;
	switch(protocol()) {
	case DataSource::QAM:
		return packForQamSource();

	case DataSource::FIFO:
	case DataSource::FILE:
		return packForFileSource();

	case DataSource::HTTP:
	case DataSource::HTTPS:
		return packForHttpSource();

	default:
		W_("Unknown protocol, It can happen if uri is not set., %s",  _uri.c_str());
		KASSERT(1);
		break;
	}
	return std::string();
}

std::string DataSource::packForQamSource() {
	S_;
    //ex) dvb://256/A/350/6900000?videoPid=33&audioPid=36&pcrPid=33&pmtPid=34&videoCodec=h264&audioCodec=mpeg
	std::string uri (_baseUri);
	auto keys = this->keys();
	bool did = NO;
	for(auto pos = keys.begin(); pos != keys.end(); ++pos) {
		auto key = *pos;
		if(key.compare("freq")==0) continue;
		if(key.compare("annex")==0) continue;
		if(key.compare("qam")==0) continue;
		if(key.compare("symrate")==0) continue;

		auto value = valueForKey(key);
		if(did==NO) {
			uri += "?";
			did = YES;
		}
		else {
			uri += "&";
		}
		uri += key;
		uri += "=";
		uri += value;
	}
	return uri;
}

std::string DataSource::packForFileSource() {
	S_;
	std::string uri (_baseUri);
	auto keys = this->keys();
	bool did = NO;
	for(auto pos = keys.begin(); pos != keys.end(); ++pos) {
		auto key = *pos;
		if(key.compare("freq")==0) continue;
		if(key.compare("annex")==0) continue;
		if(key.compare("qam")==0) continue;
		if(key.compare("symrate")==0) continue;
		if(key.compare("path")==0) continue;
		if(key.compare("index")==0) continue;

		auto value = valueForKey(key);
		if(did==NO) {
			uri += "?";
			did = YES;
		}
		else {
			uri += "&";
		}
		uri += key;
		uri += "=";
		uri += value;
	}
	return uri;
}

std::string DataSource::packForNetworkSource() {
	S_;
	std::string uri (_baseUri);
	auto keys = this->keys();
	bool did = NO;
	for(auto pos = keys.begin(); pos != keys.end(); ++pos) {
		auto key = *pos;
		if(key.compare("freq")==0) continue;
		if(key.compare("annex")==0) continue;
		if(key.compare("qam")==0) continue;
		if(key.compare("symrate")==0) continue;
		if(key.compare("path")==0) continue;
		if(key.compare("index")==0) continue;

		auto value = valueForKey(key);
		if(did==NO) {
			uri += "?";
			did = YES;
		}
		else {
			uri += "&";
		}
		uri += key;
		uri += "=";
		uri += value;
	}
	return uri;
}

std::string DataSource::packForHttpSource() {
	std::string uri (_baseUri);
	auto keys = this->keys();
	bool did = NO;
	for(auto pos = keys.begin(); pos != keys.end(); ++pos) {
		auto key = *pos;
		if(key.compare("port")==0) continue;
		if(key.compare("ip")==0) continue;
		if(key.compare("url")==0) continue;
		if(key.compare("path")==0) continue;

		auto value = valueForKey(key);
		if(did==NO) {
			uri += "?";
			did = YES;
		}
		else {
			uri += "&";
		}
		uri += key;
		uri += "=";
		uri += value;
	}
	return uri;
}

std::string DataSource::packForRtpSource() {
	std::string uri (_baseUri);
	auto keys = this->keys();
	bool did = NO;
	for(auto pos = keys.begin(); pos != keys.end(); ++pos) {
		auto key = *pos;
		if(key.compare("port")==0) continue;
		if(key.compare("ip")==0) continue;
		if(key.compare("path")==0) continue;

		auto value = valueForKey(key);
		if(did==NO) {
			uri += "?";
			did = YES;
		}
		else {
			uri += "&";
		}
		uri += key;
		uri += "=";
		uri += value;
	}
	return uri;
}

void DataSource::parseForQamSource() {
	S_;
	const char* uri = this->uri().c_str();
	m_("uri : %s", uri);
	_baseUri = uri; //default

	UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
		return;
    }

    //ex) dvb://256/A/350/6900000
    //ex) dvb://256/A/350/6900000?videoPid=33&audioPid=36&pcrPid=33&pmtPid=34&videoCodec=h264&audioCodec=mpeg

    char tmp[64];
    // 0. QAM mode
    memset(tmp, 0, sizeof(tmp));
    memcpy(tmp, uriA.hostText.first, uriA.hostText.afterLast - uriA.hostText.first);
    m_("qam: %s", tmp);
    setValue("qam", tmp);

   // 1. Annex
   memset(tmp, 0, sizeof(tmp));
   UriPathSegmentA* node = uriA.pathHead;
   KASSERT(node);
   memcpy(tmp, node->text.first, node->text.afterLast - node->text.first);
   m_("annex: %s", tmp);
   setValue("annex", tmp);

   // 2. Freq
   memset(tmp, 0, sizeof(tmp));
   node = node->next;
   KASSERT(node);
   memcpy(tmp, node->text.first, node->text.afterLast - node->text.first);
   m_("freq: %s", tmp);
   setValue("freq", tmp);

   // 3. symbolrate
   memset(tmp, 0, sizeof(tmp));
   node = node->next;
   KASSERT(node);
   memcpy(tmp, node->text.first, node->text.afterLast - node->text.first);
   m_("symrate: %s", tmp);
   setValue("symrate", tmp);

   UriQueryListA * lstQuery = NULL;
   int count = 0;
   int program = 0;
   uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
   m_("count: %d", count);

   std::string path;
   // This is a default value. But, you might need to update it if query
   path = uriA.pathHead->text.first;
   for(UriQueryListA* query = lstQuery; query; query = query->next) {
	   setValue(query->key, query->value);
	   m_("%s, %s", query->key, query->value);

	   // Only once
	   if(uriA.query.first) {
		   char tmp[512] = {0};
				m_("%s", uriA.query.first);
				m_("%s", uriA.hostText.first);
				m_("%s", uriA.pathHead->text.first);
			memcpy(tmp, uriA.hostText.first ,  uriA.query.first - uriA.hostText.first-1);
			path = tmp;
	   }
	   //@description: It is going to be used to compare ==
	   // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
	   _baseUri = "dvb://" + path;
   }
   uriFreeQueryListA(lstQuery);
   uriFreeUriMembersA(&uriA);
}

void DataSource::parseForFileSource() {
	S_;
	const char* uri = this->uri().c_str();

    UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>\n");
		exit(0);
    }


    UriQueryListA * lstQuery = NULL;
    int count = 0;
    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
    m_("count: %d", count);

    std::string path;
  	path = uriA.pathHead->text.first-1;
    _baseUri = "file://" + path;
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
 	   m_("%s, %s", query->key, query->value);
        if(uriA.query.first) {
        	char tmp[512] = {0};
//    		m_("%s", uriA.query.first);
//    		m_("%s", uriA.pathHead->text.first-1);
    		memcpy(tmp, uriA.pathHead->text.first-1 ,  uriA.query.first - uriA.pathHead->text.first);
            path = tmp;
        }

//        //@description: It is going to be used to compare ==
//        // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
        _baseUri = "file://" + path;
    }

    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
    m_("path: %s", path.c_str());

    setValue("path", path);
    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSource::parseForFifoSource() {
	S_;
//	const char* uri = this->uri().c_str();
//
//    UriParserStateA state;
//    UriUriA uriA;
//
//    state.uri = &uriA;
//    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
//            /* Failure */
//		uriFreeUriMembersA(&uriA);
//		fprintf(stderr, "failed to <uriParseUriA>\n");
//		exit(0);
//    }
//
//
//    UriQueryListA * lstQuery = NULL;
//    int count = 0;
//    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
//    m_("count: %d", count);
//
//    std::string path;
//  	path = uriA.pathHead->text.first-1;
//    _baseUri = "fifo://" + path;
//    for(UriQueryListA* query = lstQuery; query; query = query->next) {
//    	setValueForKeyPath(query->key, query->value);
// 	   m_("%s, %s", query->key, query->value);
//        if(uriA.query.first) {
//        	char tmp[512] = {0};
////    		m_("%s", uriA.query.first);
////    		m_("%s", uriA.pathHead->text.first-1);
//    		memcpy(tmp, uriA.pathHead->text.first-1 ,  uriA.query.first - uriA.pathHead->text.first);
//            path = tmp;
//        }
//
////        //@description: It is going to be used to compare ==
////        // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//        _baseUri = "fifo://" + path;
//    }
//
//    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
//    m_("path: %s", path.c_str());
//
//    setValue("path", path);
//    uriFreeQueryListA(lstQuery);
//    uriFreeUriMembersA(&uriA);
}

void DataSource::parseForNetworkSource() {
	// TODO:
	S_;
	const char* uri = this->uri().c_str();

    UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>\n");
		exit(0);
    }

    UriQueryListA * lstQuery = NULL;
    int count = 0;
    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
    m_("count: %d", count);

    std::string path;

  	path = uriA.hostText.first;
  	m_("path: %s", path.c_str());
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
 	   m_("%s, %s", query->key, query->value);
        if(uriA.query.first) {
        	char tmp[512] = {0};
    		m_("query first: %s", uriA.query.first);
    		m_("head first: %s", uriA.pathHead->text.first-1);
    		memcpy(tmp, uriA.pathHead->text.first-1 ,  uriA.query.first - uriA.pathHead->text.first);
            path = tmp;
        }

//        //@description: It is going to be used to compare ==
//        // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//        _baseUri = "http://" + path;
    }


    _baseUri = uri;
	auto at = std::string::npos;
	if((at=_baseUri.find("?")) != std::string::npos) {
		_baseUri = _baseUri.substr(0, at);
	}

    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
    m_("path: %s", path.c_str());

    setValue("path", path);
    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSource::parseForHttpSource() {
	const char* uri = this->uri().c_str();

    UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>\n");
		exit(0);
    }

    // 1. ip
    {
		char tmp[64] = {0};
		memcpy(tmp, uriA.hostText.first, (uriA.hostText.afterLast - uriA.hostText.first));
		std::string ip = tmp;
        setValue("ip", ip);
    }

    // 2. url
    {
		char tmp[64] = {0};
		memcpy(tmp, uriA.hostText.first, (uriA.hostText.afterLast - uriA.hostText.first));
	    std::string url(uriA.pathHead->text.first - 1);
	    setValue("url", url);
    }

    std::string port;
	if(uriA.portText.first) {
		char tmp[64] = {0};
		memcpy(tmp, uriA.portText.first, uriA.portText.afterLast - uriA.portText.first);
		port = tmp;
	}
	else {
		port = toString(80);
	}
	setValue("port", port);

    UriQueryListA * lstQuery = NULL;
    int count = 0;
    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
    m_("count: %d", count);

    std::string path;

  	path = uriA.hostText.first;
  	m_("path: %s", path.c_str());
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
 	   m_("k: [%s], v: [%s]", query->key, query->value);
        if(uriA.query.first) {
        	char tmp[512] = {0};
//    		m_("query first: %s", uriA.query.first);
//    		m_("head first: %s", uriA.pathHead->text.first-1);
    		memcpy(tmp, uriA.pathHead->text.first-1 ,  uriA.query.first - uriA.pathHead->text.first);
            path = tmp;
        }

//        //@description: It is going to be used to compare ==
//        // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//        _baseUri = "http://" + path;
    }


    _baseUri = uri;
	auto at = std::string::npos;
	if((at=_baseUri.find("?")) != std::string::npos) {
		_baseUri = _baseUri.substr(0, at);
	}

    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
    m_("path: %s", path.c_str());

    setValue("path", path);
    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSource::parseForRtpSource() {
	const char* uri = this->uri().c_str();

    UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>\n");
		exit(0);
    }

    // 1. ip
    {
		char tmp[64] = {0};
		memcpy(tmp, uriA.hostText.first, (uriA.hostText.afterLast - uriA.hostText.first));
		std::string ip = tmp;
        setValue("ip", ip);
    }

    std::string port;
	if(uriA.portText.first) {
		char tmp[64] = {0};
		memcpy(tmp, uriA.portText.first, uriA.portText.afterLast - uriA.portText.first);
		port = tmp;
	}
	else {
		port = 80;
	}
	setValue("port", port);

    UriQueryListA * lstQuery = NULL;
    int count = 0;
    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
    m_("count: %d", count);

    std::string path;

  	path = uriA.hostText.first;
  	m_("path: %s", path.c_str());
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
 	   m_("%s, %s", query->key, query->value);
        if(uriA.query.first) {
        	char tmp[512] = {0};
    		m_("query first: %s", uriA.query.first);
    		m_("head first: %s", uriA.pathHead->text.first-1);
    		memcpy(tmp, uriA.pathHead->text.first-1 ,  uriA.query.first - uriA.pathHead->text.first);
            path = tmp;
        }

//        //@description: It is going to be used to compare ==
//        // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//        _baseUri = "http://" + path;
    }


    _baseUri = uri;
	auto at = std::string::npos;
	if((at=_baseUri.find("?")) != std::string::npos) {
		_baseUri = _baseUri.substr(0, at);
	}

    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
    m_("path: %s", path.c_str());

    setValue("path", path);
    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSource::copy(const DataSource &rhs) {
	_uri = rhs._uri;
	_baseUri = rhs._baseUri;
	_protocol = rhs._protocol;
	_properties = rhs._properties;
}

} /* namespace k */

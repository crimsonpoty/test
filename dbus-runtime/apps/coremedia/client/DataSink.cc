/*
 * DataSink.cc
 *
 *  Created on: Aug 28, 2015
 *      Author: buttonfly
 */

#include "../../coremedia/client/include/DataSink.h"

#include <uriparser/Uri.h>
#include <algorithm>
#include "osl.h"
#include "osldbg.h"

#define S_
#define m_
#define Q_

namespace k {

DataSink::DataSink(std::string uri)
: _protocol(DataSink::UNKNOWN)
{
	init(uri);
}

DataSink::DataSink(const char *uri)
: _protocol(DataSink::UNKNOWN)
{
	init(uri);
}

DataSink::DataSink(const DataSink &rhs) {
	copy(rhs);
}

DataSink &DataSink::operator=(const DataSink &rhs) {
	if (this == &rhs) return *this;
	copy(rhs);
	return *this;
}

void DataSink::copy(const DataSink &rhs) {
	_uri = rhs._uri;
	_protocol = rhs._protocol;
	_properties = rhs._properties;
}

DataSink::~DataSink() {
}

void DataSink::init(const char *uri) {
	init(std::string(uri));
}

void DataSink::init(std::string uri) {
	_uri = uri;
	_protocol = protocol();
	parse();
}

const std::string &DataSink::uri() const {
	return _uri;
};

DataSink::SinkProtocol DataSink::protocol() const {
	S_;
	if(_protocol!=DataSink::UNKNOWN) return _protocol;

	else {
		DataSink::SinkProtocol type  = DataSink::UNKNOWN;

	    UriParserStateA state;
	    UriUriA uri;

	    state.uri = &uri;
	    if (uriParseUriA(&state, _uri.c_str()) != URI_SUCCESS) {
	            /* Failure */
			uriFreeUriMembersA(&uri);
			fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
			return DataSink::UNKNOWN;
	    }

	    m_("@@@@@@@@@@@@ %s", _uri.c_str());
		if(memcmp("video", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSink::VIDEO;
	    }
	    else if(memcmp("audio", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSink::AUDIO;
	    }
	    else if(memcmp("file", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSink::FILE;
	    }
	    else if(memcmp("fifo", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSink::FIFO;
	    }
	    else if(memcmp("buf", uri.scheme.first, uri.scheme.afterLast - uri.scheme.first)==0) {
	    	type = DataSink::BUF;
	    }
	    else {
	    	ERR("Unknown sink: %s", _uri.c_str());
	    }

	    KASSERT(type != DataSink::UNKNOWN);
	    uriFreeUriMembersA(&uri);
		return type;
	}
}

bool DataSink::operator == (const DataSink &rhs) const
{
	if(rhs.protocol() != _protocol) {
		return false;
	}
	return (_uri.compare(rhs.uri()) == 0);
}

bool DataSink::operator != (const DataSink &rhs) const
{
	return !operator == (rhs);
}

bool DataSink::operator < (const DataSink &rhs) const
{
	return (_uri.compare(rhs.uri()) < 0);
}

bool DataSink::operator > (const DataSink &rhs) const
{
	return operator < (rhs);
}

bool DataSink::operator <= (const DataSink &rhs) const
{
	return !operator > (rhs);
}

bool DataSink::operator >= (const DataSink &rhs) const
{
	return !operator < (rhs);
}

void DataSink::parse() {
	S_;
	switch(protocol()) {
	case DataSink::FIFO:
		parseForFifoSink();
		break;

	case DataSink::FILE:
		parseForFileSink();
		break;

	case DataSink::BUF:
		parseForBufSink();
		break;

	case DataSink::VIDEO:
		parseForVideoSink();
		break;

	case DataSink::AUDIO:
		parseForAudioSink();
		break;

	default:
		W_("Unknown: %d", protocol());
		break;
	}
}

const std::string &DataSink::valueForKey(std::string key) {
	return _properties[key];
}

void DataSink::setValue(std::string key, std::string value) {
	_properties[key] = value;
}

bool DataSink::hasKey(std::string &key) {
	return !(_properties[key].empty());
}

//void DataSink::parseForRendererSink() {
//	S_;
//	// ex) 'renderer://0' - main
//	// ex) 'renderer://1' - sub
//	KASSERT(1);
//}

void DataSink::parseForFileSink() {
	S_;
	// ex) 'file:///data/record/2011-3010'

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
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
    }

    std::string path;
    if(uriA.query.first) {
    	char tmp[512] = {0};
//		m_("!!%s", uriA.query.first);
//		m_("!!%s", uriA.pathHead->text.first);
		memcpy(tmp, uriA.pathHead->text.first ,  uriA.query.first - uriA.pathHead->text.first - 1);
        path = tmp;
    }
    else
    	path = uriA.pathHead->text.first - 1;

    //@description: It is going to be used to compare ==
    // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//    _baseUri = path;

    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
    m_("path: %s", path.c_str());

    setValue("path", path);

    if(!osl_file_exist(path.c_str())) {
    	W_("No such file: %s", path.c_str());
    }

    char* index = strdup(path.c_str());
    char* ptr = strchr(index, '.');
    if(ptr) strcpy(ptr, ".nav");

    if(osl_file_exist(index)) {
        setValue("index", index);
    }

    if(index)
    	free(index);
    index = NULL;

    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSink::parseForFifoSink() {
	S_;
	// ex) 'fifo:///data/record/2011-3010'

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
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
    }

    std::string path;
    if(uriA.query.first) {
    	char tmp[512] = {0};
//		m_("!!%s", uriA.query.first);
//		m_("!!%s", uriA.pathHead->text.first);
		memcpy(tmp, uriA.pathHead->text.first ,  uriA.query.first - uriA.pathHead->text.first - 1);
        path = tmp;
    }
    else
    	path = uriA.pathHead->text.first - 1;

    //@description: It is going to be used to compare ==
    // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//    _baseUri = path;

    replace_if(path.begin(), path.end(), std::bind2nd(std::equal_to<char>(), '+'), '\ ');
    m_("path: %s", path.c_str());

    setValue("path", path);

    if(!osl_file_exist(path.c_str())) {
    	W_("No such file: %s", path.c_str());
    }

    // xxx.fifompg, xxx.fifonav
    char* index = strdup(path.c_str());
    char* ptr = strchr(index, '.'); // fifompg
    if(ptr) strcpy(ptr, ".fifonav");

    if(osl_file_exist(index)) {
    	setValue("index", index);
    }

    if(index)
    	free(index);
    index = NULL;

    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSink::parseForBufSink() {
	S_;
	const char* uri = this->uri().c_str();
	m_("uri : %s", uri);

	UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
		exit(0);
    }

    //ex) dvb://256/A/350/6900000
    //ex) dvb://256/A/350/6900000?videoPid=33&audioPid=36&pcrPid=33&pmtPid=34&videoCodec=h264&audioCodec=mpeg

    //@deprecated
    // ex) "dvb://256/A/474/6900000/512/650/512/h264/mpeg"
    // ex) "dvb://256/A/474/6900000?videoCodec=h264"

    char tmp[64];
    // 0. QAM mode
    memset(tmp, 0, sizeof(tmp));
    memcpy(tmp, uriA.hostText.first, uriA.hostText.afterLast - uriA.hostText.first);
    m_("qam: %s", tmp);
    setValue("qam", tmp);

   // 1. Annex
//   memset(tmp, 0, sizeof(tmp));
//   UriPathSegmentA* node = uriA.pathHead;
//   KASSERT(node);
//   memcpy(tmp, node->text.first, node->text.afterLast - node->text.first);
//   m_("annex: %s", tmp);
//   setValue("annex", tmp);
//
//   // 2. Freq
//   memset(tmp, 0, sizeof(tmp));
//   node = node->next;
//   KASSERT(node);
//   memcpy(tmp, node->text.first, node->text.afterLast - node->text.first);
//   m_("freq: %s", tmp);
//   setValue("freq", tmp);
//
//   // 3. symbolrate
//   memset(tmp, 0, sizeof(tmp));
//   node = node->next;
//   KASSERT(node);
//   memcpy(tmp, node->text.first, node->text.afterLast - node->text.first);
//   m_("symrate: %s", tmp);
//   setValue("symrate", tmp);
//
//#if 0
//   // 4. video pid;
//   memset(tmp, 0, sizeof(tmp));
//   node = node->next;
//   if(node==NULL) { // scan mode
//	   UriQueryListA * lstQuery = NULL;
//	   int count = 0;
//	   int program = 0;
//	   uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
//	   m_("count: %d", count);
//	   for(UriQueryListA* query = lstQuery; query; query = query->next) {
//		   setValue(query->key, query->value);
//	   }
//	   uriFreeQueryListA(lstQuery);
//
//	   // scan example
////	   dvb://256/B/350350/5360537
//   }
//   else {
//	   W_("deprecated!");
//   }
//#else
//   UriQueryListA * lstQuery = NULL;
//   int count = 0;
//   int program = 0;
//   uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
//   m_("count: %d", count);
//   for(UriQueryListA* query = lstQuery; query; query = query->next) {
//	   setValue(query->key, query->value);
//	   m_("%s, %s", query->key, query->value);
//	   std::string path;
//	   if(uriA.query.first) {
//	   	char tmp[512] = {0};
////			m_("%s", uriA.query.first);
////			m_("%s", uriA.hostText.first);
////			m_("%s", uriA.pathHead->text.first);
//			memcpy(tmp, uriA.hostText.first ,  uriA.query.first - uriA.hostText.first-1);
//	       path = tmp;
//	   }
//	   else path = uriA.pathHead->text.first;
//	   //@description: It is going to be used to compare ==
//	   // for example, in QAM, _baseUri would look like this: 'dvb://256/B/350350/5360537'.
//
//	   _baseUri = "dvb://" + path;
//   }
//   uriFreeQueryListA(lstQuery);
//#endif
	uriFreeUriMembersA(&uriA);
}

void DataSink::parseForVideoSink() {
	S_;
	const char* uri = this->uri().c_str();
	m_("uri : %s", uri);

	UriParserStateA state;
    UriUriA uriA;

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
		exit(0);
    }

    char tmp[64];
    // 0. display mode : 0 , 1
    memset(tmp, 0, sizeof(tmp));
    memcpy(tmp, uriA.hostText.first, uriA.hostText.afterLast - uriA.hostText.first);
    m_("display: %s", tmp);
    setValue("display", tmp);

    int count = 0;
    int program = 0;
    UriQueryListA * lstQuery = NULL;
    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
    m_("count: %d", count);

    for(UriQueryListA* query = lstQuery; query; query = query->next) {
    	setValue(query->key, query->value);
    }

    uriFreeQueryListA(lstQuery);
    uriFreeUriMembersA(&uriA);
}

void DataSink::parseForAudioSink() {
	S_;
}


} /* namespace k */

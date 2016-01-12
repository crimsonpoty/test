/*
 * HttpQuery.h
 *
 *  Created on: Oct 16, 2015
 *      Author: buttonfly
 */

#ifndef LIBWEBSERVER_HTTPQUERY_H_
#define LIBWEBSERVER_HTTPQUERY_H_

#include <string>
#include <map>
#include <json/json.h>
#include "mgsocket.h"

#ifndef YES
#define YES true
#endif

#ifndef NO
#define NO false
#endif

#define OK		0
#define NOT_OK	-1

namespace k {

class HttpQuery {
public:
	HttpQuery(const char* query);
	virtual ~HttpQuery();

	auto parse() -> bool;
	auto toJson() -> std::string;

	static auto REST_parse(Json::Value &root, struct mg_connection * conn) -> bool;
	static auto REST_response(Json::Value &res, struct mg_connection * conn) -> int;

private:
	typedef std::map<std::string, std::string> Dictionary;
	Dictionary _kv;
	const char *_query;
};

} /* namespace k */

#endif /* LIBWEBSERVER_HTTPQUERY_H_ */

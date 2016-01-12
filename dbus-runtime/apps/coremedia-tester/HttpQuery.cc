/*
 * HttpQuery.cc
 *
 *  Created on: Oct 16, 2015
 *      Author: buttonfly
 */

#include "HttpQuery.h"
#include <uriparser/Uri.h>
#include "osldbg.h"
#include <string>

#define m_
#define S_


namespace k {

HttpQuery::HttpQuery(const char* query)
: _query(query)
{

}

HttpQuery::~HttpQuery() {

}

auto HttpQuery::parse() -> bool {

	std::string tmp("urn://tmp?");
	tmp.append(_query);

	UriParserStateA state;
    UriUriA uriA;

    auto uri = tmp.c_str();

    state.uri = &uriA;
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
            /* Failure */
		uriFreeUriMembersA(&uriA);
		fprintf(stderr, "failed to <uriParseUriA>: %s\n", uri);
//		exit(0);
		return NO;
    }

    UriQueryListA * lstQuery = NULL;
    int count = 0;
    int program = 0;
    uriDissectQueryMallocA(&lstQuery, &count, uriA.query.first, uriA.query.afterLast);
    m_("count: %d", count);
    for(UriQueryListA* query = lstQuery; query; query = query->next) {
 	   _kv[query->key] = query->value;
 	   m_("%s, %s", query->key, query->value);
    }
    uriFreeQueryListA(lstQuery);
 	uriFreeUriMembersA(&uriA);
 	return YES;
}

auto HttpQuery::toJson() -> std::string {
	std::string fmt("{");
	for(auto pos = _kv.begin(); pos != _kv.end(); ++pos) {
		if(pos != _kv.begin()) fmt.append(",");
		fmt.append("\"");
		fmt.append(pos->first);
		fmt.append("\"");

		fmt.append(":");

		fmt.append("\"");
		fmt.append(pos->second);
		fmt.append("\"");
	}
	fmt.append("}");
	return fmt;
}

auto HttpQuery::REST_parse(Json::Value &root, struct mg_connection * conn) -> bool {
	S_;
	std::string msg;

//	conn->request_method //GET, POST...
	m_("method: %s", conn->request_method);
	m_("query: %s", conn->query_string);
	m_("content: %s", conn->content);

	std::string method(conn->request_method);
	if(method.compare("GET")==0) {
		if(conn->query_string) {
			char  buf[2048] = {0};
			mg_url_decode((const char*) conn->query_string, strlen(conn->query_string), buf, sizeof(buf), 0);
			msg = buf;
			HttpQuery query(buf);
			query.parse();
			msg = query.toJson();
		}
	}
	else {
		msg = conn->content;
	}

	auto at = msg.rfind("}");
	if(at == std::string::npos) {
		return "error";
	}

	msg = msg.substr(0, at+1);
	m_("msg: %s", msg.c_str());

	Json::Reader reader;
	return reader.parse(msg, root);
}

auto HttpQuery::REST_response(Json::Value &res, struct mg_connection * conn) -> int {
	S_;
	Json::FastWriter writer;
	auto body = writer.write(res);

	k::mgsocket sck(conn);
	auto v = sck.get("id");

	sck.set_header("Access-Control-Allow-Origin", "*");
	return sck.write(body);
}

} /* namespace k */

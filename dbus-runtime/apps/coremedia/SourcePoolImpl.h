/*
 * SourcePoolImpl.h
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_SOURCEPOOLIMPL_H_
#define APPS_COREMEDIA_SOURCEPOOLIMPL_H_

extern "C" {
#include "gdbus/gdbus-Coremedia-generated.h"
}

#include "SourceImpl.h"
#include <map>
typedef std::map<std::string, k::SourceImpl *> SourceDictionary;

namespace k {

class SourcePoolImpl {
public:
	static SourcePoolImpl &shared() {
		static SourcePoolImpl inst;
		return inst;
	}

private:
	static gboolean on_request_source (
		    CoremediaSourcePool *object,
		    GDBusMethodInvocation *invocation,
			const gchar *arg_dataSource,
			gpointer user_data);

	static gboolean on_release_source (
		    CoremediaSourcePool *object,
		    GDBusMethodInvocation *invocation,
			const gchar *arg_dataSource,
			gpointer user_data);

private:
	SourcePoolImpl();
	virtual ~SourcePoolImpl();

private:
	SourceDictionary _dictionary;
};

} /* namespace k */

#endif /* APPS_COREMEDIA_SOURCEPOOLIMPL_H_ */

/*
 * SourceImpl.h
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#ifndef APPS_COREMEDIA_SOURCEIMPL_H_
#define APPS_COREMEDIA_SOURCEIMPL_H_

extern "C" {
#include "gdbus/gdbus-Coremedia-generated.h"
}
#include <string>

namespace k {

class SourcePoolImpl;
class SourceImpl {
public:
	friend SourcePoolImpl;
	SourceImpl(CoremediaSourcePool * object, GDBusMethodInvocation *invocation, const char *dataSource);
	virtual ~SourceImpl();

	void setDataSource(std::string DataSource);
	auto parseDataSource()->int;

private:
	static gboolean on_source_prepare (
			CoremediaSource *object,
			GDBusMethodInvocation *invocation, gpointer user_data);
	static gboolean on_source_start (
			CoremediaSource *object,
			GDBusMethodInvocation *invocation, gpointer user_data);
	static gboolean on_source_stop (
			CoremediaSource *object,
			GDBusMethodInvocation *invocation, gpointer user_data);
	static gboolean on_source_request_pid_channel (
			CoremediaSource *object,
			GDBusMethodInvocation *invocation, gpointer user_data) ;
	static gboolean on_source_release_pid_channel (
			CoremediaSource *object,
			GDBusMethodInvocation *invocation, gpointer user_data) ;

private:
private:
	static void signalReceived(void *manager, void *objProxy, void *ifProxy, const char *sendName, const char *signalName, void *parameters, void *userdata);
	static void propertiesChanged(void *manager, void *objProxy, void *ifProxy, void *changedProperties, const char * const *invalidatedProperties, void *userdata);


	int ref() {
		return ++_ref;
	}
	int unref() {
		return --_ref;
	}
	int _ref;
};

} /* namespace k */

#endif /* APPS_COREMEDIA_SOURCEIMPL_H_ */

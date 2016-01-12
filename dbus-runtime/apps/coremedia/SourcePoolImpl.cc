/*
 * SourcePoolImpl.cc
 *
 *  Created on: Dec 18, 2015
 *      Author: buttonfly
 */

#include "SourcePoolImpl.h"
#include "DBusObjectManagerServer.h"
#include <assert.h>
#include "def.h"
#include "osldbg.h"
#include <string>
#include <algorithm>
#include  <functional>

#include "SourceImpl.h"

namespace k {

SourcePoolImpl::SourcePoolImpl() {
	auto server = DBusObjectManagerServer::shared().server();
	assert(server);

	auto object = coremedia_object_skeleton_new(GDBUS_COREMEDIA_SOURCE_POOL_OBJECT_PATH);
	assert(object);

	auto skeleton = coremedia_source_pool_skeleton_new ();
	coremedia_object_skeleton_set_source_pool(object, skeleton);
	g_object_unref(skeleton);

	g_signal_connect(skeleton, "handle-request",	G_CALLBACK(on_request_source), this); /* user_data */
	g_signal_connect(skeleton, "handle-release",	G_CALLBACK(on_release_source), this); /* user_data */

	g_dbus_object_manager_server_export(server, G_DBUS_OBJECT_SKELETON(object));
	g_object_unref(object);
}

SourcePoolImpl::~SourcePoolImpl() {
	// TODO Auto-generated destructor stub
}


gboolean SourcePoolImpl::on_request_source (
	    CoremediaSourcePool *object,
	    GDBusMethodInvocation *invocation,
		const gchar *dataSource,
		gpointer user_data) {
	S_;
	m_("%s", dataSource);
	SourcePoolImpl *self = (SourcePoolImpl *) user_data;

	// TODO:
	k::SourceImpl *source = self->_dictionary[dataSource];
	if(source) {
		int ref = source->ref();
		m_("@@@@@@@@@ current ref.: %d", ref);
//		return source;
		coremedia_source_pool_complete_request(object, invocation);
		goto out;
	}

	source = new SourceImpl(object, invocation, dataSource);

	if(source) {
		source->setDataSource(dataSource);
		source->parseDataSource();
	}
	//TODO:  add to pool
	// We might not need to use this map. because every single Source has a dataSource in itself. (Source::setDataSource)
	self->_dictionary[dataSource] = source;

	coremedia_source_pool_complete_request(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

gboolean SourcePoolImpl::on_release_source (
	    CoremediaSourcePool *object,
	    GDBusMethodInvocation *invocation,
		const gchar *arg_dataSource,
		gpointer user_data) {
	S_;
	m_("%s", arg_dataSource);

	// TODO:
	// if we already have one.
	// if there is no such instance.
	coremedia_source_pool_complete_release(object, invocation);
	goto out;
	g_assert_not_reached();
	out: return TRUE; /* to indicate that the method was handled */
}

} /* namespace k */

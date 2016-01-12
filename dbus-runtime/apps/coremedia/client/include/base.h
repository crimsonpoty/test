/*
 * base.h
 *
 *  Created on: Sep 4, 2015
 *      Author: buttonfly
 */

#ifndef COREMEDIA_BASE_H_
#define COREMEDIA_BASE_H_

#include <stdint.h>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>

#include "type.h"
#include "osldbg.h"

namespace k {

typedef std::map<std::string, std::string> Properties;

template <typename T>
static inline std::string toString(T value) {
	std::ostringstream stm ;
	stm << value ;
	return stm.str();
}

#if 1 // It is already supported in c++11
static inline int stoi(const std::string &value) {
	return atoi(value.c_str());
}
#endif

typedef enum {
	NullStatus =0,
	PreparedStatus,
	StartedStatus,
	PausedStatus
} Status;


class BufferSinkDelegate {
public:
	// LOWORD | HIWORD
//	uint32_t id = MAKELONG(pidChannel->pid(), tableId);
	virtual void dataReceived(uint32_t id, uint8_t *buf, uint32_t len)=0;
};

class SourceDelegate {
public:
//	virtual void tuned(Properties *properties)=0;
//	virtual void untuned()=0;
//	virtual void noSignal()=0;
	virtual void lockChanged(std::string status, Properties *properties=NULL)=0;
};

//@deprecated
//class DataSource;
class ServiceScanDelegate {
public:
	virtual void serviceScanStarted()=0;
	virtual void serviceFound(std::string uri)=0;
	virtual void serviceScanCompleted()=0;
};
//-----------------------------------------------------------------------------------

class SinkDelegate {
public:
};

class Source;
class QamSourceImpl;
class FileSourceImpl;
class HttpSourceImpl;
class IpSourceImpl;
class RTPSourceImpl;
class SourceImpl;
class FifoSourceImpl;
class PidChannel {
public:
	friend class Source;
	friend class QamSourceImpl;
	friend class HttpSourceImpl;
	friend class FileSourceImpl;
	friend class SourceImpl;
	friend class IpSourceImpl;
	friend class RTPSourceImpl;
	friend class FifoSourceImpl;

	void setNativeHandle(NativeHandle nativeHandle) {
		_nativeHandle = nativeHandle;
	}
	NativeHandle nativeHandle() {
		return _nativeHandle;
	}

	// "videoCodec", "h264"
	// "audioCodec", "mp3"
	void setValue(std::string key, std::string value) {
		_properties[key] = value;
	}

	auto valueForKey(std::string key) -> std::string {
		return _properties[key];
	}

	auto pid() -> uint16_t{
		return _pid;
	}

private:
	PidChannel(NativeHandle nativeHandle, uint16_t pid) : _ref(1), _nativeHandle(nativeHandle), _pid(pid){
	}
	virtual ~PidChannel() {
	}

private:
	int ref() {
		return ++_ref;
	}
	int unref() {
		return --_ref;
	}

private:
	NativeHandle _nativeHandle;
	int _ref;

	Properties _properties;
	uint16_t _pid;
};

class SourceProtocol {
public:
	virtual PidChannel *requestPidChannel(uint16_t pid)=0;
	virtual void releasePidChannel(PidChannel *handle)=0;
	virtual int prepare()=0;
	virtual int start()=0;
	virtual int stop()=0;
	virtual bool isPrepared()=0;
	virtual NativeHandle nativeHandle()=0;
#if INTERNAL_SCAN_SUPPORT
	virtual std::string scan()=0;
	virtual int invokeScan(ServiceScanDelegate *delegate)=0;
#endif
};

class SinkProtocol {
public:
	virtual int prepare()=0;
	virtual int start()=0;
	virtual int stop()=0;
	virtual void addPidChannel(PidChannel *pidChannel);

	virtual void setValue(std::string key, std::string value)=0;
	virtual const std::string &valueForKey(std::string key)=0;
};

class SectionFilterProtocol {
public:
	virtual int prepare()=0;
	virtual int start()=0;
	virtual int stop()=0;
	virtual void setPidChannel(PidChannel *pidChannel)=0;
	virtual PidChannel *pidChannel()=0;
	virtual int read(uint8_t *buf, uint32_t len, uint32_t timeout)=0;
};

} /* namespace k */

#endif /* COREMEDIA_BASE_H_ */

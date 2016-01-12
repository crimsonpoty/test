/*
 * Dictionary.h
 *
 *  Created on: Nov 13, 2015
 *      Author: buttonfly
 */

#ifndef LIBUT___KOBJECT_H_
#define LIBUT___KOBJECT_H_

#include <string>
#include <map>
#include <list>
#include "json/json.h"

namespace k {

class Observer {
public:
	virtual void willSet(const std::string &key, const Json::Value &from, const Json::Value &to) {}
	virtual void didSet(const std::string &key, const Json::Value &from, const Json::Value &to) {}
};

class Object {
public:
	Object();
	virtual ~Object();

	bool hasKeyPath(std::string key);
	bool hasKeyPath(std::string key) const;
	virtual void setValueForKeyPath(const char *key, std::string &value);
	virtual void setValueForKeyPath(const char *key, const char *value);
	virtual void setValueForKeyPath(const char *key, int value);
	virtual void setValueForKeyPath(const char *key, unsigned int value);
	virtual void setValueForKeyPath(const char *key, const Json::Value &value);

	virtual void setValueForKeyPath(std::string &key, std::string &value);
	virtual void setValueForKeyPath(std::string &key, const char *value);
	virtual void setValueForKeyPath(std::string &key, int value);
	virtual void setValueForKeyPath(std::string &key, unsigned int value);
	virtual void setValueForKeyPath(std::string &key, const Json::Value &value);
	virtual const Json::Value &valueForKeyPath(std::string key);
	void clear();
	// Its type must be an array.
	Json::Value keys();
//	std::list<std::string> keys();

	virtual void addObserverForKeyPath(std::string key, k::Observer *o);
	virtual void removeObserverForKeyPath(std::string key, k::Observer *o);

//	void loadFromJson(Json::Value &value);
//	std::string toJson();

	static void printJson( const Json::Value &val );
	static bool printJsonTree( const Json::Value &root, unsigned short depth = 0 );

protected:
	void fireDidSet(std::string &key, const Json::Value &from, const Json::Value &to);
	void fireWillSet(std::string &key, const Json::Value &from, const Json::Value &to);

private:
//	std::map<std::string, Json::Value> _kv;
	Json::Value _kv;

	typedef std::list<k::Observer *> ObserverList;
	std::map<std::string, ObserverList> _observers;
};

} /* namespace k */

#endif /* LIBUT___KOBJECT_H_ */

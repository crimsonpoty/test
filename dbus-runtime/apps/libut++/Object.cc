/*
 * Dictionary.cc
 *
 *  Created on: Nov 13, 2015
 *      Author: buttonfly
 */

#include "include/Object.h"

#include <stdio.h>
#include <algorithm>

namespace k {

Object::Object() {
	// TODO Auto-generated constructor stub

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

bool Object::hasKeyPath(std::string key) {
	const Json::Value &value = _kv[key];
	return (value.empty()==false);
}

bool Object::hasKeyPath(std::string key) const{
	const Json::Value &value = _kv[key];
	return (value.empty()==false);
}

void Object::setValueForKeyPath(const char *key, std::string &value) {
	std::string localKey(key);
	setValueForKeyPath(localKey, value);
}

void Object::setValueForKeyPath(const char *key, const char *value) {
	std::string localKey(key);
	setValueForKeyPath(localKey, value);
}

void Object::setValueForKeyPath(const char *key, int value) {
	std::string localKey(key);
	setValueForKeyPath(localKey, value);
}

void Object::setValueForKeyPath(const char *key, unsigned int value) {
	std::string localKey(key);
	setValueForKeyPath(localKey, value);
}

void Object::setValueForKeyPath(const char *key, const Json::Value &value) {
	std::string localKey(key);
	setValueForKeyPath(localKey, value);
}

void Object::setValueForKeyPath(std::string &key, std::string &value) {
	Json::Value localValue = value;
	setValueForKeyPath(key, localValue);
}

void Object::setValueForKeyPath(std::string &key, const char *value) {
	Json::Value localValue = std::string(value);
	setValueForKeyPath(key, localValue);
}

void Object::setValueForKeyPath(std::string &key, unsigned int value) {
	Json::Value localValue = value;
	setValueForKeyPath(key, localValue);
}

void Object::setValueForKeyPath(std::string &key, int value) {
	Json::Value localValue = value;
	setValueForKeyPath(key, localValue);
}

void Object::setValueForKeyPath(std::string &key, const Json::Value &value) {
	Json::Value old = _kv[key];
	fireWillSet(key, old, value);
	_kv[key] = value;
	fireDidSet(key, old, _kv[key]);
}

const Json::Value &Object::valueForKeyPath(std::string key) {
	return _kv[key];
}

void Object::addObserverForKeyPath(std::string key, k::Observer *o) {
	ObserverList &lst = _observers[key];
	lst.push_back(o);
}

void Object::removeObserverForKeyPath(std::string key, k::Observer *o) {
	ObserverList &lst = _observers[key];
	lst.remove(o);
}

void Object::clear() {
	_kv.clear();
}

Json::Value Object::keys() {
	Json::Value keys;
	for(auto pos = keys.begin(); pos != keys.end(); ++pos) {
		keys.append(pos.key());
	}
	return keys;
}

void Object::fireDidSet(std::string &key, const Json::Value &from, const Json::Value &to) {
//	ObserverList &lst = _observers[key];
//	for(ObserverList::iterator pos = lst.begin(); pos != lst.end(); ++pos)
//		(*pos)->willSet(key, from, to);
//
//	ObserverList &lst2 = _observers["*"];
//	for(ObserverList::iterator pos = lst2.begin(); pos != lst2.end(); ++pos)
//		(*pos)->willSet(key, from, to);
//	for_each(_observers[key].begin(), _observers[key].end(), [] (std::string &key, const Json::Value &from, const Json::Value &to){
//
//	});
	for(auto &pos : _observers[key])	pos->willSet(key, from, to);
	for(auto &pos :_observers["*"])		pos->willSet(key, from, to);
}

void Object::fireWillSet(std::string &key, const Json::Value &from, const Json::Value &to) {
//	ObserverList &lst = _observers[key];
//	for(ObserverList::iterator pos = lst.begin(); pos != lst.end(); ++pos)
//		(*pos)->didSet(key, from, to);
	for(auto &pos : _observers[key])	pos->didSet(key, from, to);
	for(auto &pos :_observers["*"])		pos->didSet(key, from, to);
}

//void KObject::loadFromJson(Json::Value &value) {
//	//TODO:
////	Json::Reader reader;
////	reader.parse(_kv, value);
//}
//
//std::string KObject::toJson() {
//	// TODO
////	Json::FastWriter writer;
////	return writer.write(_kv);
//	return std::string();
//}

void Object::printJson( const Json::Value &val ) {
    if( val.isString() ) {
    	printf( "string(%s)", val.asString().c_str() );
    }
    else if( val.isBool() ) {
    	printf( "bool(%d)", val.asBool() );
    }
    else if( val.isInt() ) {
    	printf( "int(%d)", val.asInt() );
    }
    else if( val.isUInt() ) {
    	printf( "uint(%u)", val.asUInt() );
    }
    else if( val.isDouble() ) {
    	printf( "double(%f)", val.asDouble() );
    }
    else {
        printf( "unknown type=[%d]", val.type() );
    }
}

bool Object::printJsonTree( const Json::Value &root, unsigned short depth /* = 0 */) {
	depth += 1;
    printf( " {type=[%d], size=%d}", root.type(), root.size() );

    if( root.size() > 0 ) {
        printf("\n");
        for(auto itr = root.begin() ; itr != root.end() ; ++itr ) {
//        for(auto &itr : root) {
            // Print depth.
            for(int tab = 0 ; tab < depth; ++tab) {
               printf("-");
            }
            printf(" subvalue(");
            printJson(itr.key());
            printf(") -");
            printJsonTree(*itr, depth);
        }
        return true;
    }
    else {
        printf(" ");
        printJson(root);
        printf( "\n" );
    }
    return true;
}

} /* namespace k */

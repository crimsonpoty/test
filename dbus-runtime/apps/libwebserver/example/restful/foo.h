/*
 * foo.h
 *
 *  Created on: Feb 27, 2015
 *      Author: buttonfly
 */

#ifndef APPS_LIBMANCAVE_API_FOO_H_
#define APPS_LIBMANCAVE_API_FOO_H_

#include "httpservice.h"

namespace k {

class foo: public httpservice {
public:
	foo();
	virtual ~foo();

	virtual int bind(std::map<std::string, httpservice*>& handler);
	virtual int unbind(std::map<std::string, httpservice*>& handler);
	int invoke(void* conn);
};

} /* namespace k */

#endif /* APPS_LIBMANCAVE_API_FOO_H_ */

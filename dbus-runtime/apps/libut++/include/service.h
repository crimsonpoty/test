/*
 * service.h
 *
 *  Created on: May 31, 2013
 *      Author: buttonfly
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include "runnable.h"

namespace k {

class service : public k::runnable {
public:
	virtual const char* get_id()=0;
};

} /* namespace k */
#endif /* SERVICE_H_ */

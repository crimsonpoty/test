/*
 * PSIP_ComponentNameDescriptor.h
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#ifndef PSIP_COMPONENTNAMEDESCRIPTOR_H_
#define PSIP_COMPONENTNAMEDESCRIPTOR_H_

#include "PSIP_Descriptor.h"
#include "PSIP_MultipleStringStructure.h"

class PSIP_ComponentNameDescriptor : public PSIP_Descriptor, public PSIP_MultipleStringStructure
{
protected:
	PSIP_ComponentNameDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_ComponentNameDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	virtual void dump();

};

#endif /* PSIP_COMPONENTNAMEDESCRIPTOR_H_ */

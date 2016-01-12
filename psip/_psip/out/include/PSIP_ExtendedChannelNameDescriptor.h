/*
 * PSIP_ExtendedChannelNameDescriptor.h
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#ifndef PSIP_EXTENDEDCHANNELNAMEDESCRIPTOR_H_
#define PSIP_EXTENDEDCHANNELNAMEDESCRIPTOR_H_

#include "PSIP_Descriptor.h"
#include "PSIP_MultipleStringStructure.h"

class PSIP_ExtendedChannelNameDescriptor : public PSIP_Descriptor, public PSIP_MultipleStringStructure
{
protected:
	PSIP_ExtendedChannelNameDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_ExtendedChannelNameDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	virtual void dump();
};

#endif /* PSIP_EXTENDEDCHANNELNAMEDESCRIPTOR_H_ */

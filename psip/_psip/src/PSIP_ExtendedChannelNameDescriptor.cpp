/*
 * PSIP_ExtendedChannelNameDescriptor.cpp
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_ExtendedChannelNameDescriptor.h"

PSIP_ExtendedChannelNameDescriptor::PSIP_ExtendedChannelNameDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , PSIP_MultipleStringStructure((0 < descriptorLength ? &buffer[2] : buffer), descriptorLength)
{

}

PSIP_ExtendedChannelNameDescriptor::~PSIP_ExtendedChannelNameDescriptor()
{

}

void PSIP_ExtendedChannelNameDescriptor::dump()
{
	printf("		[ -> PSIP_ExtendedChannelNameDescriptor ]\n");
	PSIP_MultipleStringStructure::dump();
	printf("		[ <- PSIP_ServiceLocationDescriptor ]\n");
}

/*
 * PSIP_ComponentNameDescriptor.cpp
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#include "PSIP_ComponentNameDescriptor.h"

PSIP_ComponentNameDescriptor::PSIP_ComponentNameDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , PSIP_MultipleStringStructure((0 < descriptorLength ? &buffer[2] : buffer), descriptorLength)
{

}

PSIP_ComponentNameDescriptor::~PSIP_ComponentNameDescriptor()
{

}

void PSIP_ComponentNameDescriptor::dump()
{
	printf("		[ -> PSIP_ComponentNameDescriptor ]\n");
	PSIP_MultipleStringStructure::dump();
	printf("		[ <- PSIP_ComponentNameDescriptor ]\n");
}

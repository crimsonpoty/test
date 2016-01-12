/*
 * PSIP_ServiceLocationDescriptor.cpp
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_ServiceLocationDescriptor.h"


PSIP_ServiceLocationElement::PSIP_ServiceLocationElement()
 : stream_type(0)
 , elementary_PID(0)
{
	memset(&iso_639_language_code[0], 0x00, 4);
}

PSIP_ServiceLocationElement::~PSIP_ServiceLocationElement()
{

}

uint8_t	PSIP_ServiceLocationElement::getStreamType() const
{
	return stream_type;
}

uint16_t PSIP_ServiceLocationElement::getElementaryPID() const
{
	return elementary_PID;
}

const uint8_t* PSIP_ServiceLocationElement::getIso639LanguageCode() const
{
	return &iso_639_language_code[0];
}

void PSIP_ServiceLocationElement::dump()
{
	printf("			[ -> PSIP_ServiceLocationElement ]\n");
	printf("				stream_type = 0x%X\n", stream_type);
	printf("				elementary_PID = 0x%X\n", elementary_PID);
	printf("				iso_639_language_code = %s\n", &iso_639_language_code[0]);
	printf("			[ <- PSIP_ServiceLocationElement ]\n");
}

PSIP_ServiceLocationDescriptor::PSIP_ServiceLocationDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , PCR_PID(0)
 , number_elements(0)
{
	if ( 2 < descriptorLength ) {
		int index = 2;
		PCR_PID = (buffer[index++] & 0x1F) << 8;
		PCR_PID |= buffer[index++] & 0xFF;
		number_elements = buffer[index++] & 0xFF;
		for ( int i = 0; i < number_elements; i++ ) {
			PSIP_ServiceLocationElement* _element = new PSIP_ServiceLocationElement;
			assert(NULL != _element);
			_element->stream_type = buffer[index++] & 0xFF;
			_element->elementary_PID = (buffer[index++] & 0x1F) << 8;
			_element->elementary_PID |= buffer[index++] & 0xFF;
			_element->iso_639_language_code[0] = buffer[index++] & 0xFF;
			_element->iso_639_language_code[1] = buffer[index++] & 0xFF;
			_element->iso_639_language_code[2] = buffer[index++] & 0xFF;
			mPSIP_ServiceLocationElementList.push_back(_element);
		}
	}
}

PSIP_ServiceLocationDescriptor::~PSIP_ServiceLocationDescriptor()
{
	PSIP_ServiceLocationElementList::iterator it;
	for ( it = mPSIP_ServiceLocationElementList.begin(); it != mPSIP_ServiceLocationElementList.end(); ++it ) {
		delete *it;
	}
}

uint16_t PSIP_ServiceLocationDescriptor::getPcrPid() const
{
	return PCR_PID;
}

const PSIP_ServiceLocationElementList* PSIP_ServiceLocationDescriptor::getServiceLocationElementList() const
{
	return &mPSIP_ServiceLocationElementList;
}

void PSIP_ServiceLocationDescriptor::dump()
{
	printf("		[ -> PSIP_ServiceLocationDescriptor ]\n");
	printf("			PCR_PID = 0x%X\n", PCR_PID);
	printf("			number_elements = %d\n", number_elements);
	for ( int i = 0; i < number_elements; i++ ) {
		mPSIP_ServiceLocationElementList[i]->dump();
	}
	printf("		[ <- PSIP_ServiceLocationDescriptor ]\n");
}

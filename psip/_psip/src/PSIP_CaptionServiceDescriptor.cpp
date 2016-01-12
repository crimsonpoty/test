/*
 * PSIP_CaptionServiceDescriptor.cpp
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_CaptionServiceDescriptor.h"

PSIP_CaptionServiceInfo::PSIP_CaptionServiceInfo()
 : cc_type(0)
 , caption_service_number(0)
 , easy_reader(0)
 , wide_aspect_ratio(0)
{
	memset(&iso_639_language_code[0], 0x00, 4);
}

PSIP_CaptionServiceInfo::~PSIP_CaptionServiceInfo()
{

}

const uint8_t* PSIP_CaptionServiceInfo::getIso639LanguageCode() const
{
	return &iso_639_language_code[0];
}

uint8_t	PSIP_CaptionServiceInfo::getCCtype() const
{
	return cc_type;
}

uint8_t	PSIP_CaptionServiceInfo::getLine21Field() const
{
	if ( 0 == cc_type ) {
		return (caption_service_number & 0x01);
	}

	return 0;
}

uint8_t	PSIP_CaptionServiceInfo::getCaptionServiceNumber() const
{
	return caption_service_number;
}

uint8_t	PSIP_CaptionServiceInfo::getEasyReader() const
{
	return easy_reader;
}

uint8_t	PSIP_CaptionServiceInfo::getWideAspectRatio() const
{
	return wide_aspect_ratio;
}

void PSIP_CaptionServiceInfo::dump()
{
	printf("			[ -> PSIP_CaptionServiceInfo ]\n");
	printf("				iso_639_language_code = %s\n", &iso_639_language_code[0]);
	printf("				cc_type = 0x%X\n", cc_type);
	if ( 0 == cc_type ) {
		printf("				line21_field = 0x%X\n", (caption_service_number & 0x01));
	}
	else {
		printf("				caption_service_number = 0x%X\n", caption_service_number);
	}
	printf("				easy_reader = 0x%X\n", easy_reader);
	printf("				wide_aspect_ratio = 0x%X\n", wide_aspect_ratio);
	printf("			[ <- PSIP_CaptionServiceInfo ]\n");
}


PSIP_CaptionServiceDescriptor::PSIP_CaptionServiceDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , number_of_services(0)
{
	if ( 2 < descriptorLength ) {
		int index = 2;
		number_of_services = buffer[index++] & 0x1F;
		for ( int i = 0; i < number_of_services; i++ ) {
			PSIP_CaptionServiceInfo* _info = new PSIP_CaptionServiceInfo;
			assert(NULL != _info);
			_info->iso_639_language_code[0] = buffer[index++] & 0xFF;
			_info->iso_639_language_code[1] = buffer[index++] & 0xFF;
			_info->iso_639_language_code[2] = buffer[index++] & 0xFF;
			_info->cc_type = (buffer[index] & 0x80) >> 7;
			_info->caption_service_number = buffer[index++] & 0x3F;
			_info->easy_reader = (buffer[index] & 0x80) >> 7;
			_info->wide_aspect_ratio = (buffer[index++] & 0x40) >> 6;
			index++;	// skip reserved
			mPSIP_CaptionServiceInfoList.push_back(_info);
		}
	}
}

PSIP_CaptionServiceDescriptor::~PSIP_CaptionServiceDescriptor()
{
	PSIP_CaptionServiceInfoList::iterator it;
	for ( it = mPSIP_CaptionServiceInfoList.begin(); it != mPSIP_CaptionServiceInfoList.end(); ++it ) {
		delete *it;
	}
}

const PSIP_CaptionServiceInfoList* PSIP_CaptionServiceDescriptor::getCaptionServiceInfo() const
{
	return &mPSIP_CaptionServiceInfoList;
}

void PSIP_CaptionServiceDescriptor::dump()
{
	printf("		[ -> PSIP_CaptionServiceDescriptor ]\n");
	printf("			number_of_services = 0x%X\n", number_of_services);
	for ( int i = 0; i < number_of_services; i++ ) {
		mPSIP_CaptionServiceInfoList[i]->dump();
	}
	printf("		[ <- PSIP_CaptionServiceDescriptor ]\n");
}

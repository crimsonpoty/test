/*
 * PSIP_AC3AudioDescriptor.cpp
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#include "PSIP_AC3AudioDescriptor.h"

PSIP_AC3AudioDescriptor::PSIP_AC3AudioDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , sample_rate_code(0)
 , bsid(0)
 , bit_rate_code(0)
 , surround_mode(0)
 , bsmode(0)
 , num_channels(0)
 , full_svc(0)
 , langcod(0)
 , langcod2(0)
 , asvcflags(0)
 , textlen(0)
 , textcode(0)
 , language_flag(0)
 , language_flag_2(0)
{
	memset(&iso_639_language_code[0], 0x00, 4);

	if ( 6 < descriptorLength ) {
		int index = 2;
		sample_rate_code = (buffer[index] & 0xE0) >> 5;
		bsid = buffer[index++] & 0x1F;
		bit_rate_code = (buffer[index] & 0xFC) >> 2;
		surround_mode = buffer[index++] & 0x03;
		bsmode = (buffer[index] & 0xE0) >> 5;
		num_channels = (buffer[index] & 0x1E) >> 1;
		full_svc = buffer[index++] & 0x01;
		langcod = buffer[index++] & 0xFF;
		if ( 0 == num_channels ) {
			langcod2 = buffer[index++] & 0xFF;
		}
		asvcflags = buffer[index++] & 0xFF;
		textlen = (buffer[index] & 0xFE) >> 1;
		textcode = buffer[index++] & 0x01;
		language_flag = (buffer[index] & 0x80) >> 7;
		language_flag_2 = (buffer[index++] & 0x40) >> 6;
		if ( 1 == language_flag ) {
			iso_639_language_code[0] = buffer[index++] & 0xFF;
			iso_639_language_code[1] = buffer[index++] & 0xFF;
			iso_639_language_code[2] = buffer[index++] & 0xFF;
		}
	}
}

PSIP_AC3AudioDescriptor::~PSIP_AC3AudioDescriptor()
{

}

uint8_t	PSIP_AC3AudioDescriptor::getSampleRateCode() const
{
	return sample_rate_code;
}

uint8_t	PSIP_AC3AudioDescriptor::getBsid() const
{
	return bsid;
}

uint8_t	PSIP_AC3AudioDescriptor::getBitRateCode() const
{
	return bit_rate_code;
}

uint8_t	PSIP_AC3AudioDescriptor::getSurroundMode() const
{
	return surround_mode;
}

uint8_t	PSIP_AC3AudioDescriptor::getBsmode() const
{
	return bsmode;
}

uint8_t	PSIP_AC3AudioDescriptor::getNumChannels() const
{
	return num_channels;
}

uint8_t	PSIP_AC3AudioDescriptor::getFullSvc() const
{
	return full_svc;
}

uint8_t	PSIP_AC3AudioDescriptor::getLangCod() const
{
	return langcod;
}

uint8_t	PSIP_AC3AudioDescriptor::getLangCod2() const
{
	return langcod2;
}

uint8_t	PSIP_AC3AudioDescriptor::getMainId() const
{
	return ((asvcflags & 0xE0) >> 5);
}

uint8_t	PSIP_AC3AudioDescriptor::getPriority() const
{
	return ((asvcflags & 0x18) >> 3);
}

uint8_t	PSIP_AC3AudioDescriptor::getAsvcFlags() const
{
	return asvcflags;
}

uint8_t	PSIP_AC3AudioDescriptor::getTextLenth() const
{
	return textlen;
}

uint8_t	PSIP_AC3AudioDescriptor::getTextCode() const
{
	return textcode;
}

uint8_t	PSIP_AC3AudioDescriptor::getLanguageFlag() const
{
	return language_flag;
}

uint8_t	PSIP_AC3AudioDescriptor::getLanguageFlag2() const
{
	return language_flag_2;
}

const uint8_t* PSIP_AC3AudioDescriptor::getIso639LanguageCode() const
{
	return &iso_639_language_code[0];
}

void PSIP_AC3AudioDescriptor::dump()
{
	printf("		[ -> PSIP_AC3AudioDescriptor ]\n");
	printf("			sample_rate_code = 0x%X\n", sample_rate_code);
	printf("			bsid = 0x%X\n", bsid);
	printf("			bit_rate_code = 0x%X\n", bit_rate_code);
	printf("			surround_mode = 0x%X\n", surround_mode);
	printf("			bsmode = 0x%X\n", bsmode);
	printf("			num_channels = 0x%X\n", num_channels);
	printf("			full_svc = 0x%X\n", full_svc);
	printf("			langcod = 0x%X\n", langcod);
	if ( 0 == num_channels ) {
		printf("			langcod2 = 0x%X\n", langcod2);
	}
	if ( 2 > bsmode ) {
		printf("			mainid = 0x%X\n", getMainId());
		printf("			priority = 0x%X\n", getPriority());
	}
	else {
		printf("			asvcflags = 0x%X\n", asvcflags);
	}
	printf("			textlen = 0x%X\n", textlen);
	printf("			textcode = 0x%X\n", textcode);
	printf("			language_flag = 0x%X\n", language_flag);
	printf("			language_flag_2 = 0x%X\n", language_flag_2);
	if ( 1 == language_flag ) {
		printf("			iso_639_language_code = %s\n", &iso_639_language_code[0]);
	}
	printf("		[ <- PSIP_AC3AudioDescriptor ]\n");
}


/*
 * PSIP_ExtendedTextSection.cpp
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_ExtendedTextSection.h"

PSIP_ExtendedTextSection::PSIP_ExtendedTextSection(const uint8_t * const buffer)
 : LongCrcSection(buffer)
 , protocol_version(0)
 , ETM_id(0)
 , extended_text_length(0)
 , extended_text_message(NULL)
{
	if ( 9 < sectionLength ) {
		int index = 8;
		protocol_version = buffer[index++] & 0xFF;
		ETM_id = (buffer[index++] & 0xFF) << 24;
		ETM_id |= (buffer[index++] & 0xFF) << 16;
		ETM_id |= (buffer[index++] & 0xFF) << 8;
		ETM_id |= buffer[index++] & 0xFF;
		extended_text_length = sectionLength - 5 - 5 - 4;
		if ( 0 < extended_text_length ) {
			extended_text_message = new uint8_t[extended_text_length];
			assert(NULL != extended_text_message);
			for ( uint32_t i = 0; i < extended_text_length; i++ ) {
				extended_text_message[i] = buffer[index++] & 0xFF;
			}
		}
	}
}

PSIP_ExtendedTextSection::~PSIP_ExtendedTextSection()
{
	if ( extended_text_message ) {
		delete[] extended_text_message;
		extended_text_message = NULL;
	}
}

uint32_t PSIP_ExtendedTextSection::getETMId() const
{
	return ETM_id;
}

uint32_t PSIP_ExtendedTextSection::getExtendedTextLength() const
{
	return extended_text_length;
}

const uint8_t* PSIP_ExtendedTextSection::getExtendedText() const
{
	return extended_text_message;
}

void PSIP_ExtendedTextSection::dump()
{
	printf("[ -> PSIP_ExtendedTextSection ]\n");
	printf("	protocol_version = 0x%X\n", protocol_version);
	printf("	ETM_id = 0x%X\n", ETM_id);
	printf("	extended_text_length = 0x%X\n", extended_text_length);
	printf("	extended_text_message = ");
	for ( uint32_t i = 0; i < extended_text_length; i++ ) {
		printf(" %c", extended_text_message[i]);
	}
	printf("\n");
	printf("[ <- PSIP_ExtendedTextSection ]\n");
}

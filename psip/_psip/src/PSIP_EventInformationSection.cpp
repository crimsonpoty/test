/*
 * PSIP_EventInformationSection.cpp
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#include <assert.h>
#include <time.h>
#include <typeinfo>
#include "PSIP_Descriptor.h"
#include "PSIP_EventInformationSection.h"

static const int _UTC_OFFSET = 315964800;

PSIP_EventInfo::PSIP_EventInfo(const uint8_t* const buffer)
 : event_id(0)
 , start_time(0)
 , ETM_location(0)
 , length_in_seconds(0)
 , title_length(0)
 , title_text(NULL)
 , descriptors_length(0)
{
	int index = 0;
	event_id = (buffer[index++] & 0x3F) << 8;
	event_id |= buffer[index++] & 0xFF;
	start_time = (buffer[index++] & 0xFF) << 24;
	start_time |= (buffer[index++] & 0xFF) << 16;
	start_time |= (buffer[index++] & 0xFF) << 8;
	start_time |= buffer[index++] & 0xFF;
	ETM_location = (buffer[index] & 0x30) >> 4;
	length_in_seconds = (buffer[index++] & 0x0F) << 16;
	length_in_seconds |= (buffer[index++] & 0xFF) << 8;
	length_in_seconds |= buffer[index++] & 0xFF;
	title_length = buffer[index++] & 0xFF;
	if ( 0 < title_length ) {
		assert(0x100 > title_length);
		title_text = new uint8_t[title_length];
		assert(NULL != title_text);
		memcpy(title_text, &buffer[index++], title_length);
		index += title_length;
	}
	descriptors_length = buffer[index++] & 0xFF;

	uint16_t bytesLeft = descriptors_length;
	uint16_t loopLength = 0;

	while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
		descriptorPsip(&buffer[index]);
		index += loopLength;
		bytesLeft -= loopLength;
	}
}

PSIP_EventInfo::~PSIP_EventInfo()
{
	if ( title_text ) {
		delete[] title_text;
		title_text = NULL;
	}
}

uint16_t PSIP_EventInfo::getEventId() const
{
	return event_id;
}

uint32_t PSIP_EventInfo::getStartTime() const
{
	return start_time;
}

uint8_t PSIP_EventInfo::getETMLocation() const
{
	return ETM_location;
}

uint32_t PSIP_EventInfo::getDurationSeconds() const
{
	return length_in_seconds;
}

uint8_t PSIP_EventInfo::getTitleLength() const
{
	return title_length;
}

const uint8_t* PSIP_EventInfo::getTitleText() const
{
	return title_text;
}

void PSIP_EventInfo::dump()
{
	printf("	[ -> PSIP_EventInfo ]\n");
	printf("		event_id = 0x%X\n", event_id);
	printf("		start_time = 0x%X ", start_time);
	time_t _time = _UTC_OFFSET + start_time;
	struct tm* _localtime = localtime(&_time);
	printf(": %s", asctime (_localtime));
	printf("		ETM_location = 0x%X\n", ETM_location);
	printf("		length_in_seconds = 0x%X\n", length_in_seconds);
	printf("		title_length = 0x%X\n", title_length);
	printf("		title_text = ");
	for ( int i = 0; i < title_length; i++ ) {
		printf(" %c", title_text[i]);
	}
	printf("\n");
	printf("		descriptors_length = 0x%X\n", descriptors_length);

	const DescriptorList* dlist = PSIP_DescriptorContainer::getDescriptors();
	DescriptorList::const_iterator it;
	for ( it = dlist->begin(); it != dlist->end(); it++ ) {
		switch ( (*it)->getTag() ) {
		case PSIP_Descriptor::PSIP_AC3_AUDIO_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_CAPTION_SERVICE_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_CONTENT_ADVISORY_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_EXTENDED_CHANNEL_NAME_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_SERVICE_LOCATION_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_TIME_SHIFTED_SERVICE_DESCRIPTOR:
		case PSIP_Descriptor::PSIP_COMPONENT_NAME_DESCRIPTOR:
			static_cast<PSIP_Descriptor*>(*it)->dump();
			break;
		}
	}
	printf("	[ <- PSIP_EventInfo ]\n");
}


PSIP_EventInformationSection::PSIP_EventInformationSection(const uint8_t* const buffer)
 : LongCrcSection(buffer)
{
	if ( 10 < sectionLength ) {
		int index = 8;
		protocol_version = buffer[index++] & 0xFF;
		num_events_in_section = buffer[index++] & 0xFF;

		uint16_t bytesLeft = (sectionLength - 5 - 2);	// 5 : table_id_extention ~ last_sectoin_number, 2 : protocol_version ~ num_events_in_section
		bytesLeft -= 4;	// crc32

		for ( size_t i = 0; i < num_events_in_section; i++ ) {
			PSIP_EventInfo* _info = new PSIP_EventInfo(&buffer[index]);
			assert(NULL != _info);

			uint16_t loopLength = 12 + _info->title_length + _info->descriptors_length;
			index += loopLength;
			bytesLeft -= loopLength;
			mPSIP_EventInfoList.push_back(_info);
		}
	}
}

PSIP_EventInformationSection::~PSIP_EventInformationSection()
{
	PSIP_EventInfoList::iterator it;
	for ( it = mPSIP_EventInfoList.begin(); it != mPSIP_EventInfoList.end(); ++it ) {
		delete *it;
	}
}

const PSIP_EventInfoList* PSIP_EventInformationSection::getEventInfo() const
{
	return &mPSIP_EventInfoList;
}

void PSIP_EventInformationSection::dump()
{
	printf("[ -> PSIP_EventInformationSection ]\n");
	printf("	sectionLength = 0x%X\n", sectionLength);
	printf("	protocol_version = 0x%X\n", protocol_version);
	printf("	num_events_in_section = 0x%X\n", num_events_in_section);
	for ( int i = 0; i < num_events_in_section; i++ ) {
		mPSIP_EventInfoList[i]->dump();
	}
	printf("[ <- PSIP_EventInformationSection ]\n");
}


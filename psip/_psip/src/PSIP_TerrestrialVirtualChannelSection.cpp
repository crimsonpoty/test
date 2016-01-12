/*
 * PSIP_TerrestrialVirtualChannelSection.cpp
 *
 *  Created on: 2015. 9. 24.
 *      Author: erato
 */

#include <assert.h>
#include <typeinfo>
#include "PSIP_Descriptor.h"
#include "PSIP_TerrestrialVirtualChannelSection.h"

PSIP_VirtualChannelInfo::PSIP_VirtualChannelInfo(const uint8_t * const buffer)
 : major_channel_number(0)
 , minor_channel_number(0)
 , modulation_mode(0)
 , carrier_frequency(0)
 , channel_TSID(0)
 , program_number(0)
 , ETM_location(0)
 , access_controlled(0)
 , hidden(0)
 , path_select(0)
 , out_of_band(0)
 , hide_guide(0)
 , service_type(0)
 , source_id(0)
 , descriptors_length(0)
{
	memset(&short_name[0], 0x00, sizeof(uint16_t) * 7);
	int index = 0;
	for ( int i = 0; i < 7; i++ ) {
		short_name[i] = (buffer[index++] & 0xFF) << 8;
		short_name[i] |= buffer[index++] & 0xFF;
	}

	major_channel_number = (buffer[index++] & 0x0F) << 6;
	major_channel_number |= (buffer[index] & 0xFC) >> 2;
	minor_channel_number = (buffer[index++] & 0x03) << 8;
	minor_channel_number |= buffer[index++] & 0xFF;
	modulation_mode = buffer[index++] & 0xFF;
	carrier_frequency = (buffer[index++] & 0xFF) << 24;
	carrier_frequency |= (buffer[index++] & 0xFF) << 16;
	carrier_frequency |= (buffer[index++] & 0xFF) << 8;
	carrier_frequency |= buffer[index++] & 0xFF;
	channel_TSID = (buffer[index++] & 0xFF) << 8;
	channel_TSID |= buffer[index++] & 0xFF;
	program_number = (buffer[index++] & 0xFF) << 8;
	program_number |= buffer[index++] & 0xFF;

	ETM_location = (buffer[index] & 0xC0) >> 6;
	access_controlled = (buffer[index] & 0x20) >> 5;
	hidden = (buffer[index] & 0x10) >> 4;
	path_select = (buffer[index] & 0x08) >> 3;
	out_of_band = (buffer[index] & 0x04) >> 2;
	hide_guide = (buffer[index++] & 0x02) >> 1;

	service_type = buffer[index++] & 0x3F;
	source_id = (buffer[index++] & 0xFF) << 8;
	source_id |= buffer[index++] & 0xFF;
	descriptors_length = (buffer[index++] & 0x03) << 8;
	descriptors_length |= buffer[index++] & 0xFF;

	uint16_t bytesLeft = descriptors_length;
	uint16_t loopLength = 0;

	while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
		descriptorPsip(&buffer[index]);
		index += loopLength;
		bytesLeft -= loopLength;
	}
}

PSIP_VirtualChannelInfo::~PSIP_VirtualChannelInfo()
{

}

const uint16_t* PSIP_VirtualChannelInfo::getShortName() const
{
	return &short_name[0];
}

uint16_t PSIP_VirtualChannelInfo::getMajorChannelNumber() const
{
	return major_channel_number;
}

uint16_t PSIP_VirtualChannelInfo::getMinorChannelNumber() const
{
	return minor_channel_number;
}

uint16_t PSIP_VirtualChannelInfo::getOnePartChannelNumber() const
{
	// one_part_number = (major_channel_number & 0x00F) << 10 + minor_channel_number
	return ((major_channel_number & 0x00F) << 10) + minor_channel_number;
}

uint8_t PSIP_VirtualChannelInfo::getModulationMode() const
{
	return modulation_mode;
}

uint32_t PSIP_VirtualChannelInfo::getCarrierFrequency() const
{
	return carrier_frequency;
}

uint16_t PSIP_VirtualChannelInfo::getChannelTSID() const
{
	return channel_TSID;
}

uint16_t PSIP_VirtualChannelInfo::getProgramNumber() const
{
	return program_number;
}

uint16_t PSIP_VirtualChannelInfo::getETMLocation() const
{
	return ETM_location;
}

uint8_t PSIP_VirtualChannelInfo::isHidden() const
{
	return hidden;
}

uint8_t PSIP_VirtualChannelInfo::isAccessControlled() const
{
	return access_controlled;
}

uint8_t PSIP_VirtualChannelInfo::getServiceType() const
{
	return service_type;
}

uint16_t PSIP_VirtualChannelInfo::getSourceId() const
{
	return source_id;
}

void PSIP_VirtualChannelInfo::dump()
{
	printf("	[ -> VirtualChannelInfo ]\n");
	printf("		short_name =");
	for ( int i = 0; i < 7; i++ ) {
		printf(" 0x%X", short_name[i]);
	}
	printf("\n");
	printf("		major_channel_number = 0x%X\n", major_channel_number);
	printf("		minor_channel_number = 0x%X\n", minor_channel_number);
	printf("		modulation_mode = 0x%X\n", modulation_mode);
	printf("		carrier_frequency = %d\n", carrier_frequency);
	printf("		channel_TSID = 0x%X\n", channel_TSID);
	printf("		program_number = 0x%X\n", program_number);
	printf("		ETM_location = 0x%X\n", ETM_location);
	printf("		access_controlled = 0x%X\n", access_controlled);
	printf("		hidden = 0x%X\n", hidden);
	printf("		path_select = 0x%X\n", path_select);
	printf("		out_of_band = 0x%X\n", out_of_band);
	printf("		hide_guide = 0x%X\n", hide_guide);
	printf("		service_type = 0x%X\n", service_type);
	printf("		source_id = 0x%X\n", source_id);
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
	printf("	[ <- VirtualChannelInfo ]\n");
}

PSIP_TerrestrialVirtualChannelSection::PSIP_TerrestrialVirtualChannelSection(const uint8_t * const buffer)
 : LongCrcSection(buffer)
 , protocol_version(0)
 , num_channels_in_section(0)
 , additional_descriptors_length(0)
{
	if ( 12 < sectionLength ) {
		int index = 8;
		protocol_version = buffer[index++] & 0xFF;
		num_channels_in_section = buffer[index++] & 0xFF;

		uint16_t bytesLeft = (sectionLength - 5 - 2);	// 5 : table_id_extention ~ last_sectoin_number, 3 : protocol_version ~ num_channels_in_section
		uint16_t loopLength = 0;

		for ( size_t i = 0; i < num_channels_in_section; i++ ) {
			PSIP_VirtualChannelInfo* channel = new PSIP_VirtualChannelInfo(&buffer[index]);
			assert(NULL != channel);

			loopLength = 32 + channel->descriptors_length;
			index += loopLength;
			bytesLeft -= loopLength;
			mChannelInfoList.push_back(channel);
		}

		additional_descriptors_length = (buffer[index++] & 0x03) << 8;
		additional_descriptors_length |= buffer[index++] & 0xFF;
		bytesLeft = additional_descriptors_length;

		while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
			descriptorPsip(&buffer[index]);
			index += loopLength;
			bytesLeft -= loopLength;
		}
	}
}

PSIP_TerrestrialVirtualChannelSection::~PSIP_TerrestrialVirtualChannelSection()
{
	PSIP_VirtualChannelInfoList::iterator it;
	for ( it = mChannelInfoList.begin(); it != mChannelInfoList.end(); ++it ) {
		delete *it;
		(*it) = NULL;
	}
}

const PSIP_VirtualChannelInfoList* PSIP_TerrestrialVirtualChannelSection::getChannelInfo() const
{
	return &mChannelInfoList;
}

void PSIP_TerrestrialVirtualChannelSection::dump()
{
	printf("[ -> PSIP_TerrestrialVirtualChannelSection ]\n");
	printf("	sectionNumber = 0x%X\n", LongSection::sectionNumber);
	printf("	lastSectionNumber = 0x%X\n", LongSection::lastSectionNumber);
	printf("	protocol_version = 0x%X\n", protocol_version);
	printf("	num_channels_in_section = 0x%X\n", num_channels_in_section);
	for ( int i = 0; i < num_channels_in_section; i++ ) {
		mChannelInfoList[i]->dump();
	}
	printf("	additional_descriptors_length = 0x%X\n", additional_descriptors_length);

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
	printf("[ <- PSIP_TerrestrialVirtualChannelSection ]\n");
}

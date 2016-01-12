/*
 * PSIP_CableVirtualChannelSection.cpp
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#include <typeinfo>
#include "PSIP_Descriptor.h"
#include "PSIP_CableVirtualChannelSection.h"

PSIP_CableVirtualChannelSection::PSIP_CableVirtualChannelSection(const uint8_t * const buffer)
 : PSIP_TerrestrialVirtualChannelSection(buffer)
{

}

PSIP_CableVirtualChannelSection::~PSIP_CableVirtualChannelSection()
{

}

void PSIP_CableVirtualChannelSection::dump()
{
	printf("[ -> PSIP_CableVirtualChannelSection ]\n");
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
	printf("[ <- PSIP_CableVirtualChannelSection ]\n");
}

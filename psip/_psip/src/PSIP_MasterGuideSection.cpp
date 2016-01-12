/*
 * PSIP_MasterGuideSection.cpp
 *
 *  Created on: 2015. 9. 24.
 *      Author: erato
 */

#include <assert.h>
#include <typeinfo>
#include "PSIP_Descriptor.h"
#include "PSIP_MasterGuideSection.h"

PSIP_MasterGuideInfo::PSIP_MasterGuideInfo(const uint8_t* const buffer)
 : table_type(0)
 , table_type_PID(0)
 , table_type_version_number(0)
 , number_bytes(0)
 , table_type_descriptors_length(0)
{
	int index = 0;
	table_type = (buffer[index++] & 0xFF) << 8;
	table_type |= buffer[index++] & 0xFF;
	table_type_PID = (buffer[index++] & 0x1F) << 8;
	table_type_PID |= buffer[index++] & 0xFF;
	table_type_version_number = buffer[index++] & 0x1F;
	number_bytes = (buffer[index++] & 0xFF) << 24;
	number_bytes |= (buffer[index++] & 0xFF) << 16;
	number_bytes |= (buffer[index++] & 0xFF) << 8;
	number_bytes |= buffer[index++] & 0xFF;
	table_type_descriptors_length = (buffer[index++] & 0x0F) << 8;
	table_type_descriptors_length |= buffer[index++] & 0xFF;

	uint16_t bytesLeft = table_type_descriptors_length;
	uint16_t loopLength = 0;

	while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
		descriptorPsip(&buffer[index]);
		index += loopLength;
		bytesLeft -= loopLength;
	}
}

PSIP_MasterGuideInfo::~PSIP_MasterGuideInfo()
{

}

uint16_t PSIP_MasterGuideInfo::getTableType() const
{
	return table_type;
}

uint8_t PSIP_MasterGuideInfo::getTableTypePID() const
{
	return table_type_PID;
}

uint8_t	PSIP_MasterGuideInfo::getTableTypeVersionNumber() const
{
	return table_type_version_number;
}

uint32_t PSIP_MasterGuideInfo::getNumberBytes() const
{
	return number_bytes;
}

void PSIP_MasterGuideInfo::dump()
{
	printf("	[ -> PSIP_MasterGuideInfo ]\n");
	printf("		table_type = 0x%X\n", table_type);
	printf("		table_type_PID = 0x%X\n", table_type_PID);
	printf("		table_type_version_number = 0x%X\n", table_type_version_number);
	printf("		number_bytes = 0x%X\n", number_bytes);
	printf("		table_type_descriptors_length = 0x%X\n", table_type_descriptors_length);

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
	printf("	[ <- PSIP_MasterGuideInfo ]\n");
}


PSIP_MasterGuideSection::PSIP_MasterGuideSection(const uint8_t * const buffer)
 : LongCrcSection(buffer)
{
	if ( 13 < sectionLength ) {
		int index = 8;
		protocol_version = buffer[index++] & 0xFF;
		tables_defined = (buffer[index++] & 0xFF) << 8;
		tables_defined |= buffer[index++] & 0xFF;

		uint16_t bytesLeft = (sectionLength - 5 - 3);	// 5 : table_id_extention ~ last_sectoin_number, 3 : protocol_version ~ tables_defined
		uint16_t loopLength = 0;

		for ( size_t i = 0; i < tables_defined; i++ ) {
			PSIP_MasterGuideInfo* _info = new PSIP_MasterGuideInfo(&buffer[index]);
			assert(NULL != _info);

			loopLength = 11 + _info->table_type_descriptors_length;
			index += loopLength;
			bytesLeft -= loopLength;
			mMasterGuideInfoList.push_back(_info);
		}

		descriptors_length = (buffer[index++] & 0xFF) << 8;
		descriptors_length |= buffer[index++] & 0xFF;
		bytesLeft = descriptors_length;

		while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
			descriptorPsip(&buffer[index]);
			index += loopLength;
			bytesLeft -= loopLength;
		}
	}
}

PSIP_MasterGuideSection::~PSIP_MasterGuideSection()
{
	PSIP_MasterGuideInfoList::iterator it;
	for ( it = mMasterGuideInfoList.begin(); it != mMasterGuideInfoList.end(); ++it ) {
		delete *it;
	}
}


const PSIP_MasterGuideInfoList* PSIP_MasterGuideSection::getTableInfo() const
{
	return &mMasterGuideInfoList;
}

void PSIP_MasterGuideSection::dump()
{
	printf("[ -> PSIP_MasterGuideSection ]\n");
	printf("	protocol_version = 0x%X\n", protocol_version);
	printf("	tables_defined = 0x%X\n", tables_defined);
	for ( int i = 0; i < tables_defined; i++ ) {
		mMasterGuideInfoList[i]->dump();
	}
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
	printf("[ <- PSIP_MasterGuideSection ]\n");
}

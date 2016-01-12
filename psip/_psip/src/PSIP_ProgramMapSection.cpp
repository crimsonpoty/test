/*
 * PSIP_ProgramMapSection.cpp
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_Descriptor.h"
#include "PSIP_ProgramMapSection.h"

PSIP_ElementaryStreamInfo::PSIP_ElementaryStreamInfo(const uint8_t * const buffer)
 : streamType(0)
 , elementaryPid(0)
{
	int index = 0;
	streamType = buffer[index++] & 0xFF;
	elementaryPid = (buffer[index++] & 0x1F) << 8;
	elementaryPid |= buffer[index++] & 0xFF;

	esInfoLength = (buffer[index++] & 0x0F) << 8;
	esInfoLength |= buffer[index++] & 0xFF;

	for (size_t i = index; i < esInfoLength; i += buffer[index+1] + 2) {
		descriptorPsip(&buffer[i]);
	}
}

uint8_t PSIP_ElementaryStreamInfo::getType(void) const
{
	return streamType;
}

uint16_t PSIP_ElementaryStreamInfo::getPid(void) const
{
	return elementaryPid;
}

void PSIP_ElementaryStreamInfo::dump()
{
	printf("	[ -> PSIP_ElementaryStreamInfo ]\n");
	printf("		streamType = 0x%X\n", streamType);
	printf("		elementaryPid = 0x%X\n", elementaryPid);

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
	printf("	[ <- PSIP_ElementaryStreamInfo ]\n");
}

PSIP_ProgramMapSection::PSIP_ProgramMapSection(const uint8_t * const buffer)
 : LongCrcSection(buffer)
 , pcrPid(0)
 , programInfoLength(0)
{
	if ( 10 < sectionLength ) {
		int index = 8;
		pcrPid = (buffer[index++] & 0x1F) << 8;
		pcrPid |= buffer[index++] & 0xFF;
		programInfoLength = (buffer[index++] & 0x0F) << 8;
		programInfoLength |= buffer[index++] & 0xFF;

		uint16_t bytesLeft = programInfoLength;
		uint16_t loopLength = 0;

		while (bytesLeft > 1 && bytesLeft >= (loopLength = 2 + buffer[index+1])) {
			descriptorPsip(&buffer[index]);
			index += loopLength;
			bytesLeft -= loopLength;
		}

		bytesLeft = sectionLength - 9 - programInfoLength;
		loopLength = 5;
		while ( bytesLeft > 4 && bytesLeft >= loopLength ) {
			PSIP_ElementaryStreamInfo* _info = new PSIP_ElementaryStreamInfo(&buffer[index]);
			assert(NULL != _info);
			mPSIP_ElementaryStreamInfoList.push_back(_info);
			loopLength = (buffer[index+3] & 0x0F) << 8;
			loopLength |= buffer[index+4] & 0xFF;
			loopLength += 5;
			index += loopLength;
			bytesLeft -= loopLength;
		}
	}
}

PSIP_ProgramMapSection::~PSIP_ProgramMapSection()
{
	PSIP_ElementaryStreamInfoList::iterator it;
	for ( it = mPSIP_ElementaryStreamInfoList.begin(); it != mPSIP_ElementaryStreamInfoList.end(); ++it ) {
		delete *it;
	}
}

uint16_t PSIP_ProgramMapSection::getPcrPid(void) const
{
	return pcrPid;
}

uint16_t PSIP_ProgramMapSection::getProgramNumber(void) const
{
	return getTableIdExtension();
}

const PSIP_ElementaryStreamInfoList *PSIP_ProgramMapSection::getEsInfo(void) const
{
	return &mPSIP_ElementaryStreamInfoList;
}

void PSIP_ProgramMapSection::dump()
{
	printf("[ -> PSIP_ProgramMapSection ]\n");
	printf("	program_number = 0x%X\n", getTableIdExtension());
	printf("	pcrPid = 0x%X\n", pcrPid);

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

	PSIP_ElementaryStreamInfoList::iterator it_e;
	for ( it_e = mPSIP_ElementaryStreamInfoList.begin(); it_e != mPSIP_ElementaryStreamInfoList.end(); ++it_e ) {
		(*it_e)->dump();
	}
	printf("[ <- PSIP_ProgramMapSection ]\n");
}

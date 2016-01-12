/*
 * PSIP_TimeShiftedServiceDescriptor.cpp
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#include <assert.h>
#include "PSIP_TimeShiftedServiceDescriptor.h"

PSIP_TimeShiftedServiceInfo::PSIP_TimeShiftedServiceInfo()
 : time_shift(0)
 , major_channel_number(0)
 , minor_channel_number(0)
{

}

PSIP_TimeShiftedServiceInfo::~PSIP_TimeShiftedServiceInfo()
{

}

uint16_t PSIP_TimeShiftedServiceInfo::getTimeShift() const
{
	return time_shift;
}

uint16_t PSIP_TimeShiftedServiceInfo::getMajorChannelNumber() const
{
	return major_channel_number;
}

uint16_t PSIP_TimeShiftedServiceInfo::getMinorChannelNumber() const
{
	return minor_channel_number;
}


PSIP_TimeShiftedServiceDescriptor::PSIP_TimeShiftedServiceDescriptor(const uint8_t * const buffer)
 : PSIP_Descriptor(buffer)
 , number_of_services(0)
{
	if ( 2 < descriptorLength ) {
		int index = 2;
		number_of_services = buffer[index++] & 0xFF;
		for ( uint8_t i = 0; i < number_of_services; i++ ) {
			PSIP_TimeShiftedServiceInfo* _info = new PSIP_TimeShiftedServiceInfo;
			assert(NULL != _info);
			_info->time_shift = (buffer[index] & 0x03) << 8;
			_info->time_shift |= buffer[index++] & 0xFF;
			_info->major_channel_number = (buffer[index++] & 0x0F) << 8;
			_info->major_channel_number |= buffer[index] & 0xFC;
			_info->minor_channel_number = (buffer[index++] & 0x03) << 8;
			_info->minor_channel_number |= buffer[index++] & 0xFF;
			mPSIP_TimeShiftedServiceInfoList.push_back(_info);
		}
	}
}

PSIP_TimeShiftedServiceDescriptor::~PSIP_TimeShiftedServiceDescriptor()
{
	PSIP_TimeShiftedServiceInfoList::iterator it;
	for ( it = mPSIP_TimeShiftedServiceInfoList.begin(); it != mPSIP_TimeShiftedServiceInfoList.end(); ++it ) {
		delete *it;
	}
}

const PSIP_TimeShiftedServiceInfoList* PSIP_TimeShiftedServiceDescriptor::getPSIP_TimeShiftedServiceInfo() const
{
	return &mPSIP_TimeShiftedServiceInfoList;
}

void PSIP_TimeShiftedServiceDescriptor::dump()
{
	printf("		[ -> PSIP_TimeShiftedServiceDescriptor ]\n");
	printf("			number_of_services = %d\n", number_of_services);
	for ( int i = 0; i < number_of_services; i++ ) {
		printf("				time_shift = 0x%X\n", mPSIP_TimeShiftedServiceInfoList[i]->time_shift);
		printf("				major_channel_number = 0x%X\n", mPSIP_TimeShiftedServiceInfoList[i]->major_channel_number);
		printf("				minor_channel_number = 0x%X\n", mPSIP_TimeShiftedServiceInfoList[i]->minor_channel_number);
	}
	printf("		[ <- PSIP_TimeShiftedServiceDescriptor ]\n");
}


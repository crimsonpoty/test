/*
 * PSIP_TimeShiftedServiceDescriptor.h
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#ifndef PSIP_TIMESHIFTEDSERVICEDESCRIPTOR_H_
#define PSIP_TIMESHIFTEDSERVICEDESCRIPTOR_H_

#include "PSIP_Descriptor.h"

class PSIP_TimeShiftedServiceInfo;

typedef std::vector<PSIP_TimeShiftedServiceInfo*>	PSIP_TimeShiftedServiceInfoList;


class PSIP_TimeShiftedServiceInfo
{
protected:
	PSIP_TimeShiftedServiceInfo();
	virtual ~PSIP_TimeShiftedServiceInfo();
	friend class PSIP_TimeShiftedServiceDescriptor;

public:
	uint16_t	getTimeShift() const;
	uint16_t	getMajorChannelNumber() const;
	uint16_t	getMinorChannelNumber() const;

protected:
	uint16_t	:6;
	uint16_t	time_shift:10;
	uint16_t	:6;
	uint16_t	major_channel_number:10;
	uint16_t	:6;
	uint16_t	minor_channel_number:10;
};


class PSIP_TimeShiftedServiceDescriptor : public PSIP_Descriptor
{
protected:
	PSIP_TimeShiftedServiceDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_TimeShiftedServiceDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	const PSIP_TimeShiftedServiceInfoList* getPSIP_TimeShiftedServiceInfo() const;
	virtual void dump();

protected:
	uint8_t		:3;
	uint8_t		number_of_services:5;

protected:
	PSIP_TimeShiftedServiceInfoList mPSIP_TimeShiftedServiceInfoList;
};

#endif /* PSIP_TIMESHIFTEDSERVICEDESCRIPTOR_H_ */

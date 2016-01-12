/*
 * PSIP_SystemTimeSection.h
 *
 *  Created on: 2015. 9. 23.
 *      Author: erato
 */

#ifndef PSIP_SYSTEMTIMESECTION_H_
#define PSIP_SYSTEMTIMESECTION_H_

#include "long_crc_section.h"
#include "PSIP_DescriptorContainer.h"

class PSIP_SystemTimeSection : public LongCrcSection, public PSIP_DescriptorContainer
{
public:
	PSIP_SystemTimeSection(const uint8_t * const buffer);
	virtual ~PSIP_SystemTimeSection();

public:
	static const int PID = 0x1FFB;
	static const int TID = 0xCD;

public:
	struct daylight_saving {
		uint16_t DS_status:1;
		uint16_t :2;
		uint16_t DS_day_of_month:5;
		uint16_t DS_hour:8;
	};

public:
	uint8_t getProtocolVersion() const;
	uint32_t getSystemTime() const;
	uint8_t getGpsUtcOffset() const;
	time_t getConvertedUtcTime() const;
	daylight_saving* getDaylight_saving() const;
	void dump();

protected:
	uint32_t system_time;
	uint32_t protocol_version:8;
	uint32_t GPS_UTC_offset:8;
	uint32_t daylight_savings:16;
	daylight_saving mDaylight_saving;
};

#endif /* PSIP_SYSTEMTIMESECTION_H_ */

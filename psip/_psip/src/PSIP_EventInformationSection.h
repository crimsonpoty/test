/*
 * PSIP_EventInformationSection.h
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#ifndef PSIP_EVENTINFORMATIONSECTION_H_
#define PSIP_EVENTINFORMATIONSECTION_H_

#include "long_crc_section.h"
#include "PSIP_DescriptorContainer.h"

class PSIP_EventInfo;

typedef std::vector<PSIP_EventInfo*> PSIP_EventInfoList;


class PSIP_EventInfo : public PSIP_DescriptorContainer
{
protected:
	PSIP_EventInfo(const uint8_t* const buffer);
	virtual ~PSIP_EventInfo();
	friend class PSIP_EventInformationSection;

public:
	uint16_t	getEventId() const;
	uint32_t	getStartTime() const;
	uint8_t		getETMLocation() const;
	uint32_t	getDurationSeconds() const;
	uint8_t		getTitleLength() const;
	const uint8_t*	getTitleText() const;
	void 		dump();

protected:
	uint16_t	:2;
	uint16_t	event_id:2;
	uint32_t	start_time;
	uint32_t	:2;
	uint32_t	ETM_location:2;
	uint32_t	length_in_seconds:20;
	uint32_t	title_length:8;
	uint8_t*	title_text;
	uint16_t	:2;
	uint16_t	descriptors_length:14;
};


class PSIP_EventInformationSection : public LongCrcSection
{
public:
	PSIP_EventInformationSection(const uint8_t* const buffer);
	virtual ~PSIP_EventInformationSection();

public:
	static const int TID = 0xCB;

public:
	const PSIP_EventInfoList* getEventInfo() const;
	void dump();

protected:
	uint8_t 	protocol_version;
	uint8_t 	num_events_in_section;

protected:
	PSIP_EventInfoList mPSIP_EventInfoList;
};

#endif /* PSIP_EVENTINFORMATIONSECTION_H_ */

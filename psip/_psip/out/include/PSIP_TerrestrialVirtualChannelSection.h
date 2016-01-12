/*
 * PSIP_TerrestrialVirtualChannelSection.h
 *
 *  Created on: 2015. 9. 24.
 *      Author: erato
 */

#ifndef PSIP_TERRESTRIALVIRTUALCHANNELSECTION_H_
#define PSIP_TERRESTRIALVIRTUALCHANNELSECTION_H_

#include "long_crc_section.h"
#include "PSIP_DescriptorContainer.h"

class PSIP_VirtualChannelInfo;

typedef std::vector<PSIP_VirtualChannelInfo*> PSIP_VirtualChannelInfoList;


class PSIP_VirtualChannelInfo : public PSIP_DescriptorContainer
{
protected:
	PSIP_VirtualChannelInfo(const uint8_t * const buffer);
	virtual ~PSIP_VirtualChannelInfo();
	friend class PSIP_TerrestrialVirtualChannelSection;

public:
	const uint16_t*	getShortName() const;
	uint16_t		getMajorChannelNumber() const;
	uint16_t		getMinorChannelNumber() const;
	uint16_t		getOnePartChannelNumber() const;
	uint8_t			getModulationMode() const;
	uint32_t		getCarrierFrequency() const;
	uint16_t		getChannelTSID() const;
	uint16_t		getProgramNumber() const;
	uint16_t		getETMLocation() const;
	uint8_t			isHidden() const;
	uint8_t			isAccessControlled() const;
	uint8_t			getServiceType() const;
	uint16_t		getSourceId() const;
	virtual void 	dump();

protected:
	uint16_t	short_name[8];
	uint32_t	:4;
	uint32_t	major_channel_number:10;
	uint32_t	minor_channel_number:10;
	uint32_t	modulation_mode:8;
	uint32_t	carrier_frequency;
	uint16_t	channel_TSID;
	uint16_t	program_number;
	uint16_t	ETM_location:2;
	uint16_t	access_controlled:1;
	uint16_t	hidden:1;
	uint16_t	path_select:1;
	uint16_t	out_of_band:1;
	uint16_t	hide_guide:1;
	uint16_t	:3;
	uint16_t	service_type:6;
	uint32_t	source_id:16;
	uint32_t	:6;
	uint32_t	descriptors_length:10;
};


class PSIP_TerrestrialVirtualChannelSection : public LongCrcSection, public PSIP_DescriptorContainer
{
public:
	PSIP_TerrestrialVirtualChannelSection(const uint8_t * const buffer);
	virtual ~PSIP_TerrestrialVirtualChannelSection();

public:
	static const int PID = 0x1FFB;
	static const int TID = 0xC8;

public:
	virtual const PSIP_VirtualChannelInfoList* getChannelInfo() const;
	virtual void dump();

protected:
	uint16_t 	protocol_version:8;
	uint16_t 	num_channels_in_section:8;
	uint16_t 	:6;
	uint16_t 	additional_descriptors_length:8;

protected:
	PSIP_VirtualChannelInfoList mChannelInfoList;
};

#endif /* PSIP_TERRESTRIALVIRTUALCHANNELSECTION_H_ */

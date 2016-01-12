/*
 * PSIP_CableVirtualChannelSection.h
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#ifndef PSIP_CABLEVIRTUALCHANNELSECTION_H_
#define PSIP_CABLEVIRTUALCHANNELSECTION_H_

#include "PSIP_TerrestrialVirtualChannelSection.h"

class PSIP_CableVirtualChannelSection : public PSIP_TerrestrialVirtualChannelSection
{
public:
	PSIP_CableVirtualChannelSection(const uint8_t * const buffer);
	virtual ~PSIP_CableVirtualChannelSection();

public:
	static const int PID = 0x1FFB;
	static const int TID = 0xC9;

public:
	virtual void dump();

protected:
	friend class PSIP_TerrestrialVirtualChannelSection;
};

#endif /* PSIP_CABLEVIRTUALCHANNELSECTION_H_ */

/*
 * PSIP_Descriptor.h
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#ifndef PSIP_DESCRIPTOR_H_
#define PSIP_DESCRIPTOR_H_

#include <vector>
#include "descriptor.h"

class PSIP_Descriptor : public Descriptor
{
public:
	PSIP_Descriptor(const uint8_t * const buffer);
	virtual ~PSIP_Descriptor();

public:
	static const int PSIP_AC3_AUDIO_DESCRIPTOR				= 0x81;
	static const int PSIP_CAPTION_SERVICE_DESCRIPTOR		= 0x86;
	static const int PSIP_CONTENT_ADVISORY_DESCRIPTOR		= 0x87;
	static const int PSIP_EXTENDED_CHANNEL_NAME_DESCRIPTOR	= 0xA0;
	static const int PSIP_SERVICE_LOCATION_DESCRIPTOR		= 0xA1;
	static const int PSIP_TIME_SHIFTED_SERVICE_DESCRIPTOR	= 0xA2;
	static const int PSIP_COMPONENT_NAME_DESCRIPTOR			= 0xA3;

public:
	virtual void dump();
};

#endif /* PSIP_DESCRIPTOR_H_ */

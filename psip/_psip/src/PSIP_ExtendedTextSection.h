/*
 * PSIP_ExtendedTextSection.h
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#ifndef PSIP_EXTENDEDTEXTSECTION_H_
#define PSIP_EXTENDEDTEXTSECTION_H_

#include "long_crc_section.h"

class PSIP_ExtendedTextSection : public LongCrcSection
{
public:
	PSIP_ExtendedTextSection(const uint8_t * const buffer);
	virtual ~PSIP_ExtendedTextSection();

public:
	static const int TID = 0xCC;

public:
	uint32_t	getETMId() const;
	uint32_t	getExtendedTextLength() const;
	const uint8_t* getExtendedText() const;
	void 		dump();

protected:
	uint8_t 	protocol_version;
	uint32_t	ETM_id;
	uint32_t	extended_text_length;
	uint8_t*	extended_text_message;
};

#endif /* PSIP_EXTENDEDTEXTSECTION_H_ */

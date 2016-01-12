/*
 * PSIP_AC3AudioDescriptor.h
 *
 *  Created on: 2015. 9. 30.
 *      Author: erato
 */

#ifndef PSIP_AC3AUDIODESCRIPTOR_H_
#define PSIP_AC3AUDIODESCRIPTOR_H_

#include "PSIP_Descriptor.h"

class PSIP_AC3AudioDescriptor : public PSIP_Descriptor
{
protected:
	PSIP_AC3AudioDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_AC3AudioDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	uint8_t		getSampleRateCode() const;
	uint8_t		getBsid() const;
	uint8_t		getBitRateCode() const;
	uint8_t		getSurroundMode() const;
	uint8_t		getBsmode() const;
	uint8_t		getNumChannels() const;
	uint8_t		getFullSvc() const;
	uint8_t		getLangCod() const;
	uint8_t		getLangCod2() const;
	uint8_t		getMainId() const;
	uint8_t		getPriority() const;
	uint8_t		getAsvcFlags() const;
	uint8_t		getTextLenth() const;
	uint8_t		getTextCode() const;
	uint8_t		getLanguageFlag() const;
	uint8_t		getLanguageFlag2() const;
	const uint8_t*	getIso639LanguageCode() const;
	virtual void dump();

protected:
	uint8_t		sample_rate_code:3;
	uint8_t		bsid:5;
	uint8_t		bit_rate_code:6;
	uint8_t		surround_mode:2;
	uint8_t		bsmode:3;
	uint8_t		num_channels:4;
	uint8_t		full_svc:1;
	uint8_t		langcod;
	uint8_t		langcod2;
	uint8_t		asvcflags;	// or if ( bsmode < 2 ) than mainid(3), priority(2)
	uint8_t		textlen:7;
	uint8_t		textcode:1;
	uint8_t		language_flag:1;
	uint8_t		language_flag_2:1;
	uint8_t		:6;
	uint8_t		iso_639_language_code[4];
};

#endif /* PSIP_AC3AUDIODESCRIPTOR_H_ */

/*
 * PSIP_CaptionServiceDescriptor.h
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#ifndef PSIP_CAPTIONSERVICEDESCRIPTOR_H_
#define PSIP_CAPTIONSERVICEDESCRIPTOR_H_

#include "PSIP_Descriptor.h"

class PSIP_CaptionServiceInfo;
typedef std::vector<PSIP_CaptionServiceInfo*>	PSIP_CaptionServiceInfoList;


class PSIP_CaptionServiceInfo
{
protected:
	PSIP_CaptionServiceInfo();
	virtual ~PSIP_CaptionServiceInfo();
	friend class PSIP_CaptionServiceDescriptor;

public:
	const uint8_t*	getIso639LanguageCode() const;
	uint8_t		getCCtype() const;
	uint8_t		getLine21Field() const;
	uint8_t		getCaptionServiceNumber() const;
	uint8_t		getEasyReader() const;
	uint8_t		getWideAspectRatio() const;
	void		dump();

protected:
	uint8_t		iso_639_language_code[4];
	uint8_t		cc_type:1;
	uint8_t		:1;
	uint8_t		caption_service_number:6;
	uint16_t	easy_reader:1;
	uint16_t	wide_aspect_ratio:1;
	uint16_t	:14;
};

class PSIP_CaptionServiceDescriptor : public PSIP_Descriptor
{
protected:
	PSIP_CaptionServiceDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_CaptionServiceDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	const PSIP_CaptionServiceInfoList* getCaptionServiceInfo() const;
	virtual void dump();

protected:
	uint8_t	number_of_services;

protected:
	PSIP_CaptionServiceInfoList mPSIP_CaptionServiceInfoList;
};

#endif /* PSIP_CAPTIONSERVICEDESCRIPTOR_H_ */

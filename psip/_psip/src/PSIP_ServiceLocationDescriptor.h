/*
 * PSIP_ServiceLocationDescriptor.h
 *
 *  Created on: 2015. 9. 25.
 *      Author: erato
 */

#ifndef PSIP_SERVICELOCATIONDESCRIPTOR_H_
#define PSIP_SERVICELOCATIONDESCRIPTOR_H_

#include "PSIP_Descriptor.h"

class PSIP_ServiceLocationElement;

typedef std::vector<PSIP_ServiceLocationElement*>	PSIP_ServiceLocationElementList;


class PSIP_ServiceLocationElement
{
protected:
	PSIP_ServiceLocationElement();
	virtual ~PSIP_ServiceLocationElement();
	friend class PSIP_ServiceLocationDescriptor;

public:
	uint8_t		getStreamType() const;
	uint16_t	getElementaryPID() const;
	const uint8_t*	getIso639LanguageCode() const;
	void 		dump();

protected:
	uint8_t		stream_type;
	uint16_t	:3;
	uint16_t	elementary_PID:13;
	uint8_t		iso_639_language_code[4];
};


class PSIP_ServiceLocationDescriptor : public PSIP_Descriptor
{
protected:
	PSIP_ServiceLocationDescriptor(const uint8_t * const buffer);
	virtual ~PSIP_ServiceLocationDescriptor();
	friend class PSIP_DescriptorContainer;

public:
	uint16_t	getPcrPid() const;
	const PSIP_ServiceLocationElementList* getServiceLocationElementList() const;
	virtual void dump();

protected:
	uint16_t	:3;
	uint16_t	PCR_PID:13;
	uint8_t		number_elements;

protected:
	PSIP_ServiceLocationElementList	mPSIP_ServiceLocationElementList;
};

#endif /* PSIP_SERVICELOCATIONDESCRIPTOR_H_ */

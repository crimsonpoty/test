/*
 * PSIP_DescriptorContainer.cpp
 *
 *  Created on: 2015. 9. 23.
 *      Author: erato
 */

#include "PSIP_DescriptorContainer.h"
#include "PSIP_ExtendedChannelNameDescriptor.h"
#include "PSIP_ServiceLocationDescriptor.h"
#include "PSIP_AC3AudioDescriptor.h"
#include "PSIP_ContentAdvisoryDescriptor.h"
#include "PSIP_CaptionServiceDescriptor.h"
#include "PSIP_TimeShiftedServiceDescriptor.h"
#include "PSIP_ContentAdvisoryDescriptor.h"

PSIP_DescriptorContainer::PSIP_DescriptorContainer()
{

}

PSIP_DescriptorContainer::~PSIP_DescriptorContainer()
{

}


void PSIP_DescriptorContainer::descriptorPsip(const uint8_t * const buffer, bool back/*=true*/)
{
	// 0. check descriptor for just only PSIP.
	// 1. if PSIP descriptor
	// 2. if else DVB SI or PSI descriptor

	Descriptor* d = NULL;

	switch (buffer[0]) {
	case 0x80:		// stuffing descriptor
		break;
	case 0x81:		// AC-3 audio descriptor
		d = new PSIP_AC3AudioDescriptor(buffer);
		break;
	case 0x86:		// caption service descriptor
		d = new PSIP_CaptionServiceDescriptor(buffer);
		break;
	case 0x87:		// content advisory descriptor
		d = new PSIP_ContentAdvisoryDescriptor(buffer);
		break;
	case 0xA0:		// extended channel name descriptor
		d = new PSIP_ExtendedChannelNameDescriptor(buffer);
		break;
	case 0xA1:		// service location descriptor
		d = new PSIP_ServiceLocationDescriptor(buffer);
		break;
	case 0xA2:		// time-shifted service descriptor
		d = new PSIP_TimeShiftedServiceDescriptor(buffer);
		break;
	case 0xA3:		// component name descriptor
		d = new PSIP_ContentAdvisoryDescriptor(buffer);
		break;
	}

	if ( d && d->isValid() ) {
		descriptorList.push_back(d);
		return;
	}

	DescriptorContainer::descriptor(buffer, SCOPE_SI);
}

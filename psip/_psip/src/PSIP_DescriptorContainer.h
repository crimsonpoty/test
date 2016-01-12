/*
 * PSIP_DescriptorContainer.h
 *
 *  Created on: 2015. 9. 23.
 *      Author: erato
 */

#ifndef PSIP_DESCRIPTORCONTAINER_H_
#define PSIP_DESCRIPTORCONTAINER_H_

#include <vector>
#include "descriptor_container.h"

class PSIP_DescriptorContainer : public DescriptorContainer
{
public:
	virtual ~PSIP_DescriptorContainer();

protected:
	PSIP_DescriptorContainer();
	void descriptorPsip(const uint8_t * const buffer, bool back=true);
};

#endif /* PSIP_DESCRIPTORCONTAINER_H_ */

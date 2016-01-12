/*
 * $Id$
 *
 * Copyright (C) 2002-2005 Andreas Oberritter <obi@saftware.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * See the file 'COPYING' in the top level directory for details.
 */

#ifndef __stuffing_descriptor_h__
#define __stuffing_descriptor_h__

#include "descriptor.h"

class StuffingDescriptor : public Descriptor
{
	protected:
		std::list<uint8_t> stuffingByte;

	public:
		StuffingDescriptor(const uint8_t * const buffer);
};

#endif /* __stuffing_descriptor_h__ */

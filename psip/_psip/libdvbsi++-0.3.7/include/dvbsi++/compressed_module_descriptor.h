/*
 * $Id$
 *
 * Copyright (C) 2004-2005 St�phane Est�-Gracias <sestegra@free.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * See the file 'COPYING' in the top level directory for details.
 */

#ifndef __compressed_module_descriptor_h__
#define __compressed_module_descriptor_h__

#include "descriptor.h"

class CompressedModuleDescriptor : public Descriptor
{
	protected:
		unsigned compressionMethod			: 8;
		unsigned originalSize				: 32;

	public:
		CompressedModuleDescriptor(const uint8_t * const buffer);

		uint8_t getCompressionMethod(void) const;
		uint32_t getOriginalSize(void) const;
};

#endif /* __compressed_module_descriptor_h__ */

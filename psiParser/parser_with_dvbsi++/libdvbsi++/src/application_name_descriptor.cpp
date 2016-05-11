/*
 * $Id$
 *
 * Copyright (C) 2004-2005 Stéphane Esté-Gracias <sestegra@free.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * See the file 'COPYING' in the top level directory for details.
 */

#include <dvbsi++/application_name_descriptor.h>

ApplicationName::ApplicationName(const uint8_t * const buffer)
{
	iso639LanguageCode.assign((char *)&buffer[0], 3);
	applicationNameLength = buffer[3];
	applicationName.assign((char *)&buffer[4], applicationNameLength);
}

const std::string &ApplicationName::getIso639LanguageCode(void) const
{
	return iso639LanguageCode;
}

const std::string &ApplicationName::getApplicationName(void) const
{
	return applicationName;
}

ApplicationNameDescriptor::ApplicationNameDescriptor(const uint8_t * const buffer) : Descriptor(buffer)
{
	/**< BENJAMIN [2011. 08. 01] Descriptor Ã³¸® ¿À·ù·Î ÀÎÇÑ ¹ö±× ¼öÁ¤ */
//	for (size_t i = 0; i < descriptorLength; i += buffer[i + 3] + 2) {
//		ASSERT_MIN_DLEN(i + buffer[i + 3] + 2);
//		applicationNames.push_back(new ApplicationName(&buffer[i + 2]));
//	}
	for (size_t i = 2; i < descriptorLength; i += buffer[i + 3] + 4) {
		ASSERT_MIN_DLEN(i + buffer[i + 3] + 2);
		applicationNames.push_back(new ApplicationName(&buffer[i]));
	}

}

ApplicationNameDescriptor::~ApplicationNameDescriptor(void)
{
	for (ApplicationNameIterator i = applicationNames.begin(); i != applicationNames.end(); ++i)
		delete *i;
}

const ApplicationNameList *ApplicationNameDescriptor::getApplicationNames(void) const
{
	return &applicationNames;
}

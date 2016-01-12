/*
 * PSIP_MasterGuideSection.h
 *
 *  Created on: 2015. 9. 24.
 *      Author: erato
 */

#ifndef PSIP_MASTERGUIDESECTION_H_
#define PSIP_MASTERGUIDESECTION_H_

#include "long_crc_section.h"
#include "PSIP_DescriptorContainer.h"

class PSIP_MasterGuideInfo;

typedef std::vector<PSIP_MasterGuideInfo*> PSIP_MasterGuideInfoList;


class PSIP_MasterGuideInfo : public PSIP_DescriptorContainer
{
protected:
	PSIP_MasterGuideInfo(const uint8_t* const buffer);
	virtual ~PSIP_MasterGuideInfo();
	friend class PSIP_MasterGuideSection;

public:
	uint16_t 	getTableType() const;
	uint8_t		getTableTypePID() const;
	uint8_t		getTableTypeVersionNumber() const;
	uint32_t	getNumberBytes() const;
	void 		dump();

protected:
	uint32_t	table_type:16;
	uint32_t	:3;
	uint32_t	table_type_PID:13;
	uint8_t		:3;
	uint8_t		table_type_version_number:5;
	uint32_t	number_bytes;
	uint16_t	:4;
	uint16_t	table_type_descriptors_length:12;
};


class PSIP_MasterGuideSection : public LongCrcSection, public PSIP_DescriptorContainer
{
public:
	PSIP_MasterGuideSection(const uint8_t * const buffer);
	virtual ~PSIP_MasterGuideSection();

public:
	static const int PID = 0x1FFB;
	static const int TID = 0xC7;

public:
	const PSIP_MasterGuideInfoList* getTableInfo() const;
	void dump();

protected:
	uint8_t 	protocol_version;
	uint16_t 	tables_defined;
	uint16_t	:4;
	uint16_t	descriptors_length:12;

protected:
	PSIP_MasterGuideInfoList mMasterGuideInfoList;
};

#endif /* PSIP_MASTERGUIDESECTION_H_ */

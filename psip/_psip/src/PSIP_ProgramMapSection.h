/*
 * PSIP_ProgramMapSection.h
 *
 *  Created on: 2015. 10. 1.
 *      Author: erato
 */

#ifndef PSIP_PROGRAMMAPSECTION_H_
#define PSIP_PROGRAMMAPSECTION_H_

#include "program_map_section.h"
#include "PSIP_DescriptorContainer.h"

class PSIP_ProgramMapSection;
class PSIP_ElementaryStreamInfo;

typedef std::vector<PSIP_ElementaryStreamInfo *> PSIP_ElementaryStreamInfoList;


class PSIP_ElementaryStreamInfo : public PSIP_DescriptorContainer
{
public:
	PSIP_ElementaryStreamInfo(const uint8_t * const buffer);
	friend class PSIP_ProgramMapSection;
	friend class CaElementaryStreamInfo;

public:
	uint8_t getType(void) const;
	uint16_t getPid(void) const;
	void dump();

protected:
	unsigned streamType			: 8;
	unsigned elementaryPid		: 13;
	unsigned esInfoLength		: 12;
};

class PSIP_ProgramMapSection : public LongCrcSection, public PSIP_DescriptorContainer
{
public:
	PSIP_ProgramMapSection(const uint8_t * const buffer);
	virtual ~PSIP_ProgramMapSection();
	friend class CaProgramMapSection;

public:
	static const enum TableId TID = TID_PMT;
	static const uint32_t TIMEOUT = 600;

public:
	uint16_t getPcrPid(void) const;
	uint16_t getProgramNumber(void) const;
	const PSIP_ElementaryStreamInfoList *getEsInfo(void) const;
	void dump();

protected:
	unsigned pcrPid				: 13;
	unsigned programInfoLength	: 12;
	PSIP_ElementaryStreamInfoList mPSIP_ElementaryStreamInfoList;
};

#endif /* PSIP_PROGRAMMAPSECTION_H_ */

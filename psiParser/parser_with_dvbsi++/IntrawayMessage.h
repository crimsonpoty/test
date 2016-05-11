/**
	*	@file		IntrawayMessage.h
	*	@date		2016. 04. 25
	*	@author		BenjaminTeam, k150030
	*	@brief		IWM 테이블 정보를 저장하는 클래스
	*/

#ifndef _INTRAWAY_MESSAGE_SECTION_H_
#define _INTRAWAY_MESSAGE_SECTION_H_

#include "Common.h"
#include <dvbsi++/long_crc_section.h>
#include <dvbsi++/descriptor.h>

class CIntrawayMessage
{
protected:
	uint8_t addressType;
	uint8_t addressLength;
	uint32_t stbMask;
	uint32_t stbId;
	uint32_t stbFirst;
	uint32_t stbLast;
	uint32_t msgId;
	uint8_t msgVersion;
	uint8_t isCompressed;
	uint8_t msgDescLength;
	DescriptorList MessageDiscriptors;
	uint8_t text[200];

public:
	CIntrawayMessage(const uint8_t* buffer);
	~CIntrawayMessage();

	uint8_t getAddressType(void) const;
	uint8_t getAddressLength(void) const;
	uint32_t getStbId(void) const;
	uint32_t getMsgId(void) const;
	uint8_t getMsgVersion(void) const;
	uint8_t getIsCompressed(void) const;
	uint8_t getMsgDescLength(void) const;

	const DescriptorList* getMessageDescriptors(void) const;
	const uint8_t* getText(void) const;
};

typedef std::list<CIntrawayMessage *> IntrawayMessageList;
typedef IntrawayMessageList::iterator IntrawayMessageIterator;
typedef IntrawayMessageList::const_iterator IntrawayMessageConstIterator;

/**
	@class		CIntraway_Message_Section
	@brief		IWM 테이블 정보를 저장하는 클래스
*/
class CIntrawayMessageSection : public LongCrcSection
{
protected:
	IntrawayMessageList intrawayMessages;

public:
	CIntrawayMessageSection(const uint8_t* const buffer);
	~CIntrawayMessageSection();

	static const int PID = 0x3E7;
	static const int TID = 0xF0;
	static const uint32_t TIMEOUT = 12000;

	const IntrawayMessageList* getIntrawayMessages(void) const;
};

typedef std::list<CIntrawayMessageSection *> IntrawayMessageSectionList;
typedef IntrawayMessageSectionList::iterator IntrawayMessageSectionIterator;
typedef IntrawayMessageSectionList::const_iterator IntrawayMessageSectionConstIterator;


#endif
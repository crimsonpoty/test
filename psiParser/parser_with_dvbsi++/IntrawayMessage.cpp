#include <dvbsi++/byte_stream.h>
#include "IntrawayMessage.h"


CIntrawayMessage::CIntrawayMessage(const uint8_t* buffer) : addressType(0), addressLength(0), stbMask(0), stbId(0), stbFirst(0), stbLast(0), msgId(0), isCompressed(0), msgDescLength(0)
{
	memset(text, 0x00, sizeof(text));
	
	addressType = (buffer[0] & 0xF0) >> 4;
	if(1 == addressType || 2 == addressType || 3 == addressType) {
		addressLength = buffer[0] & 0x0F;
	}
	
	switch(addressType)
	{
		case 0:
			addressLength = 0;
			break;
			
		case 1:
			stbMask = UINT32(&buffer[1]);
			break;

		case 2:
			stbId = UINT32(&buffer[1]);
			// stbId = 65536;
			msgId = UINT32(&buffer[5]);
			msgVersion = (buffer[9] & 0xE0) >> 5;
			isCompressed = (buffer[9] & 0x10) >> 4;
			msgDescLength = UINT16(&buffer[9]) & 0xFFF;
			// memcpy(text, &buffer[0], 11 + msgDescLength);
			break;

		case 3:
			stbFirst = UINT32(&buffer[1]);
			stbLast = UINT32(&buffer[5]);
			break;
	}
	
	// bool isMatched = false;
	// uint32_t temp[] = {0x80677957, 0x80677957, 0x6A0D4391, 0x6ED7C8D6, 0x8062D7E9, 0x6ED5546E, 0x8076DE2B, 0x6EF3C244};	
	
	// for(int i = 0; i < (int)(sizeof(temp)/sizeof(temp[0])); i++) {
		// if(msgId == temp[i]) {
			// isMatched = true;
			// break;
		// }
	// }
	
	// if(!isCompressed && isMatched) {
		// if (msgDescLength > 0) {
			// for (size_t i = 12; i < (size_t)(msgDescLength + 12) ; i += (size_t)(buffer[i + 1]) + 2) {
				// MessageDiscriptors.push_back(new Descriptor(&buffer[i]));
			// }
			// 
			// if(!memcpy(text, &buffer[12], msgDescLength)) {
				// printf("not memcpy!\n");
			// }
		// }
	// }
}

CIntrawayMessage::~CIntrawayMessage()
{
	
}

uint8_t CIntrawayMessage::getAddressType(void) const
{
	return addressType;
}

uint8_t CIntrawayMessage::getAddressLength(void) const
{
	return addressLength;
}

uint32_t CIntrawayMessage::getStbId(void) const
{
	if(2 == addressType) {
		return stbId;
	}
	
	return 0;
}

uint32_t CIntrawayMessage::getMsgId(void) const
{
	return msgId;
}

uint8_t CIntrawayMessage::getMsgVersion(void) const
{
	return msgVersion;	
}

uint8_t CIntrawayMessage::getIsCompressed(void) const
{
	return isCompressed;	
}

uint8_t CIntrawayMessage::getMsgDescLength(void) const
{
	return msgDescLength;
}

const DescriptorList* CIntrawayMessage::getMessageDescriptors(void) const
{
	return &MessageDiscriptors;
}

const uint8_t* CIntrawayMessage::getText(void) const
{
	return &text[0];
}



CIntrawayMessageSection::CIntrawayMessageSection(const uint8_t* const buffer) : LongCrcSection(buffer)
{
	uint16_t pos = 8;
	intrawayMessages.push_back(new CIntrawayMessage(&buffer[pos]));
}

CIntrawayMessageSection::~CIntrawayMessageSection()
{
	for(IntrawayMessageIterator it = intrawayMessages.begin(); it != intrawayMessages.end(); ++it) {
		delete *it;
	}
}

const IntrawayMessageList* CIntrawayMessageSection::getIntrawayMessages(void) const
{
	return &intrawayMessages;
}

#include <dvbsi++/byte_stream.h>
#include <LzmaDec.h>
#include <Alloc.h> /**< For MyAlloc, MyFree */

#include "IntrawayMessage.h"


static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

const int OUT_BUF_SIZE = 65536;

void UncompressLZMAStream(const uint8_t* InBuffer, std::vector<uint8_t>& vOutBuffer, size_t Length)
{
	vOutBuffer.clear();
	const uchar DummySizeBufferFixHeader[13] = {0x5D, 0x00, 0x10, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	uint32_t LzmaCompressedDataLength = Length - 5;
	uint8_t *EditedLzmaCompressedData = new uint8_t[LzmaCompressedDataLength];
	memcpy(EditedLzmaCompressedData, &InBuffer[5], LzmaCompressedDataLength);

	UInt64 unpackSize = 0;
	CLzmaDec state;

	for (int i = 0; i < 8; i++) {
		unpackSize += (UInt64)DummySizeBufferFixHeader[LZMA_PROPS_SIZE + i] << (i * 8);
	}

	LzmaDec_Construct(&state);
	LzmaDec_Allocate(&state, DummySizeBufferFixHeader, LZMA_PROPS_SIZE, &g_Alloc);

	int thereIsSize = (unpackSize != (UInt64)(UInt64)-1);
	Byte *outBuf = new Byte[OUT_BUF_SIZE];
	size_t inPos = 0, inSize = LzmaCompressedDataLength;
	LzmaDec_Init(&state);

	for (;;) {
		SRes res;
		SizeT inProcessed = inSize - inPos;
		SizeT outProcessed = OUT_BUF_SIZE;
		ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
		ELzmaStatus status;
		if (thereIsSize && outProcessed > unpackSize) {
			outProcessed = (SizeT)unpackSize;
			finishMode = LZMA_FINISH_END;
		}
		// Just Treat Only Compressed Data Part(Not include Header)
		res = LzmaDec_DecodeToBuf(&state, outBuf, &outProcessed,
				EditedLzmaCompressedData + inPos, &inProcessed, finishMode, &status);
		inPos += inProcessed;
		unpackSize -= outProcessed;

		std::vector<uint8_t>::iterator it = vOutBuffer.end();
		vOutBuffer.insert(it, outBuf , outBuf + outProcessed);

		if ((res != SZ_OK || thereIsSize) && unpackSize == 0) {
			break;
		}

		if (inProcessed == 0 && outProcessed == 0) {
			break;
		}
	}

	delete [] outBuf;
	LzmaDec_Free(&state, &g_Alloc);
	delete [] EditedLzmaCompressedData;
}


CIntrawayMessage::CIntrawayMessage(const uint8_t* buffer) : addressType(0), addressLength(0), stbMask(0), stbId(0), stbFirst(0), stbLast(0), msgId(0), isCompressed(0), msgDescLength(0)
{
	uint16_t pos = 0;

	addressType = (buffer[0] & 0xF0) >> 4;

	if(1 == addressType || 2 == addressType || 3 == addressType) {
		addressLength = buffer[0] & 0x0F;
	}
	else if(0 == addressType) {
		addressLength = 0;
	}
	else {
		printf("addressType is wrong!(%u)\n", addressType);
		return;
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

			// stbId 선별
			if(0x809415D0 != stbId) {
				return;
			}
			else {
				// printf("Discovered matched stbId(%u)\n", stbId);
			}

			msgId = UINT32(&buffer[5]);
			// msgVersion = (buffer[9] & 0xE0) >> 5;
			// isCompressed = (buffer[9] & 0x10) >> 4;
			// msgDescLength = UINT16(&buffer[9]) & 0xFFF;
			msgVersion = (UINT16(&buffer[9]) & 0xE000) >> 13;
			isCompressed = (UINT16(&buffer[9]) & 0x1000) >> 12;
			msgDescLength = DVB_LENGTH(&buffer[9]);
			pos = 11;
			break;

		case 3:
			stbFirst = UINT32(&buffer[1]);
			stbLast = UINT32(&buffer[5]);
			break;
	}
	
	if(2 != addressType) {
		printf("addressType is wrong!(%u)\n", addressType);
		return;
	}

	bool isMatched = false;
	uint32_t matchingId[] = {34287250};

	for(int i = 0; i < (int)(sizeof(matchingId)/sizeof(matchingId[0])); i++) {
		if(msgId == matchingId[i]) {
			isMatched = true;
			printf("Match(%u), isCompressed(%u)!!!\n", msgId, isCompressed);
			break;
		}
	}

	if(isMatched) {
		uint8_t *uncompressedData = NULL;
		uint32_t uncompressedDataLength = msgDescLength;

		if(isCompressed) {
			if(msgDescLength < 5 ) {
				printf("Desc Length is wrong!\n");
				return;
			}

			std::vector<uint8_t> vCompressedData;
			UncompressLZMAStream(&buffer[pos], vCompressedData, msgDescLength);
			uncompressedDataLength = vCompressedData.size();
			uncompressedData = new uint8_t[uncompressedDataLength];
			copy(vCompressedData.begin(), vCompressedData.end(), uncompressedData);
		}
		else {
			uncompressedData = (uint8_t*)(&buffer[pos]);
		}

		printf("msgId(%u), msgDescLength(%u), uncompressedData size(%u)\n", msgId, msgDescLength, uncompressedDataLength);

		
		if(uncompressedDataLength > 0) {
			for (uint32_t i = 0; i < uncompressedDataLength; i += (uncompressedData[i + 1]) + 2) {
				MessageDiscriptors.push_back(new Descriptor(&uncompressedData[i]));
			}
		}
		
		for(DescriptorConstIterator desc = MessageDiscriptors.begin(); desc != MessageDiscriptors.end(); desc++) {
			printf("MsgId(%d), Tag(%X)\n", msgId, (*desc)->getTag());
		}
		
		if(isCompressed) {
			delete [] uncompressedData;
		}
	}
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

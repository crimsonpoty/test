#include "Common.h"
#include "VirtualChannelTable.h"

#include <vector>


namespace App {


CVirtualChannelTable::CVirtualChannelTable()
{
	// printf("======= CVirtualChannelTable::CVirtualChannelTable() Default Constructor =======\n");
	Init();
}

CVirtualChannelTable::CVirtualChannelTable(const uchar *Buffer, int NumberOfChannels)
{
	if(Buffer[0] == 0xC8 || Buffer[0] == 0xC9) {
		printf("CVirtualChannelTable::GetChannel, TableID=%2X is invalid", Buffer[0]);
	}

	if( NumberOfChannels >= Buffer[PSIP_TABLE_DATA_OFFSET] )
	{
		printf("CVirtualChannelTable::GetChannel, NumberOfChannels=%d is Invalid", NumberOfChannels);
	}

	int ByteOffset = GetChannelByteOffset(Buffer, NumberOfChannels);

	for(int i = 0; i < 7; i++ )
	{
		m_ShortName[i] = TS_READ_16(&Buffer[ByteOffset+(i*2)]);
	}
	m_MajorChannelNumber = (ushort)((TS_READ_16(&Buffer[ByteOffset + 14] ) >> 2) & 0x3FF);
	m_MinorChannelNumber = (ushort)(TS_READ_16(&Buffer[ByteOffset + 15]) & 0x3FF);
	m_ModulationMode = (EPSIP_VCT_MODULATION_MODE)Buffer[ByteOffset + 17];
	m_CarrierFrequency = TS_READ_32(&Buffer[ByteOffset + 18]);
	m_ChannelTsId = TS_READ_16(&Buffer[ByteOffset + 22]);
	m_ProgramNumber = TS_READ_16(&Buffer[ByteOffset + 24]);
	m_EtmLocation = (EPSIP_ETM_LOCATION)((Buffer[ByteOffset + 26] >> 6) & 3);
	m_AccessControlled = (uchar)((Buffer[ByteOffset + 26] >> 5) & 1);
	m_Hidden = (Buffer[ByteOffset + 26] >> 4) & 1;
	m_PathSelect = (Buffer[ByteOffset + 26] >> 3) & 1;
	m_OutOfBand = (Buffer[ByteOffset + 26] >> 2) & 1;
	m_HideGuide = (Buffer[ByteOffset + 26] >> 1) & 1;
	m_ServiceType = (EPSIP_VCT_SERVICE_TYPE)(Buffer[ByteOffset + 27] & 0x3F);
	m_SourceId = TS_READ_16(&Buffer[ByteOffset + 28]);

	m_IsParse = true;	// k150030(ywkim) comment : 한번만 파싱하기 위한 플래그
}

CVirtualChannelTable::~CVirtualChannelTable()
{
}

void CVirtualChannelTable::Init()
{
	m_IsParse = false;
}

void CVirtualChannelTable::PrintContent()
{
	printf("m_ShortName=%s\n", (char*)m_ShortName);
	printf("m_MajorChannelNumber=%d\n", m_MajorChannelNumber);
	printf("m_MinorChannelNumber=%d\n", m_MinorChannelNumber);
	printf("m_ModulationMode=%d\n", m_ModulationMode);
	printf("m_CarrierFrequency=%d\n", m_CarrierFrequency);
	printf("m_ChannelTsId=%d\n", m_ChannelTsId);
	printf("m_ProgramNumber=%d\n", m_ProgramNumber);
	printf("m_EtmLocation=%d\n", m_EtmLocation);
	printf("m_AccessControlled=%d\n", m_AccessControlled);
	printf("m_Hidden=%d\n", m_Hidden);
	printf("m_PathSelect=%d\n", m_PathSelect);
	printf("m_OutOfBand=%d\n", m_OutOfBand);
	printf("m_HideGuide=%d\n", m_HideGuide);
	printf("m_ServiceType=%d\n", m_ServiceType);
	printf("m_SourceId=%d\n", m_SourceId);
}

bool CVirtualChannelTable::GetIsParse()
{
	return m_IsParse;
}

uchar GetProtocolVersion(const uchar *Buffer)
{
	return Buffer[PSIP_PROTOCOL_VERSION_OFFSET];
}

int CVirtualChannelTable::GetChannelByteOffset(const uchar *Buffer, int NumberOfChannels)
{
	uchar i;
	int ByteOffset = PSIP_TABLE_DATA_OFFSET + 1;

	if(NumberOfChannels == -1)
	{
		NumberOfChannels = Buffer[PSIP_TABLE_DATA_OFFSET];
	}

	/* Jump to correct table (or first byte after last table) */
	for(i = 0; i < NumberOfChannels; i++)
	{
		ByteOffset += 32 + (TS_READ_16(&Buffer[ByteOffset+30]) & 0x03FF);

		if(ByteOffset <= TS_PSI_MAX_BYTE_OFFSET(Buffer)) {
			printf("CVirtualChannelTable::GetChannelByteOffset, ByteOffset=%d is invalid", ByteOffset);
		}
	}

	return ByteOffset;
}

uchar CVirtualChannelTable::GetNumChannels(const uchar *Buffer)
{
	if(Buffer[0] == 0xC8 || Buffer[0] == 0xC9) {
		return Buffer[PSIP_TABLE_DATA_OFFSET];
	}
	else {
		printf("CVirtualChannelTable::GetNumChannels, TableID=%2X is invalid", Buffer[0]);
		return 0;
	}
}

TS_PSI_Descriptor CVirtualChannelTable::GetAdditionalDescriptor(const uchar *Buffer, int NumberOfDescriptor)
{
	if(Buffer[0] == 0xC8 || Buffer[0] == 0xC9) {
		printf("CVirtualChannelTable::GetNumChannels, TableID=%2X is invalid", Buffer[0]);
		return NULL;
	}

	int ByteOffset = GetChannelByteOffset(Buffer, -1);

	// k150030(ywkim) test: temporary NULL return
	return NULL;
	// uint DescriptorLength = TS_READ_16(&Buffer[ByteOffset]) & 0x03FF;
	// return TS_P_getDescriptor(&Buffer[ByteOffset+2], DescriptorLength, NumberOfDescriptor);
}
/*
uint CVirtualChannelTable::GetChannel(const uchar *Buffer, int NumberOfChannels)
{
	if(Buffer[0] == 0xC8 || Buffer[0] == 0xC9) {
		printf("CVirtualChannelTable::GetChannel, TableID=%2X is invalid", Buffer[0]);
		return 1;
	}

	if( NumberOfChannels >= Buffer[PSIP_TABLE_DATA_OFFSET] )
	{
		// k150030(ywkim) test: temporary 0 return
		return 1;
	}

	int ByteOffset = GetChannelByteOffset(Buffer, NumberOfChannels);

	for(int i = 0; i < 7; i++ )
	{
		m_ShortName[i] = TS_READ_16(&Buffer[ByteOffset+(i*2)]);
	}
	m_MajorChannelNumber = (ushort)((TS_READ_16(&Buffer[ByteOffset + 14] ) >> 2) & 0x3FF);
	m_MinorChannelNumber = (ushort)(TS_READ_16(&Buffer[ByteOffset + 15]) & 0x3FF);
	m_ModulationMode = (EPSIP_VCT_MODULATION_MODE)Buffer[ByteOffset + 17];
	m_CarrierFrequency = TS_READ_32(&Buffer[ByteOffset + 18]);
	m_ChannelTsId = TS_READ_16(&Buffer[ByteOffset + 22]);
	m_ProgramNumber = TS_READ_16(&Buffer[ByteOffset + 24]);
	m_EtmLocation = (EPSIP_ETM_LOCATION)((Buffer[ByteOffset + 26] >> 6) & 3);
	m_AccessControlled = (uchar)((Buffer[ByteOffset + 26] >> 5) & 1);
	m_Hidden = (Buffer[ByteOffset + 26] >> 4) & 1;
	m_PathSelect = (Buffer[ByteOffset + 26] >> 3) & 1;
	m_OutOfBand = (Buffer[ByteOffset + 26] >> 2) & 1;
	m_HideGuide = (Buffer[ByteOffset + 26] >> 1) & 1;
	m_ServiceType = (EPSIP_VCT_SERVICE_TYPE)(Buffer[ByteOffset + 27] & 0x3F);
	m_SourceId = TS_READ_16(&Buffer[ByteOffset + 28]);

	return KERR_SUCCESS; //temp
}
*/
TS_PSI_Descriptor CVirtualChannelTable::GetChannelDescriptor(const uchar *Buffer, int NumberOfChannels, int NumberOfDescriptor)
{
	if(Buffer[0] == 0xC8 || Buffer[0] == 0xC9) {
		printf("CVirtualChannelTable::GetChannelDescriptor, TableID=%2X is invalid", Buffer[0]);
		return NULL;
	}

	int ByteOffset = GetChannelByteOffset(Buffer, NumberOfChannels);

	// k150030(ywkim) test: temporary NULL return
	return NULL;
	// uint DescriptorLength = TS_READ_16(&Buffer[ByteOffset+30]) & 0x3FF;
	// return TS_P_getDescriptor(&Buffer[ByteOffset+32], DescriptorLength, NumberOfDescriptor);
}


} // namespace App

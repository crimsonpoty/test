/**
 @file		VirtualChannelTable.h
 @date		2015. 07. 15
 @author	k150030(ywkim@kaonmedia.com)
 @brief		VirtualChannelTable 임시 구현
 @remarks
*/

#ifndef _VIRTUAL_CHANNEL_TABLE_H_
#define _VIRTUAL_CHANNEL_TABLE_H_

#include "DataType.h"


namespace App {

#define TS_READ_16(Buffer) ((ushort)((Buffer)[0] << 8 | (Buffer)[1]))
#define TS_READ_32(Buffer) ((uint)((Buffer)[0] << 24 | (Buffer)[1] << 16 | (Buffer)[2] << 8 | (Buffer)[3]))

#define TS_PSI_TABLE_ID_OFFSET				0
#define TS_PSI_SECTION_LENGTH_OFFSET		1
#define TS_PSI_TABLE_ID_EXT_OFFSET			3
#define TS_PSI_CNI_OFFSET					5
#define TS_PSI_SECTION_NUMBER_OFFSET		6
#define TS_PSI_LAST_SECTION_NUMBER_OFFSET	7

#define TS_PSI_GET_SECTION_LENGTH(Buffer)	(ushort)(TS_READ_16(&(Buffer)[TS_PSI_SECTION_LENGTH_OFFSET]) & 0x0FFF)
#define TS_PSI_MAX_BYTE_OFFSET(Buffer)		(TS_PSI_GET_SECTION_LENGTH(Buffer) - 1)

#define PSIP_PROTOCOL_VERSION_OFFSET	8
#define PSIP_TABLE_DATA_OFFSET			9


enum EPSIP_VCT_MODULATION_MODE
{
	ePSIP_VCT_MODULATION_MODE_ANALOG,
	ePSIP_VCT_MODULATION_MODE_SCTE_MODE_1,
	ePSIP_VCT_MODULATION_MODE_SCTE_MODE_2,
	ePSIP_VCT_MODULATION_MODE_ATSC_8,
	ePSIP_VCT_MODULATION_MODE_ATSC_16
};

enum EPSIP_ETM_LOCATION
{
	ePSIP_ETM_LOCATION_NO_ETM,
	ePSIP_ETM_LOCATION_ETM,
	ePSIP_ETM_LOCATION_ETM_CHANNEL_TSID
};

enum EPSIP_VCT_SERVICE_TYPE
{
	ePSIP_VCT_SERVICE_ANALOG_TELEVISION = 1,
	ePSIP_VCT_SERVICE_ATSC_DIGITAL_TELEVISION,
	ePSIP_VCT_SERVICE_ATSC_AUDIO,
	ePSIP_VCT_SERVICE_ATSC_DATA_ONLY_SERVICE
};

// k150030(ywkim) temp typedef
typedef const uint* TS_PSI_Descriptor;
typedef const uchar* PSIP_MSS_string;


/**
 @class		CVirtualChannelTable
 @brief
 @remarks
 */
class CVirtualChannelTable
{
public:
	/** Default Constructor & Destructor */
	CVirtualChannelTable();
	CVirtualChannelTable(const uchar *Buffer, int NumberOfChannels);
	virtual ~CVirtualChannelTable();
	/** I will use default Copy Constructor & Assignment Operator */
	/** CVirtualChannelTable(const CVirtualChannelTable &s); */
	/** CVirtualChannelTable& operator=(const CVirtualChannelTable &s); */

	/** Operations */
public:
	// uint/*BERR_Code*/ GetChannel(const uchar *Buffer, int channelNum);
	TS_PSI_Descriptor GetChannelDescriptor(const uchar *Buffer, int channelNum, int descriptorNum);
	void PrintContent();
	bool GetIsParse();

protected:
private:
	void Init();
	uchar GetProtocolVersion(const uchar *Buffer);
	int GetChannelByteOffset(const uchar *Buffer, int ChannelNumber);
	uchar GetNumChannels(const uchar *Buffer);
	TS_PSI_Descriptor GetAdditionalDescriptor(const uchar *Buffer, int NumberOfDescriptor);

	/** Attribute */
public:
protected:
private:
	ushort					m_ShortName[7];
	ushort					m_MajorChannelNumber;
	ushort					m_MinorChannelNumber;
	EPSIP_VCT_MODULATION_MODE m_ModulationMode;
	uint					m_CarrierFrequency;
	ushort					m_ChannelTsId;
	ushort					m_ProgramNumber;
	EPSIP_ETM_LOCATION		m_EtmLocation;
	ushort					m_AccessControlled;
	bool					m_Hidden;
	bool					m_PathSelect;		/* valid for cable VCT only */
	bool					m_OutOfBand;		/* valid for cable VCT only */
	bool					m_HideGuide;
	EPSIP_VCT_SERVICE_TYPE	m_ServiceType;
	ushort					m_SourceId;

	bool					m_IsParse;
};

} // namespace App
#endif // _VIRTUAL_CHANNEL_TABLE_H_

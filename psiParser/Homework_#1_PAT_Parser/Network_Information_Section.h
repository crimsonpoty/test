/**
	@file		Network_Information_Section.h
	@date		2015. 03. 24
	@author		BenjaminTeam, k150030
	@brief		NIT 테이블의 정보를 저장하는 클래스
*/

#ifndef _NETWORK_INFORMATION_SECTION_H_
#define _NETWORK_INFORMATION_SECTION_H_

#include "Common.h"
#include "Long_Section.h"
#include "Transport_Stream.h"

using namespace std;

/**
	@class		CNetwork_Infomation_Section
	@brief		NIT 테이블의 정보를 저장하는 클래스
*/
class CNetwork_Infomation_Section
{
	public:
	CNetwork_Infomation_Section() {};
	~CNetwork_Infomation_Section() {};

	public:
	void Parse(const char *buffer);
	void Print();


	public:
	uchar get_Table_ID();
	bool get_Section_Syntax_Indicator();
	ushort get_Section_Length();
	ushort get_Network_ID();
	uchar get_Version_Number();
	bool get_Current_Next_Indicator();
	uchar get_Section_Number();
	uchar get_Last_Section_Number();
	ushort get_Network_Descriptor_Length();

	protected:
	uchar m_Table_ID;
	bool m_Section_Syntax_Indicator;	///통상적으로 '1'이 세팅됨
	ushort m_Section_Length;			///섹션의 길이(bytes)를 나타낸다(10bit, 처음 2bit는 '00', CRC포함, 1021(0x3FD)을 초과할 수는 없음)
	ushort m_Network_ID;
	uchar m_Version_Number;				///전체 PAT의 버전 번호이며, 변경사항이 있을 때마다 1씩 증가함, current_next_indicator와 관련이 있음
	bool m_Current_Next_Indicator;		///'1'이면 현재 적용중, '0'이면 적용예정(다음에 적용)
	uchar m_Section_Number;
	uchar m_Last_Section_Number;
	ushort m_Network_Descriptor_Length;
	//CDescriptor Descriptor;

	ushort m_Transport_Stream_Loop_Length;
	list<CTransport_Stream> m_TS_List;
	list<CTransport_Stream>::iterator iter_TS_List;

	// ushort m_Transport_Stream_ID;
	// ushort m_Original_Network_ID;
	// ushort m_Transport_Descriptor_Length;

	uint32 m_CRC_32;

	private:
	bool isParse;
	char *ptr_Parse;

	public:
	bool checkParse();
};

#endif
/**
	@file		Network_Information_Section.h
	@date		2015. 03. 24
	@author		BenjaminTeam, k150030
	@brief		NIT ���̺��� ������ �����ϴ� Ŭ����
*/

#ifndef _NETWORK_INFORMATION_SECTION_H_
#define _NETWORK_INFORMATION_SECTION_H_

#include "Common.h"
#include "Long_Section.h"
#include "Transport_Stream.h"

using namespace std;

/**
	@class		CNetwork_Infomation_Section
	@brief		NIT ���̺��� ������ �����ϴ� Ŭ����
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
	bool m_Section_Syntax_Indicator;	///��������� '1'�� ���õ�
	ushort m_Section_Length;			///������ ����(bytes)�� ��Ÿ����(10bit, ó�� 2bit�� '00', CRC����, 1021(0x3FD)�� �ʰ��� ���� ����)
	ushort m_Network_ID;
	uchar m_Version_Number;				///��ü PAT�� ���� ��ȣ�̸�, ��������� ���� ������ 1�� ������, current_next_indicator�� ������ ����
	bool m_Current_Next_Indicator;		///'1'�̸� ���� ������, '0'�̸� ���뿹��(������ ����)
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
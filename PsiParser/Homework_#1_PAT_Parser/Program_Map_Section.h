/**
	*	@file		Program_Map_Section.h
	*	@date		2015. 03. 24
	*	@author		BenjaminTeam, k150030
	*	@brief		PMT ���̺��� ������ �����ϴ� Ŭ����
	*/

#ifndef _PROGRAM_MAP_SECTION_H_
#define _PROGRAM_MAP_SECTION_H_

#include "Common.h"
#include "Program.h"
#include "Descriptor.h"
#include "ES_Info.h"

using namespace std;

/**
	@class		CProgram_Map_Section
	@brief		PMT ���̺��� ������ �����ϴ� Ŭ����
*/
class CProgram_Map_Section : public CProgram
{
	public:
	CProgram_Map_Section() {};
	~CProgram_Map_Section() {};
	CProgram_Map_Section(const char *buffer);

	public:
	void Parse(const char *buffer);
	void Print();

	public:
	uchar get_Table_ID();
	bool get_Section_Syntax_Indicator();
	ushort get_Section_Length();
	ushort get_Program_Number();
	uchar get_Version_Number();
	bool get_Current_Next_Indicator();
	uchar get_Section_Number();
	uchar get_Last_Section_Number();
	ushort get_PCR_PID();
	ushort get_Program_Info_Length();

	protected:
	uchar m_Table_ID;
	bool m_Section_Syntax_Indicator;	///��������� '1'�� ���õ�
	ushort m_Section_Length;			///������ ����(bytes)�� ��Ÿ����(10bit, ó�� 2bit�� '00', CRC����, 1021(0x3FD)�� �ʰ��� ���� ����)
	ushort m_Program_Number;
	uchar m_Version_Number;				///��ü PAT�� ���� ��ȣ�̸�, ��������� ���� ������ 1�� ������, current_next_indicator�� ������ ����
	bool m_Current_Next_Indicator;		///'1'�̸� ���� ������, '0'�̸� ���뿹��(������ ����)
	uchar m_Section_Number;
	uchar m_Last_Section_Number;
	ushort m_PCR_PID;
	ushort m_Program_Info_Length;
	//CDescriptor m_Descriptor();
	uint32 m_CRC_32;

	public:
	list<CES_Info> m_ES_Info_List;
	list<CES_Info>::iterator iter_ES_Info_List;

	private:
	bool isParse;
	char *ptr_Parse;

	public:
	bool checkParse();
};

#endif
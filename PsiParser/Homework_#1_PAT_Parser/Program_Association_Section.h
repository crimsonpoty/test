/**
	*	@file		Program_Association_Section.h
	*	@date		2015. 03. 24
	*	@author		BenjaminTeam, k150030
	*	@brief		PAT ���̺� ������ �����ϴ� Ŭ����
	*/

#ifndef _PROGRAM_ASSOCIATION_SECTION_H_
#define _PROGRAM_ASSOCIATION_SECTION_H_

#include "Common.h"
#include "Program.h"

using namespace std;

/**
	@class		CProgram_Association_Section
	@brief		PAT ���̺� ������ �����ϴ� Ŭ����
*/
class CProgram_Association_Section : public CProgram
{
	public:
	CProgram_Association_Section() {};
	~CProgram_Association_Section() {};
	// CProgram_Association_Section(const char *buffer);

	public:
	void Parse(const char *buffer);
	void Parse_Programs(const char *buffer);
	void Print();

	public:
	uchar get_Table_ID();
	bool get_Section_Syntax_Indicator();
	ushort get_Section_Length();
	ushort get_Transport_Stream_ID();
	uchar get_Version_Number();
	bool get_Current_Next_Indicator();
	uchar get_Section_Number();
	uchar get_Last_Section_Number();
	uint32 get_CRC_32();

	public:
	int get_Number_of_Programs();

	protected:
	uchar m_Table_ID;
	bool m_Section_Syntax_Indicator;	///��������� '1'�� ���õ�
	ushort m_Section_Length;			///������ ����(bytes)�� ��Ÿ����(10bit, ó�� 2bit�� '00', CRC����, 1021(0x3FD)�� �ʰ��� ���� ����)
	ushort m_Transport_Stream_ID;
	uchar m_Version_Number;				///��ü PAT�� ���� ��ȣ�̸�, ��������� ���� ������ 1�� ������, current_next_indicator�� ������ ����
	bool m_Current_Next_Indicator;		///'1'�̸� ���� ������, '0'�̸� ���뿹��(������ ����)
	uchar m_Section_Number;				///���� ��ȣ, If ù��° �����̸� 0x00, PAT�� �� �߰��Ǵ� ���Ǹ��� 1�� ����
	uchar m_Last_Section_Number;
	uint32 m_CRC_32;

	public:
	list<CProgram> m_Program_List;
	list<CProgram>::iterator iter_Program_List;
	int m_Number_of_Programs;

	private:
	bool isParse;

	public:
	bool checkParse();
};

#endif
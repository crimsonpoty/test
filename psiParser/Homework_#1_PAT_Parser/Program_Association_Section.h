/**
	*	@file		Program_Association_Section.h
	*	@date		2015. 03. 24
	*	@author		BenjaminTeam, k150030
	*	@brief		PAT 테이블 정보를 저장하는 클래스
	*/

#ifndef _PROGRAM_ASSOCIATION_SECTION_H_
#define _PROGRAM_ASSOCIATION_SECTION_H_

#include "Common.h"
#include "Program.h"

using namespace std;

/**
	@class		CProgram_Association_Section
	@brief		PAT 테이블 정보를 저장하는 클래스
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
	bool m_Section_Syntax_Indicator;	///통상적으로 '1'이 세팅됨
	ushort m_Section_Length;			///섹션의 길이(bytes)를 나타낸다(10bit, 처음 2bit는 '00', CRC포함, 1021(0x3FD)을 초과할 수는 없음)
	ushort m_Transport_Stream_ID;
	uchar m_Version_Number;				///전체 PAT의 버전 번호이며, 변경사항이 있을 때마다 1씩 증가함, current_next_indicator와 관련이 있음
	bool m_Current_Next_Indicator;		///'1'이면 현재 적용중, '0'이면 적용예정(다음에 적용)
	uchar m_Section_Number;				///섹션 번호, If 첫번째 섹션이면 0x00, PAT의 각 추가되는 섹션마다 1씩 증가
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
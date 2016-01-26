/**
	*	@file		Program.h
	*	@date		2015. 03. 24
	*	@author		BenjaminTeam, k150030
	*	@brief		PAT ���̺��� ���α׷� ������ �����ϴ� Ŭ����
	*/

#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "Common.h"

/**
	@class		CProgram
	@brief		PAT ���̺��� ���α׷� ������ �����ϴ� Ŭ����
*/
class CProgram
{
	public:
	CProgram() {};
	~CProgram() {};
	// CProgram(const ushort ProNum, const ushort PID);
	CProgram(const char *buffer);

	public:
	void Parse(const char *buffer);
	void set_Program(const ushort ProNum, const ushort PID);
	void Print();
	ushort get_Program_Number();
	ushort get_Program_Map_PID();

	protected:
	ushort m_Program_Number;
	union {
		ushort m_Network_PID;
		ushort m_Program_Map_PID;
	};
};

#endif
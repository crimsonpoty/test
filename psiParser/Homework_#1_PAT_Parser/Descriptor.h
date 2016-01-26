/**
	*	@file		Descriptor.h
	*	@date		2015. 03. 26
	*	@author		BenjaminTeam, k150030
	*	@brief		�����(Descriptor) ������ �����ϴ� Ŭ����
	*/

#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#include "Common.h"

/**
	@class		CDescriptor
	@brief		������� ������ �����ϴ� Ŭ����
*/
class CDescriptor
{
	public:
	CDescriptor() {};
	~CDescriptor() {};
	CDescriptor(const char *buffer) {
		Parse(buffer);
	}

	public:
	void Parse(const char *buffer);
	void Print();
	ushort get_Descriptor_Tag();
	ushort get_Descriptor_Length();

	protected:
	uchar m_Descriptor_Tag;
	uchar m_Descriptor_Length;
};

#endif
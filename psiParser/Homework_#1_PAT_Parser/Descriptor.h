/**
	*	@file		Descriptor.h
	*	@date		2015. 03. 26
	*	@author		BenjaminTeam, k150030
	*	@brief		기술자(Descriptor) 정보를 저장하는 클래스
	*/

#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#include "Common.h"

/**
	@class		CDescriptor
	@brief		기술자의 정보를 저장하는 클래스
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
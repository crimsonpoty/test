/**
	*	@file		ES_Info.h
	*	@date		2015. 03. 26
	*	@author		BenjaminTeam, k150030
	*	@brief		Elementary_Stream 정보를 저장하는 클래스
	*/

#ifndef _ELEMENTARY_STREAM_INFO_H_
#define _ELEMENTARY_STREAM_INFO_H_

#include "Common.h"
#include "Descriptor.h"

/**
	@class		CES_Info
	@brief		Elementary_Stream 정보를 저장하는 클래스
*/
class CES_Info
{
	public:
	CES_Info() {};
	~CES_Info() {};
	CES_Info(const char *buffer) {
		Parse(buffer);
	}

	public:
	void Parse(const char *buffer) {
		m_Stream_Type = buffer[0];
		m_Elementary_PID = DVB_PID(&buffer[1]);
		m_ES_Info_Length = bit_swap_16(&buffer[3]) & 0xFFF;

		//ES_descriptor();
		// ptr_Parse += m_ES_Info_Length;
	}
	void Print() {
		PARSER_PRINT(("\tStream_Type: \t\t%1$#x (%1$d)\n", m_Stream_Type))
		PARSER_PRINT(("\tElementary_PID: \t%1$#x (%1$d)\n", m_Elementary_PID))
		PARSER_PRINT(("\tES_Info_Length: \t%1$#x (%1$d)\n\n", m_ES_Info_Length))

		//printf_ES_descriptor
	}
	ushort get_ES_Info_Length() {
		return m_ES_Info_Length;
	}

	protected:
	uchar m_Stream_Type;
	ushort m_Elementary_PID;
	ushort m_ES_Info_Length;
	char *ptr_Parse;
};

#endif
/**
	*	@file		Transport_Stream.h
	*	@date		2015. 03. 31
	*	@author		BenjaminTeam, k150030
	*	@brief		NIT 테이블 내의 Transport Stream 정보를 저장하는 클래스
	*/

#ifndef _TRANSPORT_STREAM_H_
#define _TRANSPORT_STREAM_H_

#include "Common.h"
#include "Descriptor.h"

/**
	@class		CTransport_Stream
	@brief		NIT 테이블 내의 Transport Stream 정보를 저장하는 클래스
*/
class CTransport_Stream
{
	public:
	CTransport_Stream() {};
	~CTransport_Stream() {};
	CTransport_Stream(const char *buffer) {
		Parse(buffer);
	}

	public:
	void Parse(const char *buffer) {
		m_Transport_Stream_ID = bit_swap_16(&buffer[0]);
		m_Original_Network_ID = bit_swap_16(&buffer[2]);
		m_Transport_Descriptor_Length = bit_swap_16(&buffer[4]) & 0xFFF;

		ptr_Parse = (char *)&buffer[6];
		//m_Descriptor
		ptr_Parse += m_Transport_Descriptor_Length;
	}
	void Print() {
		PARSER_PRINT(("\tm_Transport_Stream_ID: \t\t%1$#x (%1$d)\n", m_Transport_Stream_ID))
		PARSER_PRINT(("\tm_Original_Network_ID: \t\t%1$#x (%1$d)\n", m_Original_Network_ID))
		PARSER_PRINT(("\tm_Transport_Descriptor_Length: \t%1$#x (%1$d)\n\n", m_Transport_Descriptor_Length))
		//m_Descriptor
	}
	// ushort get_Transport_Stream_ID();
	// ushort get_Original_Network_ID();
	ushort get_Transport_Descriptor_Length() {
		return m_Transport_Descriptor_Length;
	}

	protected:
	ushort m_Transport_Stream_ID;
	ushort m_Original_Network_ID;
	ushort m_Transport_Descriptor_Length;
	CDescriptor m_Descriptor;

	private:
	char *ptr_Parse;
};

#endif
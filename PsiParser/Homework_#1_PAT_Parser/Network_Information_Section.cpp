#include "Network_Information_Section.h"

void CNetwork_Infomation_Section::Parse(const char *buffer) {
	ptr_Parse = (char *)buffer;
	
	m_Table_ID = ptr_Parse[0];
	m_Section_Syntax_Indicator = (ptr_Parse[1] & 0x80) >> 7;
	m_Section_Length = bit_swap_16(&ptr_Parse[1]) & 0xFFF;
	m_Network_ID = bit_swap_16(&ptr_Parse[3]);
	m_Version_Number = (ptr_Parse[5] & 0x3E) >> 1;
	m_Current_Next_Indicator = ptr_Parse[5] & 1;
	m_Section_Number = ptr_Parse[6];
	m_Last_Section_Number = ptr_Parse[7];
	m_Network_Descriptor_Length = bit_swap_16(&ptr_Parse[8]) & 0xFFF;
	ptr_Parse += 10;
	//Descriptor();
	ptr_Parse += m_Network_Descriptor_Length;

	m_Transport_Stream_Loop_Length = bit_swap_16(&ptr_Parse[0]) & 0xFFF;
	ptr_Parse += 2;

	//TS_Loop_Size : m_Transport_Stream_Loop_Length 이후부터 CRC 사이의 길이
	int TS_Loop_Size = m_Transport_Stream_Loop_Length;
	iter_TS_List = m_TS_List.begin();
	do {
		m_TS_List.push_back(CTransport_Stream(ptr_Parse));
		iter_TS_List++;
		TS_Loop_Size -= (6 + iter_TS_List->get_Transport_Descriptor_Length());
		ptr_Parse += (6 + iter_TS_List->get_Transport_Descriptor_Length());
	} while(TS_Loop_Size > 0);

	//Descriptor();	
	char_buffer_dump(buffer, m_Section_Length, 2);

	isParse = true;
}

void CNetwork_Infomation_Section::Print() {
	PARSER_PRINT(("--------------------- NIT ---------------------\n\n"))
	PARSER_PRINT(("Table_ID: \t\t\t%1$#x (%1$d)\n", m_Table_ID))
	PARSER_PRINT(("Section_Syntax_Indicator: \t%1$#x (%1$d)\n", m_Section_Syntax_Indicator))
	PARSER_PRINT(("Section_Length: \t\t%1$#x (%1$d)\n", m_Section_Length))
	PARSER_PRINT(("Network_ID: \t\t\t%1$#x (%1$d)\n", m_Network_ID))
	PARSER_PRINT(("Version_Number: \t\t%1$#x (%1$d)\n", m_Version_Number))
	PARSER_PRINT(("Current_Next_Indicator: \t%1$#x (%1$d)\n", m_Current_Next_Indicator))
	PARSER_PRINT(("Section_Number: \t\t%1$#x (%1$d)\n", m_Section_Number))
	PARSER_PRINT(("Last_Section_Number: \t\t%1$#x (%1$d)\n", m_Last_Section_Number))
	PARSER_PRINT(("Network_Descriptor_Length: \t%1$#x (%1$d)\n\n", m_Network_Descriptor_Length))
	PARSER_PRINT(("Transport_Stream_Loop_Length: \t%1$#x (%1$d)\n", m_Transport_Stream_Loop_Length))
	//Descriptor

	PARSER_PRINT(("\n\tComponents: %zd\n\n", m_TS_List.size()))
	for (iter_TS_List = m_TS_List.begin(); iter_TS_List != m_TS_List.end(); iter_TS_List++) {
			iter_TS_List->Print();
	}
	PARSER_PRINT(("-----------------------------------------------\n\n"))	
}

uchar CNetwork_Infomation_Section::get_Table_ID() {
	return m_Table_ID;
}

bool CNetwork_Infomation_Section::get_Section_Syntax_Indicator() {
	return m_Section_Syntax_Indicator;
}

/**
	@brief	섹션의 길이를 구하는 함수
	@return	정상: 섹션의 길이 / 에러: -1 리턴
	@remark	섹션의 길이는 1021(0x3FD)를 초과할 수 없음 (from, ISO 13818-1)
*/
ushort CNetwork_Infomation_Section::get_Section_Length() {
	if(m_Section_Length > 1021) {
		printf("!!ERROR: Section_Length is wrong!!\n");
		return -1;
	}
	else {
		return m_Section_Length;
	}
}

ushort CNetwork_Infomation_Section::get_Network_ID() {
	return m_Network_ID;
}

uchar CNetwork_Infomation_Section::get_Version_Number() {
	return m_Version_Number;
}

bool CNetwork_Infomation_Section::get_Current_Next_Indicator() {
	return m_Current_Next_Indicator;
}

uchar CNetwork_Infomation_Section::get_Section_Number() {
	return m_Section_Number;
}

uchar CNetwork_Infomation_Section::get_Last_Section_Number() {
	return m_Last_Section_Number;
}

ushort CNetwork_Infomation_Section::get_Network_Descriptor_Length() {
	return m_Network_Descriptor_Length;
}

bool CNetwork_Infomation_Section::checkParse() {
	return isParse;
}

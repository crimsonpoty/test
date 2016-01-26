#include "Program_Association_Section.h"


void CProgram_Association_Section::Parse(const char *buffer) {
	m_Table_ID = buffer[0];
	m_Section_Syntax_Indicator = (buffer[1] & 0x80) >> 7;
	m_Section_Length = bit_swap_16(&buffer[1]) & 0xFFF;
	m_Transport_Stream_ID = bit_swap_16(&buffer[3]);
	m_Version_Number = (buffer[5] & 0x3E) >> 1;
	m_Current_Next_Indicator = buffer[5] & 1;
	m_Section_Number = buffer[6];
	m_Last_Section_Number = buffer[7];
	Parse_Programs(&buffer[8]);
	isParse = true;
}

void CProgram_Association_Section::Parse_Programs(const char *buffer) {
	m_Number_of_Programs = (int)((m_Section_Length - 9) / 4);

	for(int i = 0; i < m_Number_of_Programs; i++) {
		m_Program_List.push_back(CProgram(&buffer[i*4]));
	}
}

void CProgram_Association_Section::Print() {
	PARSER_PRINT(("--------------------- PAT ---------------------\n\n"))
	PARSER_PRINT(("Table_ID: \t\t\t%1$#x (%1$d)\n", m_Table_ID))
	PARSER_PRINT(("Section_Syntax_Indicator: \t%1$#x (%1$d)\n", m_Section_Syntax_Indicator))
	PARSER_PRINT(("Section_Length: \t\t%1$#x (%1$d)\n", m_Section_Length))
	PARSER_PRINT(("Transport_Stream_ID: \t\t%1$#x (%1$d)\n", m_Transport_Stream_ID))
	PARSER_PRINT(("Version_Number: \t\t%1$#x (%1$d)\n", m_Version_Number))
	PARSER_PRINT(("Current_Next_Indicator: \t%1$#x (%1$d)\n", m_Current_Next_Indicator))
	PARSER_PRINT(("Section_Number: \t\t%1$#x (%1$d)\n", m_Section_Number))
	PARSER_PRINT(("Last_Section_Number: \t\t%1$#x (%1$d)\n", m_Last_Section_Number))
	PARSER_PRINT(("\nThe Number of Programs: \t%1$#x (%1$d)\n\n", m_Number_of_Programs))

	for (iter_Program_List = m_Program_List.begin(); iter_Program_List != m_Program_List.end();
		iter_Program_List++) {
		iter_Program_List->Print();
	}
	PARSER_PRINT(("-----------------------------------------------\n\n"))
}

uchar CProgram_Association_Section::get_Table_ID() {
	return m_Table_ID;
}

bool CProgram_Association_Section::get_Section_Syntax_Indicator() {
	return m_Section_Syntax_Indicator;
}

/**
	@brief	섹션의 길이를 구하는 함수
	@return	정상: 섹션의 길이 / 에러: -1 리턴
	@remark	섹션의 길이는 1021(0x3FD)를 초과할 수 없음 (from, ISO 13818-1)
*/
ushort CProgram_Association_Section::get_Section_Length() {
	if(m_Section_Length > 1021) {
		printf("!!ERROR: Section_Length is wrong!!\n");
		return -1;
	}
	else {
		return m_Section_Length;
	}
}

ushort CProgram_Association_Section::get_Transport_Stream_ID() {
	return m_Transport_Stream_ID;
}

uchar CProgram_Association_Section::get_Version_Number() {
	return m_Version_Number;
}

bool CProgram_Association_Section::get_Current_Next_Indicator() {
	return m_Current_Next_Indicator;
}

uchar CProgram_Association_Section::get_Section_Number() {
	return m_Section_Number;
}

uchar CProgram_Association_Section::get_Last_Section_Number() {
	return m_Last_Section_Number;
}

uint32 CProgram_Association_Section::get_CRC_32() {
	return m_CRC_32;
}

int CProgram_Association_Section::get_Number_of_Programs() {
	return m_Number_of_Programs;
}

bool CProgram_Association_Section::checkParse() {
	return isParse;
}
#include "Program_Map_Section.h"


CProgram_Map_Section::CProgram_Map_Section(const char *buffer) {
	Parse(buffer);
}

void CProgram_Map_Section::Parse(const char *buffer) {
	m_Table_ID = buffer[0];
	m_Section_Syntax_Indicator = ((buffer[1] & 0x80) >> 7);
	m_Section_Length = bit_swap_16(&buffer[1]) & 0xFFF;
	m_Program_Number = bit_swap_16(&buffer[3]);
	m_Version_Number = (buffer[5] & 0x3E) >> 1;
	m_Current_Next_Indicator = buffer[5] & 1;
	m_Section_Number = buffer[6];
	m_Last_Section_Number = buffer[7];
	m_PCR_PID = DVB_PID(&buffer[8]);
	m_Program_Info_Length = bit_swap_16(&buffer[10]) & 0xFFF;

	ptr_Parse = (char *)&buffer[12];
	//CDescriptor();
	ptr_Parse += m_Program_Info_Length;

	//ES Info 사이즈 : m_Section_Length 이후부터 고정부분, Program_Info_Length, CRC를 제외한 나머지 길이
	int ES_Info_Size = m_Section_Length - 9 - m_Program_Info_Length - 4;
	iter_ES_Info_List = m_ES_Info_List.begin();
	do {
		m_ES_Info_List.push_back(CES_Info(ptr_Parse));
		iter_ES_Info_List++;
		ES_Info_Size = ES_Info_Size - 5 - iter_ES_Info_List->get_ES_Info_Length();
		ptr_Parse += 5 + iter_ES_Info_List->get_ES_Info_Length();

	} while(ES_Info_Size > 0);

	isParse = true;
}

void CProgram_Map_Section::Print() {
	PARSER_PRINT(("--------------------- PMT ---------------------\n\n"))
	PARSER_PRINT(("Table_ID: \t\t\t%1$#x (%1$d)\n", m_Table_ID))
	PARSER_PRINT(("Section_Syntax_Indicator: \t%d\n", m_Section_Syntax_Indicator))
	PARSER_PRINT(("Section_Length: \t\t%d\n", m_Section_Length))
	PARSER_PRINT(("Program_Number: \t\t%1$#x (%1$d)\n", m_Program_Number))
	PARSER_PRINT(("Version_Number: \t\t%1$#x (%1$d)\n", m_Version_Number))
	PARSER_PRINT(("Current_Next_Indicator: \t%1$#x (%1$d)\n", m_Current_Next_Indicator))
	PARSER_PRINT(("Section_Number: \t\t%1$#x (%1$d)\n", m_Section_Number))
	PARSER_PRINT(("Last_Section_Number: \t\t%1$#x (%1$d)\n", m_Last_Section_Number))
	PARSER_PRINT(("PCR_PID: \t\t\t%1$#x (%1$d)\n", m_PCR_PID))
	PARSER_PRINT(("Program_Info_Length: \t\t%1$#x (%1$d)\n", m_Program_Info_Length))
	//CDescriptor.CPrint_Descriptor();

	PARSER_PRINT(("\n\tComponents: %ld\n\n", m_ES_Info_List.size()))
	for (iter_ES_Info_List = m_ES_Info_List.begin(); iter_ES_Info_List != m_ES_Info_List.end();
			iter_ES_Info_List++) {
			iter_ES_Info_List->Print();
	}
	PARSER_PRINT(("-----------------------------------------------\n\n"))
}

uchar CProgram_Map_Section::get_Table_ID() {
	return m_Table_ID;
}

bool CProgram_Map_Section::get_Section_Syntax_Indicator() {
	return m_Section_Syntax_Indicator;
}

/**
	@brief	섹션의 길이를 구하는 함수
	@return	정상: 섹션의 길이 / 에러: -1 리턴
	@remark	섹션의 길이는 1021(0x3FD)를 초과할 수 없음 (from, ISO 13818-1)
*/
ushort CProgram_Map_Section::get_Section_Length() {
	if(m_Section_Length > 1021) {
		printf("!!ERROR: Section_Length is wrong!!\n");
		return -1;
	}
	else {
		return m_Section_Length;
	}
}

ushort CProgram_Map_Section::get_Program_Number() {
	return m_Program_Number;
}

uchar CProgram_Map_Section::get_Version_Number() {
	return m_Version_Number;
}

bool CProgram_Map_Section::get_Current_Next_Indicator() {
	return m_Current_Next_Indicator;
}

uchar CProgram_Map_Section::get_Section_Number() {
	return m_Section_Number;
}

uchar CProgram_Map_Section::get_Last_Section_Number() {
	return m_Last_Section_Number;
}

ushort CProgram_Map_Section::get_PCR_PID() {
	return m_PCR_PID;
}

ushort CProgram_Map_Section::get_Program_Info_Length() {
	return m_Program_Info_Length;
}

bool CProgram_Map_Section::checkParse() {
	return isParse;
}

#include "Program.h"


CProgram::CProgram(const char *buffer) {
	Parse(buffer);
}

void CProgram::Parse(const char *buffer) {
	m_Program_Number = bit_swap_16(&buffer[0]);
	if (m_Program_Number) {
		m_Program_Map_PID = DVB_PID(&buffer[2]);
	}
	else {
		m_Network_PID = DVB_PID(&buffer[2]);
	}
}

void CProgram::set_Program(const ushort ProNum, const ushort PID) {
	m_Program_Number = ProNum;
	if (m_Program_Number) {
		m_Program_Map_PID = PID;
	}
	else {
		m_Network_PID = PID;
	}
}

void CProgram::Print() {
	PARSER_PRINT(("\tProgram_Number: \t%1$#x (%1$d)\n", m_Program_Number))
	if (m_Program_Number) {
		PARSER_PRINT(("\tProgram_Map_PID: \t%1$#x (%1$d)\n\n", m_Program_Map_PID))
	}
	else {
		PARSER_PRINT(("\tNetwork_PID: \t\t%1$#x (%1$d)\n\n", m_Network_PID))
	}
}

ushort CProgram::get_Program_Number() {
	return m_Program_Number;
}

ushort CProgram::get_Program_Map_PID() {
	if(m_Program_Number) {
		return m_Program_Map_PID;
	}
	else {
		return m_Network_PID;
	}
}

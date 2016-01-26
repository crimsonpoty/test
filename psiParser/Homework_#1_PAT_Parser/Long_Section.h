/**
	*	@file		Long_Section.h
	*	@date		2015. 03. 30
	*	@author		BenjaminTeam, k150030
	*	@brief		Long_Section 정보를 저장하는 클래스
	*/

#ifndef _LONG_SECTION_H_
#define _LONG_SECTION_H_

#include "Common.h"

/**
	@class		CLong_Section
	@brief		Long_Section 정보를 저장하는 클래스
*/
class CLong_Section
{
	public:
	CLong_Section() {
		m_Start_ptr = (char *)malloc(184 * sizeof(char));
	};
	~CLong_Section() {
		// free(m_Start_ptr);
	};

	public:
	void Assemble(const char *buffer, const int size) {
		memcpy(m_Move_ptr, buffer, size);
		m_Move_ptr += size;
	}
	// void Resize(const char *buffer, const int size) {
	void Resize(const int size) {
		m_Start_ptr = (char *)realloc(m_Start_ptr, size);
		if(NULL == m_Start_ptr) {
			printf("ERROR: Memory realloc in Long_Section\n");
			exit(0);
		}
		m_Move_ptr = m_Start_ptr;
	}
	char* get_Buffer() {
		return m_Start_ptr;
	}
	void set_Total_Section_Length(const int Length) {
		m_Total_Section_Length = Length;
		Resize(m_Total_Section_Length + 3);
	}
	int get_Total_Section_Length() {
		return m_Total_Section_Length;
	}
	// void set_Remained_Length(const int Length) {
		// m_Remained_Length = Length;
	// }
	// int get_Remained_Length() {
		// return m_Remained_Length;
	// }
	// bool check_Is_Long_Section() {
		// return is_Long_Section;
	// }

	protected:
	ushort m_PID;	//각 섹션을 구분하기 위한 Index로 사용한다.
	int m_Total_Section_Length;
	// int m_Remained_Length;
	char *m_Start_ptr;
	char *m_Move_ptr;
	// bool is_Long_Section;
	public:
	int m_Remained_Length;
};

#endif
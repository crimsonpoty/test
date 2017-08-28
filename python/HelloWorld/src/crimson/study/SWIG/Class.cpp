#include "Class.h"

CClass::CClass(string input)
{
	m_str = input;
}

bool CClass::Toggle(bool bBool)
{
	return !bBool;
}

string CClass::GetText()
{
	return m_str;
}

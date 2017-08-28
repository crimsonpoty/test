#include "Class.h"

CClass::CClass()
{
	m_str = "Hello World!";
}

bool CClass::Toggle(bool bBool)
{
	return !bBool;
}

string CClass::GetText()
{
	return m_str;
}

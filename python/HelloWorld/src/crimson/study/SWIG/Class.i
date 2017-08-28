/* Class.i */
%module cppClass
%{
#include "Class.h"
%}

%include std_string.i

class CClass
{
public:
	CClass(std::string input);
	bool Toggle(bool bBool);
	std::string GetText();
};

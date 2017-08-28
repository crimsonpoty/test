// #ifndef _CLASS_H_
// #define _CLASS_H_

#include <string>

using std::string;

class CClass
{
public:
	CClass();
	bool Toggle(bool bBool);
	string GetText();

private:
	string m_str;
};

// #endif // _CLASS_H_
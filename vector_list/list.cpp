#include <cstdio>
#include <iostream>
#include <list>
#include <iterator>
#include <cstring>

using namespace std;

template<typename C>
void dump(const char *desc, C c)
{
	cout.width(12);cout << left << desc << "==> ";
	copy(c.begin(), c.end(), ostream_iterator<typename C::value_type>(cout," "));
	cout << endl;
}

int main(int argc, char* argv[])
{
	const char *str1="abcdefghij";
	list<char> lc1(&str1[0],&str1[10]);
	list<char>::iterator it;

	dump("최초",lc1);
	it=lc1.begin();it++;it++;it++;it++;it++;
	lc1.insert(it,'Z');
	dump("Z 삽입",lc1);
	it=lc1.end();it--;it--;it--;
	lc1.erase(it);
	dump("h삭제",lc1);
	
	cout << endl;
	
	const char *str2="double linked list class";
    list<char> lc2(&str2[0],&str2[strlen(str2)]);
 
    dump("최초",lc2);
    lc2.remove('l');
    dump("l삭제",lc2);

	cout << endl;

	return 0;
}
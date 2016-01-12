#include <cstdio>
#include <iostream>
#include <list>
#include <iterator>

using namespace std;

int main(int argc, char* argv[])
{
	list<int> li;
	list<int>::iterator it;

	li.push_back(8);
	li.push_back(9);
	li.push_front(2);
	li.push_front(1);

	for (it=li.begin();it!=li.end();it++) {
		printf("%d\n",*it);
	}

	return 0;
}
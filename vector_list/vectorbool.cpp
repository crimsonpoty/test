// #include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	vector<bool> vb(32);

	cout << vb[10] << endl;
	vb[10]=true;
	cout << vb[10] << endl;

	vector<bool>::reference r=vb[10];
	cout << r << endl;
	r.flip();
	cout << r << endl;
	vector<bool>::iterator it;
	for (it=vb.begin();it!=vb.end();it++) {
		cout << *it;
	}
	cout << endl;

	return 0;
}

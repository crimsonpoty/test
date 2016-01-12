#include <iostream>
#include <string>
#include <vector>
#include <iterator>

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
	vector<string> v1;
	dump("v1",v1);
	
	vector<double> v2(10);
	dump("v2",v2);
	
	vector<int> v3(10,7);
	dump("v3",v3);
	
	vector<int> v4(v3);
	dump("v4",v4);
	
	int ar[]={1,2,3,4,5,6,7,8,9};
	
	vector<int> v5(&ar[2],&ar[5]);
	dump("v5",v5);

	return 0;
}

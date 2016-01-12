#include <iostream>
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
	vector<int> vi;
	dump("최초",vi);
	
	for (int i=1;i<=8;i++)
		vi.push_back(i);
	dump("8개 추가",vi);
	
	vi.insert(vi.begin()+3,10);
	dump("10 삽입",vi);
	
	vi.insert(vi.begin()+3,11);
	dump("11 삽입",vi);
	
	vi.insert(vi.begin()+3,12);
	dump("12 삽입",vi);
	
	vi.erase(vi.begin()+7);
	dump("요소 7 삭제",vi);

	return 0;
}

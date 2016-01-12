#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <list>
#include <algorithm>
#include <exception>

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
	const char *str="0123456789";

	vector<char> vc(&str[0],&str[10]);
	dump ("생성 직후 ", vc);

	vc.push_back('A');
	dump ("A 추가", vc);

	vc.insert(vc.begin()+3,'B');
	dump ("B 삽입", vc);

	vc.pop_back();
	dump ("끝 요소 삭제", vc);

	vc.erase(vc.begin()+5,vc.begin()+8);
	dump ("5~8 삭제", vc);

	cout << endl;
	cout << endl;


	vector<char> vc1;
	for (int i=0;i<10;i++) {
		vc1.insert(vc1.begin(),'Z');
	}
	dump("개별 추가", vc1);

	vector<char>vc2;
	vc2.insert(vc2.begin(),10,'Z');
	dump("한꺼번에 추가", vc2);

	cout << endl;
	cout << endl;

	list<int> li;
	for (int i=0;i<100;i++) {
		li.push_back(i);
	}
	vector<int> vi;

	vi.insert(vi.begin(), find(li.begin(),li.end(),8), find(li.begin(),li.end(),25));
	dump("추가 후", vi);

	cout << endl;
	cout << endl;

	vector<int> vi1;
	for (int i=0;i<80;i++) {
		vi1.push_back(i);
	}
	vector<int>::iterator it;
	it=find(vi1.begin(),vi1.end(),55);
	cout << *it << endl;
	// vi1.erase(it+1);
	vi1.insert(vi1.begin(),1234);
	cout << *it << endl;

	cout << endl;
	cout << endl;

	int ari[]={1,2,3,4,5};
    vector<int> vi4(&ari[0],&ari[5]);

    vector<int>vi2;
    vi2=vi4;
    dump("vi2",vi2);

    vector<int>vi3;
    vi3.assign(vi4.begin()+1,vi4.end()-1);
    dump("vi3",vi3);

	cout << endl;
	cout << endl;

	vector<char> vc3;
	vector<char> vc4;

	vc3 = vc;
	puts(vc == vc3 ? "같다" : "다르다");
	puts(vc == vc4 ? "같다" : "다르다");
	vc3.pop_back();
	puts(vc > vc3 ? "크다" : "안크다");

	cout << endl;
	cout << endl;

	try {
		// cout << vi4[10] << endl;
		cout << vi4.at(10) << endl;
	}
	catch (out_of_range e) {
		cout << "벡터의 범위를 벗어났습니다." << endl;
	}

	return 0;
}

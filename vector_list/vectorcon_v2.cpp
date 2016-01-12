#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;
 
int main(int argc, char* argv[])
{
	vector<int> vi;
	
	printf("max_size =  %d\n", vi.max_size());
	printf("size =  %d, capacity = %d\n", vi.size(), vi.capacity());
	vi.push_back(123);
	vi.push_back(456);
	printf("size =  %d, capacity = %d\n", vi.size(), vi.capacity());
	vi.resize(10);
	printf("size =  %d, capacity = %d\n", vi.size(), vi.capacity());
	vi.reserve(20);
	printf("size =  %d, capacity = %d\n", vi.size(), vi.capacity());
	
	return 0;
}

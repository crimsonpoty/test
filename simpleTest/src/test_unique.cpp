#include <vector>
#include <iostream>
#include <algorithm>

int main(/*int argc, char** argv*/) {

	std::vector<int> v = {0, 3, 2, 2, 3, 5, 0};
	for(auto& a: v) { std::cout << a << " "; } std::cout <<std::endl;

	std::sort(v.begin(), v.end(), std::less<int>());
	for(auto& a: v) { std::cout << a << " "; } std::cout <<std::endl;

	auto pos = std::unique(v.begin(), v.end());
	for(auto& a: v) { std::cout << a << " "; } std::cout <<std::endl;
	
	v.erase(pos, v.end());
	for(auto& a: v) { std::cout << a << " "; } std::cout <<std::endl;
	
	return 0;
}

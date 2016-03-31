#include <stdio.h>
#include <vector>
#include <string>

int main() {
    
	// std::vector<std::string> Text("A", "B", "C", "D", "E");

	// for(int i = 0; i < Text.size(); i++) {
		// printf("%d: %s\n", i, Text[i].c_str());
	// }

	std::string Text[5] = {"A", "B", "C", "D", "E"};

	for(int i = 0; i < 5; i++) {
		printf("%d: %s\n", i, Text[i].c_str());
	}

	return 0;
}

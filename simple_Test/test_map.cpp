#include <stdio.h>
#include <map>
#include <string>

int main() {
    
	std::map<std::string, std::string> TelefonicaModelMap;
	std::map<std::string, std::string>::iterator it;
	TelefonicaModelMap["NA1160"] 		= "bca_NA1160.bin";
	TelefonicaModelMap["NA1160-A1"] 	= "bca_NA1160_A1.bin";
	TelefonicaModelMap["NA1160-A2"] 	= "bca_NA1160_A2.bin";
	TelefonicaModelMap["NA1160-CNT"] 	= "bca_NA1160_CNT.bin";
	
	it = TelefonicaModelMap.find("NA1160-A2");
	
	printf("%s\n", (it->second).c_str());

	return 0;
}

#include <stdio.h>
#include <map>
#include <string>

enum ETELEFONICA_MODEL
{
	eTELEFONICA_MODEL_NA1160,
	eTELEFONICA_MODEL_NA1160_A1,
	eTELEFONICA_MODEL_NA1160_A2,
	eTELEFONICA_MODEL_NA1160_CNT,
	eTELEFONICA_MODEL_MAX
};

int main() {
    
	std::map<ETELEFONICA_MODEL, std::string> TelefonicaModelMap;
	std::map<ETELEFONICA_MODEL, std::string>::iterator it;
	TelefonicaModelMap[eTELEFONICA_MODEL_NA1160] 		= "bca_NA1160.bin";
	TelefonicaModelMap[eTELEFONICA_MODEL_NA1160_A1] 	= "bca_NA1160_A1.bin";
	TelefonicaModelMap[eTELEFONICA_MODEL_NA1160_A2] 	= "bca_NA1160_A2.bin";
	TelefonicaModelMap[eTELEFONICA_MODEL_NA1160_CNT] 	= "bca_NA1160_CNT.bin";
	
	it = TelefonicaModelMap.find(eTELEFONICA_MODEL_NA1160_CNT);
	
	printf("%s\n", (it->second).c_str());

	return 0;
}

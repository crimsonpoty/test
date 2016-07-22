#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>

struct SDlnbUbList
{
	int UbNo;
	int TxFrequency;
	int ePolarization;
	int OutputLevel;
	int UbFrequency;
	int Bandwidth;
};

int main() {
	std::ofstream Out("DlnbUbList.csv");

	if(!Out.is_open()) {
		printf("ofstream | File open is Fail!\n");
		return 0;
	}

	SDlnbUbList DlnbUbList[] = {
		{1,		10728,	1,	25,	978,	36},
		{2,		10768,	1,	25,	1018,	36},
		{3,		10808,	1,	25,	1058,	36},
		{4,		10848,	1,	25,	1098,	36},
		{5,		10888,	1,	25,	1138,	36}
	};

	int DlnbUbListSize = sizeof(DlnbUbList)/sizeof(SDlnbUbList);

	for(int i = 0; i < DlnbUbListSize; i++) {
		Out << DlnbUbList[i].UbNo << ','
			<< DlnbUbList[i].TxFrequency << ','
			<< DlnbUbList[i].ePolarization << ','
			<< DlnbUbList[i].OutputLevel << ','
			<< DlnbUbList[i].UbFrequency << ','
			<< DlnbUbList[i].Bandwidth << ','
			<< std::endl;
	}
	const unsigned char CRC = 0xFF;
	const unsigned int ModFlag = 1;
	Out << static_cast<int>(CRC) << "," << ModFlag;

	Out.close();

	std::ifstream In("DlnbUbList.csv", std::ios::in);

	if(!In.is_open()) {
		printf("ifstream | File open is Fail!\n");
		return 0;
	}

	char tt[256] = "";
	std::vector<int> v;

	while(!In.eof()) {
		In.getline(tt, 256, ',');
		v.push_back(std::atoi(tt));
	}

	// v.pop_back();

	int i = 0;
	for(auto it = v.begin(); it != v.end(); it++) {
		printf("%d ", v[i++]);
		if(i && i % 6 == 0) printf("\n");
	}
	printf("vector size(%d)\n", (int)v.size());

	In.close();

	SDlnbUbList temp[v.size() / 6];

	for(int i = 0, j = 0; i < (int)v.size() / 6; i++, j+=6) {
		temp[i].UbNo = v[j];
		temp[i].TxFrequency = v[j+1];
		temp[i].ePolarization = v[j+2];
		temp[i].OutputLevel = v[j+3];
		temp[i].UbFrequency = v[j+4];
		temp[i].Bandwidth = v[j+5];
	}

	// for(int i = 0; i < (int)(sizeof(temp)/sizeof(SDlnbUbList)); i++) {
		// printf("%d: %d %d %d %d %d|\n", temp[i].UbNo, temp[i].TxFrequency, temp[i].ePolarization, temp[i].OutputLevel, temp[i].UbFrequency, temp[i].Bandwidth);
	// }

	return 0;
}

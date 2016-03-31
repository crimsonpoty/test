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
		{0,		10728,	1,	25,	978,	36},
		{1,		10768,	1,	25,	1018,	36},
		{2,		10808,	1,	25,	1058,	36},
		{3,		10848,	1,	25,	1098,	36},
		{4,		10888,	1,	25,	1138,	36},
		{5,		10928,	1,	25,	1178,	36},
		{6,		11222,	1,	25,	1472,	36},
		{7,		11262,	1,	25,	1512,	36},
		{8,		11302,	1,	25,	1552,	36},
		{9,		11342,	1,	25,	1592,	36},
		{10,	11382,	1,	25,	1632,	36},
		{11,	11422,	1,	25,	1672,	36}
	};
	
	int DlnbUbListSize = sizeof(DlnbUbList)/sizeof(SDlnbUbList);
	
	for(int i = 0; i < DlnbUbListSize; i++) {
		Out << DlnbUbList[i].UbNo << ',' << DlnbUbList[i].TxFrequency << ',' << DlnbUbList[i].ePolarization << ',' << DlnbUbList[i].OutputLevel << ',' << DlnbUbList[i].UbFrequency << ',' << DlnbUbList[i].Bandwidth << ',' << std::endl;
	}
	
	Out.close();
	
	std::ifstream In("DlnbUbList.csv", std::ios::in);
	
	if(!In.is_open()) {
		printf("ifstream | File open is Fail!\n");
		return 0;
	}	
	
	char tt[10] = "";
	std::vector<int> v;
	
	while(!In.eof()) {
		In.getline(tt, 100, ',');
		v.push_back(std::atoi(tt));
	}
	
	v.pop_back();
	
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
	
	for(int i = 0; i < (int)(sizeof(temp)/sizeof(SDlnbUbList)); i++) {
		printf("%d: %d %d %d %d %d|\n", temp[i].UbNo, temp[i].TxFrequency, temp[i].ePolarization, temp[i].OutputLevel, temp[i].UbFrequency, temp[i].Bandwidth);
	}
		
	return 0;
}

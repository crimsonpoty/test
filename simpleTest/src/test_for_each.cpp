#include <stdio.h>
#include <string>

#include <iostream>
#include <algorithm>
#include <vector>

typedef unsigned int uint;

enum EPOLARIZATION
{
	ePOLARIZATION_V,
	ePOLARIZATION_H,
	ePOLARIZATION_AUTO
};

struct SDlnbUbList
{
	int UbNo;
	uint TxFrequency;
	EPOLARIZATION ePolarization;
	int OutputLevel;
	uint UbFrequency;
	uint Bandwidth;
};

typedef std::vector<SDlnbUbList> DlnbUbList;


int main()
{
	SDlnbUbList sDlnbUbList[] = {
		{1,		10975,	ePOLARIZATION_H,	25,	975,	36},
		{2,		11015,	ePOLARIZATION_H,	25,	1015,	36},
		{3,		11055,	ePOLARIZATION_H,	25,	1055,	36},
		{4,		11095,	ePOLARIZATION_H,	25,	1095,	36},
		{5,		11135,	ePOLARIZATION_H,	25,	1135,	36},
		{6,		11175,	ePOLARIZATION_H,	25,	1175,	36},
		{7,		12470,	ePOLARIZATION_H,	25,	1215,	36},
		{8,		12510,	ePOLARIZATION_H,	25,	1255,	36},
		{9,		12550,	ePOLARIZATION_H,	25,	1305,	36},
		{10,	12590,	ePOLARIZATION_H,	25,	1345,	36},
		{11,	11055,	ePOLARIZATION_V,	25,	1385,	36},
		{12,	11095,	ePOLARIZATION_V,	25,	1425,	36},
		{13,	11135,	ePOLARIZATION_V,	25,	1465,	36},
		{14,	11185,	ePOLARIZATION_V,	25,	1515,	36},
		{15,	11222,	ePOLARIZATION_V,	25,	1552,	36},
		{16,	11262,	ePOLARIZATION_V,	25,	1592,	36},
		{17,	12630,	ePOLARIZATION_H,	25,	1632,	36},
		{18,	11342,	ePOLARIZATION_V,	25,	1672,	36},
		{19,	12670,	ePOLARIZATION_H,	25,	1712,	36},
		{20,	11422,	ePOLARIZATION_V,	25,	1752,	36},
		{21,	11808,	ePOLARIZATION_H,	25,	1886,	36},
		{22,	11851,	ePOLARIZATION_H,	25,	1929,	36},
		{23,	11891,	ePOLARIZATION_H,	25,	1969,	36},
		{24,	11931,	ePOLARIZATION_H,	25,	2009,	36},
		{25,	11972,	ePOLARIZATION_H,	25,	2050,	36},
		{26,	12012,	ePOLARIZATION_H,	25,	2090,	36},
		{27,	12052,	ePOLARIZATION_H,	25,	2130,	36},
		{28,	12092,	ePOLARIZATION_H,	25,	2170,	36}
	};

	DlnbUbList vDlnbUbList;
	size_t sDlnbUbListSize = sizeof(sDlnbUbList) / sizeof(sDlnbUbList[0]);
	std::copy(sDlnbUbList, sDlnbUbList + sDlnbUbListSize, back_inserter(vDlnbUbList));

	DlnbUbList compareDlnbUbList;
	for(int i = 0; i < (int)(sizeof(sDlnbUbList)/sizeof(sDlnbUbList[0])); i++) {
		compareDlnbUbList.push_back(sDlnbUbList[i]);
	}
	
	bool vCompare = false;
	if(vDlnbUbList.size() == compareDlnbUbList.size()) {
		for(int i = 0; i < (int)compareDlnbUbList.size(); i++) {
			if(vDlnbUbList[i].UbNo == compareDlnbUbList[i].UbNo
				&& vDlnbUbList[i].UbNo == compareDlnbUbList[i].UbNo
				&& vDlnbUbList[i].TxFrequency == compareDlnbUbList[i].TxFrequency
				&& vDlnbUbList[i].ePolarization == compareDlnbUbList[i].ePolarization
				&& vDlnbUbList[i].OutputLevel == compareDlnbUbList[i].OutputLevel
				&& vDlnbUbList[i].UbFrequency == compareDlnbUbList[i].UbFrequency
				&& vDlnbUbList[i].Bandwidth == compareDlnbUbList[i].Bandwidth
				) {
				vCompare = true;
			}
		}
	}
	
	if(vCompare) {
		printf("Same Vector!!\n");
	}
	else {
		printf("Diff Vector T.T\n");
	}

	
	printf("s-Size: %lu | v-Size: %lu\n", sDlnbUbListSize, vDlnbUbList.size());
	
	std::for_each(vDlnbUbList.begin(), vDlnbUbList.end(), [] (const SDlnbUbList vItem) {
		printf("UbNo : %d\t", vItem.UbNo);
		printf("TxFrequency : %u\t", vItem.TxFrequency);
		printf("ePolarization : %s\t", vItem.ePolarization == 0 ? "ePOLARIZATION_V" : "ePOLARIZATION_H");
		printf("OutputLevel : %d\t", vItem.OutputLevel);
		printf("UbFrequency : %u\t", vItem.UbFrequency);
		printf("Bandwidth : %u\n", vItem.Bandwidth);
	});

	return 0;
}

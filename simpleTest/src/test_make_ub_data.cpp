#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <algorithm>
// #include "DlnbUbList.h"

typedef std::vector<uint8_t> PayloadData;

struct SUbList
{
	uint8_t UbNo;
	uint16_t InputFrequency;
	uint8_t Polarization;
	uint8_t OutputLevel;
	uint16_t UbFrequency;
	uint8_t Bandwidth;
};

SUbList UbList[] =
{
#if 0	// TelefonicaIsdbT
		{0,		10975,	1,	25,	975,	36},
		{1,		11015,	1,	25,	1015,	36},
		{2,		11055,	1,	25,	1055,	36},
		{3,		11095,	1,	25,	1095,	36},
		{4,		11135,	1,	25,	1135,	36},
		{5,		11175,	1,	25,	1175,	36},
		{6,		12470,	1,	25,	1215,	36},
		{7,		12510,	1,	25,	1255,	36},
		{8,		12550,	1,	25,	1305,	36},
		{9,		12590,	1,	25,	1345,	36},
		{10,	11055,	0,	25,	1385,	36},
		{11,	11095,	0,	25,	1425,	36},
		{12,	11135,	0,	25,	1465,	36},
		{13,	11185,	0,	25,	1515,	36},
		{14,	11222,	0,	25,	1552,	36},
		{15,	11262,	0,	25,	1592,	36},
		{16,	12630,	1,	25,	1632,	36},
		{17,	11342,	0,	25,	1672,	36},
		{18,	12670,	1,	25,	1712,	36},
		{19,	11422,	0,	25,	1752,	36},
		{20,	11808,	1,	25,	1886,	36},
		{21,	11851,	1,	25,	1929,	36},
		{22,	11891,	1,	25,	1969,	36},
		{23,	11931,	1,	25,	2009,	36},
		{24,	11972,	1,	25,	2050,	36},
		{25,	12012,	1,	25,	2090,	36},
		{26,	12052,	1,	25,	2130,	36},
		{27,	12092,	1,	25,	2170,	36}
#else
		{0,		10728,	1,	25,	978,	36},
		{1,		10768,	1,	25,	1018,	36},
		{2,		10808,	1,	25,	1058,	36},
		{3,		10848,	1,	25,	1098,	36},
		{4,		10888,	1,	25,	1138,	36},
		{5,		10928,	1,	25,	1178,	36},
		{6,		10808,	0,	25,	1218,	36},
		{7,		10848,	0,	25,	1258,	36},
		{8,		10928,	0,	25,	1298,	36},
		{9,		12092,	1,	25,	1338,	36},
		{10,	12390,	1,	25,	1378,	36},
		{11,	11175,	1,	25,	1425,	36},
		{12,	11222,	1,	25,	1472,	36},
		{13,	11262,	1,	25,	1512,	36},
		{14,	11302,	1,	25,	1552,	36},
		{15,	11342,	1,	25,	1592,	36},
		{16,	11382,	1,	25,	1632,	36},
		{17,	11422,	1,	25,	1672,	36},
		{18,	10728,	0,	25,	1768,	36},
		{19,	10768,	0,	25,	1808,	36},
		{20,	11738,	0,	25,	1866,	56},
		{21,	11799,	0,	25,	1930,	56},
		{22,	11972,	0,	25,	1984,	36},
		{23,	12092,	0,	25,	2024,	36},
		{24,	12172,	0,	25,	2064,	36}
#endif
};

/** Return CRC-8 of the data, using x^8 + x^2 + x + 1 polynomial. */
uint8_t Crc8(const PayloadData &data, int Len)
{
	auto it = data.begin(); it++;
	uint16_t crc = 0;

	for (int j = Len - 1; j; j--, it++) {
		crc ^= (*it << 8);
		for(int i = 8; i; i--) {
			if (crc & 0x8000)
				crc ^= (0x1070 << 3);
			crc <<= 1;
		}
	}
	return (uint8_t)(crc >> 8);
}

int main(int argc, char *argv[])
{
	if(argc > 3) {
		printf("Too many argument\n");
		printf("[Usage] %s 1  # print Ub List\n", argv[0]);
		return -1;
	}

#if 1
	const int UbListSize = sizeof(UbList)/sizeof(SUbList);

	if(argc == 2 && !strcmp(argv[1], "1")) {
		for(int i = 0; i < UbListSize; i++)	{
			printf("%2d: UbNo(%2d), InputFrequency(%4d), Polarization(%d), OutputLevel(%d), UbFrequency(%4d), Bandwidth(%d)\n",
				i, UbList[i].UbNo, UbList[i].InputFrequency, UbList[i].Polarization, UbList[i].OutputLevel, UbList[i].UbFrequency, UbList[i].Bandwidth);
		}
	}
	printf("\n");

	PayloadData UbData;
	UbData.push_back(3);	// Version

	for(int i = 0; i < UbListSize; i++)	{
		const uint16_t tempInputFrequency = UbList[i].InputFrequency > 11700 ? UbList[i].InputFrequency - 10600 : UbList[i].InputFrequency - 9750;
		UbData.push_back((UbList[i].UbNo << 3) + (((tempInputFrequency - 300) >> 8) & 0x7));
		UbData.push_back((tempInputFrequency - 300) & 0xFF);
		uint8_t DlnbPolarization = 0;
		if(UbList[i].InputFrequency > 11700) {
			if(UbList[i].Polarization == 0)	DlnbPolarization = 0;
			else							DlnbPolarization = 1;
		}
		else {
			if(UbList[i].Polarization == 0)	DlnbPolarization = 3;
			else							DlnbPolarization = 2;
		}
		UbData.push_back(DlnbPolarization);
		UbData.push_back((UbList[i].OutputLevel << 3) + (((UbList[i].UbFrequency - 300) >> 8) & 0x7));
		UbData.push_back((UbList[i].UbFrequency - 300) & 0xFF);
		UbData.push_back(UbList[i].Bandwidth & 0x7F);
	}

	uint8_t crc = Crc8(UbData, UbData.size());
	UbData.push_back(crc);	// CRC-8

	int i = 0;
	for(auto it = UbData.begin(); it != UbData.end(); it++, i++) {
		// printf("%02X", (*it));
		printf("0x%02X, ", (*it));
		if(i && i % 10 == 0) puts("");
	};

	printf("\n");
	printf("crc: %02X", crc);
	printf("\n");
	printf("===================================\n");
	for(auto it = ++UbData.begin(); it != --UbData.end(); it+=6) {
	// for(auto it = UbData.begin(); it != --UbData.end(); it+=6) {
		printf("UbNo(%2d), ", (*it) >> 3);
		printf("InputFrequency(%4d), ", (((*(it) & 0x7) << 8) | *(it+1)) + 300);
		printf("Polarization(%d), ", *(it+2));
		printf("OutputLevel(%2d), ", *(it+3) >> 3);
		printf("UbFrequency(%4d), ", (((*(it+3) & 0x7) << 8) | *(it+4)) + 300);
		printf("Bandwidth(%2d) ", *(it+5));
		printf("\n");
	}
#else
	CDlnbUbList DlnbUbList;
	DlnbUbList.SetVersion(3);

	if(argc == 2 && !strcmp(argv[1], "1")) {
		for(int i = 0; i < DlnbUbList.GetUbListSize(); i++)	{
			printf("%2d: UbNo(%2d), TxFrequency(%4d), Polarization(%d), UbFrequency(%4d), Bandwidth(%d), OutputLevel(%d)\n",
				i, DlnbUbList.GetUbNo(i), DlnbUbList.GetTxFrequency(i), DlnbUbList.GetPolarization(i), DlnbUbList.GetUbFrequency(i), DlnbUbList.GetBandwidth(i), DlnbUbList.GetOutputLevel(i));
		}
	}
	printf("\n");

	PayloadData oPayloadData = DlnbUbList.GetPayloadData();

	int i = 0;
	for(auto it = oPayloadData.begin(); it != oPayloadData.end(); it++, i++) {
		printf("%02X", *it);
	};
	printf("\n");
#endif

	return 0;
}

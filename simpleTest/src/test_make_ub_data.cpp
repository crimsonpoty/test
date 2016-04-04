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
	{0,		978,	2,	25,	978,	36},
	{1,		1018,	2,	25,	1018,	36},
	{2,		1058,	2,	25,	1058,	36},
	{3,		1098,	2,	25,	1098,	36},
	{4,		1138,	2,	25,	1138,	36},
	{5,		1178,	2,	25,	1178,	36},
	{6,		1472,	2,	25,	1472,	36},
	{7,		1512,	2,	25,	1512,	36},
	{8,		1552,	2,	25,	1552,	36},
	{9,		1592,	2,	25,	1592,	36},
	{10,	1632,	2,	25,	1632,	36},
	{11,	1672,	2,	25,	1672,	36}
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
		printf("e.g.,) %s 1  # print Ub List\n", argv[0]);
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
		UbData.push_back((UbList[i].UbNo << 3) + (((UbList[i].InputFrequency - 300) >> 8) & 0x7));
		UbData.push_back((UbList[i].InputFrequency - 300) & 0xFF);
		UbData.push_back(UbList[i].Polarization);
		UbData.push_back((UbList[i].OutputLevel << 3) + (((UbList[i].UbFrequency - 300) >> 8) & 0x7));
		UbData.push_back((UbList[i].UbFrequency - 300) & 0xFF);
		UbData.push_back(UbList[i].Bandwidth & 0x7F);
	}

	uint8_t crc = Crc8(UbData, UbData.size());
	UbData.push_back(crc);	// CRC-8

	int i = 0;
	for(auto it = UbData.begin(); it != UbData.end(); it++, i++) {
		printf("%02X", *it);
	};
	printf("\n");
	printf("===================================\n");
	for(auto it = ++UbData.begin(); it != --UbData.end(); it+=6) {
		printf("UbNo(%2d), ", *it >> 3);
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

#include <stdio.h>

struct SManualUbList
{
	int UbNo;
	int TxFrequency;
	int ePolarization;
	int UbFrequency;
	int BandWidth;
	int PowerLevel;
};

int main() {
    
	SManualUbList ManualUbList[5] = {
		{1, 2, 3, 4, 5, 6},
		{7, 8, 9, 10, 11, 12},
	};
	
	printf("size array: %lu %lu\n", sizeof(ManualUbList), sizeof(SManualUbList));
	
	for(int i = 0; i < static_cast<int>(sizeof(ManualUbList) / sizeof(SManualUbList)); i++) {
		printf("%d: %d %d %d %d %d %d\n", i, ManualUbList[i].UbNo, ManualUbList[i].TxFrequency, ManualUbList[i].ePolarization, ManualUbList[i].UbFrequency, ManualUbList[i].BandWidth, ManualUbList[i].PowerLevel);
	}

	return 0;
}

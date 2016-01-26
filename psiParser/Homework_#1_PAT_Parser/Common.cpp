#include "Common.h"

//� ��� ��� ��??

void char_buffer_dump(const char *buffer, const int size, const int mode) {
	printf("\n=================================== char buffer dump ============================================\n");
	for(int i = 0; i < size; i++) {
		if(1 == mode) {	//index�� buffer�� ������ ���� ���
			printf("%3d: %02hhX   ", i, buffer[i]); if(0 != i && 0 == ((i + 1) % 10)) { printf("\n");}\
		}
		if(2 == mode) {	//���ٷ� ���� ���� ���
			printf("%02hhX", buffer[i]);
		}
	}
	printf("\n=================================================================================================\n\n");
}

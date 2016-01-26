#include "Common.h"

//¿ ¿¿¿ ¿¿¿ ¿¿??

void char_buffer_dump(const char *buffer, const int size, const int mode) {
	printf("\n=================================== char buffer dump ============================================\n");
	for(int i = 0; i < size; i++) {
		if(1 == mode) {	//index¿Í bufferÀÇ ³»¿ëÀ» °°ÀÌ Ãâ·Â
			printf("%3d: %02hhX   ", i, buffer[i]); if(0 != i && 0 == ((i + 1) % 10)) { printf("\n");}\
		}
		if(2 == mode) {	//ÇÑÁÙ·Î ¹öÆÛ ³»¿ë Ãâ·Â
			printf("%02hhX", buffer[i]);
		}
	}
	printf("\n=================================================================================================\n\n");
}

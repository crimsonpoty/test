#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(2 != argc) {
		puts("input scale size");
	}
	
	
	const int width = 2;
	const int rearQRCodeBufferLength = width * width;
	const int scale = atoi(argv[1]);
	const int scaledWidth = width * scale;
	for(int i = 0; i < rearQRCodeBufferLength; i++) {
		for(int col = 0; col < scale; col++) {
			for(int row = 0; row < scale; row++) {
				int offset = (i / width) * scale * width * scale + (i % width) * scale + row + col * scaledWidth;
				printf("%3d ", offset);				
			}
			puts("");
		}
		puts("");
	}

	return 0;
}

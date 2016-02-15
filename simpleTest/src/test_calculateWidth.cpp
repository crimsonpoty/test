#include <cstdio>
#include <iostream>
#include <string.h>
#include "ttxosd_font_gray_data.h"
// #include "ttxosd_font_mono_data.h"
// #include "ttxosd_font_mono_width.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

// k150030 추가 : 폰트 폭 구하는 함수(Little endian으로 인해 XY가 바뀌어 있으니, 세로폭을 구함)
int CalculateFontWidth(const uchar *pSrc, const int pWidth, const int pHeight, const int Length)
{
	uchar buffer[pWidth][pHeight];
	memset(buffer, 0, Length);
	for(int Y = 0 ; Y < pHeight ; Y++) {
		for(int X = 0 ; X < pWidth ; X++) {
			buffer[X][Y] = pSrc[X + Y * pWidth];
		}
	}

	// k150030 comment : 왼쪽 가장자리 구하기
	uint Left = pWidth;
	
	for(int Y = 0 ; Y < pHeight ; Y++) {
		for(int X = 0 ; X < pWidth ; X++) {
			printf("%02X ", buffer[X][Y]);
		}
		cout << endl;
	}
	
	for(int Y = 0 ; Y < pHeight ; Y++) {
		for(int X = 0 ; X < pWidth ; X++) {
			if(buffer[X][Y] == 0xFF) {
				const uint temp = X;
				if(temp < Left) {
					Left = temp;
				}
				X = pWidth;
			}
		}
	}

	// k150030 comment : 오른쪽 가장자리 구하기
	uint Right = 0;
	
	for(int Y = pHeight - 1 ; Y >= 0 ; Y--) {
		for(int X = pWidth - 1 ; X >= 0 ; X--) {
			if(buffer[X][Y] == 0xFF) {
				const uint temp = X;
				if(temp > Right) {
					Right = temp;					
				}
				X = -1;
			}
		}
	}
	
	int Width = Right - Left + 3;
	if(Width > 0) {
		printf("Right = %d, Left = %d\n", Right, Left);
	}

	return (Width > 0) ? Width : 0;
}


int main() {
	int Width = 16;
	int Height = 24;	
	uint Length = Width * Height;
	
	uchar buffer[Length];
	memset(buffer, 0, Length);	
	memcpy(buffer, ttx_font_data, Length);
	int printWidth = CalculateFontWidth(buffer, Width, Height, Length);
	cout << "Width = " << printWidth << endl;		
	
	
	// CalculateFontWidth(const uchar *pSrc, const int pWidth, const int pHeight, const int Length);

	return 0;
}


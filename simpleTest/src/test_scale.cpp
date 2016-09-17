#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <string.h>
#include <algorithm>


#define VISIBLE_FOR_ARRAY_TYPE


int main(int argc, char* argv[])
{
	if(2 != argc) {
		puts("input scale size");
		return -1;
	}

	const int width = 2;
	const int rearQRCodeBufferLength = width * width;
	const int scale = atoi(argv[1]);
	const int scaledWidth = width * scale;

#ifdef VISIBLE_FOR_ARRAY_TYPE
	std::vector<std::string> v;
#endif

	for(int i = 0; i < rearQRCodeBufferLength; i++) {
		for(int col = 0; col < scale; col++) {
#ifdef VISIBLE_FOR_ARRAY_TYPE
			const int vIndex = static_cast<int>(i / width) ? col + ((i / width) * scale) + 1/*개행 문자에 대한 오프셋*/ : col + ((i / width) * width);
			if(0 == (i % width)) {
				v.push_back(std::string(""));
			}
			else {
				v[vIndex].append("\t");
			}
#endif

			for(int row = 0; row < scale; row++) {
				int offset = (i / width) * scale * width * scale + (i % width) * scale + row + col * scaledWidth;
#ifdef VISIBLE_FOR_ARRAY_TYPE
				char c[5];
				memset(c, 0x00, sizeof(c));
				sprintf(c, "%3d ", offset);
				v[vIndex].append(c);
#else
				printf("%3d ", offset);
#endif
			}
#ifdef VISIBLE_FOR_ARRAY_TYPE
			if(0 != (i % width)) {
				v[vIndex].append("\n");
			}
#else
			puts("");
#endif
		}
#ifdef VISIBLE_FOR_ARRAY_TYPE
		if(i == (width - 1)) {
			v.push_back(std::string("\n"));
		}
#else
		puts("");
#endif
	}

#ifdef VISIBLE_FOR_ARRAY_TYPE
	std::for_each(v.begin(), v.end(), [] (const std::string Item) { puts(Item.c_str()); });
#endif

	return 0;
}

#include <stdio.h>
#include <string>

//#include "../../../../benjamin/INC/DataType.h"
#include "DataType.h"
/**
  * FontMaker 사용법
  * 1. FontMake 폴더에서 make
  * 2. 변환에 사용될 Font File(ex, .ttf, .otf)을 bin폴더에 폰트 파일 복사
  * 3. 68 Line에 파일명 변경
  * 4. 16x20(기본크기)로 생성하는 경우, 아래 define의 주석 해제하지 말고 bin 폴더에서 FontMaker 실행
  * 5. 크기를 변경하는 경우, 아래의 순서를 따름
  *      1) 22 Line의 #define _CREATE_EXTEND_BASE_ 주석을 해제 (이때 폰트 사이즈는 SET_WIDTH = 16 / SET_HEIGHT = 20이어야 함
  *      2) 49 LIne의 SET_WIDTH_EXTEND 및 SET_HEIGHT_EXTEND에 원하는 크기의 폰트 지정
  *      3) FontMake 폴더에서 make, 그리고 cd ./bin/ && ./FontMaker
  *      4) #define _CREATE_EXTEND_BASE_에 다시 주석을 걸고, //#define _GRAY_EXTEND_ 의 주석 해제
  *      5) 46 Line의 SET_WIDTH / SET_HEIGHT를 원하는 크기로 변경
  *      6) FontMake 폴더에서 make, 그리고 cd ./bin/ && ./FontMaker
  */


// k150030 comment : 폰트 크기 늘릴 시, defind 주석 해제
// #define _CREATE_EXTEND_BASE_
// #define _GRAY_EXTEND_
// k150030

#ifdef _GRAY_EXTEND_
#include "./bin/ttxosd_font_gray_extend_base_data.h"
#include "./bin/ttxosd_font_gray_extend_base_width.h"
#else
#include "ttxosd_font_mono_data.h"
#include "ttxosd_font_mono_width.h"
#endif
#include "font_mapping.h"

#include <boost/format.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SYNTHESIS_H


// #define FT_RESOLUTION_H	57
#define FT_RESOLUTION_H	64
#define FT_RESOLUTION_V	64

// k150030 comment : Original Size 16x20
#define SET_WIDTH	24
#define SET_HEIGHT	24
//#define SET_HEIGHT	24
#ifdef _CREATE_EXTEND_BASE_
#define SET_WIDTH_EXTEND	24
#define SET_HEIGHT_EXTEND	24
#endif


int g_baseline_offset;

FT_Library g_library;
FT_Face g_pFace;


bool InitFreetype()
{
	if(FT_Init_FreeType(&g_library) != FT_Err_Ok) {
		printf("FT_Init_FreeType failed\n");
		return false;
	}

	// if(FT_New_Face(g_library, "./arialbd.ttf", 0, &g_pFace) != FT_Err_Ok) {
	if(FT_New_Face(g_library, "Arial-Unicode-Italic.ttf", 0, &g_pFace) != FT_Err_Ok) {
		printf("FT_New_Face failed\n");
		return false;
	}

	return true;
}

void UninitFreetype()
{
	FT_Done_Face(g_pFace);
	FT_Done_FreeType(g_library);
}

void SetCharSize(int Width, int Height)
{
	FT_Face pFace = g_pFace;

	if(FT_Set_Char_Size(pFace, Width << 6, Height << 6, FT_RESOLUTION_H, FT_RESOLUTION_V) != FT_Err_Ok) {
		printf("FT_Set_Char_Size failed\n");
		return;
	}

	// g_baseline_offset = pFace->descender / (pFace->units_per_EM / (float)Height) * 1.15;
	// g_baseline_offset = pFace->descender / (pFace->units_per_EM / (float)Height);
	float temp = (float)pFace->descender / (float)(pFace->ascender - pFace->descender);
	// g_baseline_offset = temp * (float)Height + 1;
	g_baseline_offset = temp * (float)Height;

	// printf("temp = %f | pFace->descender = %d | pFace->ascender = %d | g_baseline_offset = %d\n", temp, pFace->descender, pFace->ascender, g_baseline_offset);
/*
	g_underline_thickness = pFace->underline_thickness / (pFace->units_per_EM / (float)size);
	if(g_underline_thickness < 1) {
		g_underline_thickness = 1;
	}
*/
}

unsigned int GetUnicode(const unsigned int Index)
{
	const int Length = sizeof(ttx_font_mapping) / sizeof(unsigned int) / 2;

	int iLeft = 0, iRight = Length - 1;

	if( Index < ttx_font_mapping[iLeft][0] || Index > ttx_font_mapping[iRight][0]) {
		return 0;
	}

	while(iLeft <= iRight) {
		const int iMiddle = (iLeft + iRight ) >> 1;

		if( ttx_font_mapping[iMiddle][0] < Index )
			iLeft = iMiddle + 1;
		else if( ttx_font_mapping[iMiddle][0] > Index )
			iRight = iMiddle - 1;
		else
		{
			return ttx_font_mapping[iMiddle][1];
		}
	}

	return 0;
}

bool RenderFont(const unsigned int Index, unsigned char *rBuffer, int Width, int Height, int &rWidth)
{
	const unsigned int charCode = GetUnicode(Index);
	if(charCode == 0) {
		return false;
	}


	FT_Face pFace = g_pFace;
	FT_UInt glyph_index = pFace->charmap ? FT_Get_Char_Index(pFace, static_cast<FT_ULong>(charCode)) : static_cast<FT_UInt>(charCode);

	if(FT_Load_Glyph(pFace, glyph_index, FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_NO_AUTOHINT) != FT_Err_Ok || FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL) != FT_Err_Ok) {
		printf("[0x%04X] Render failed\n", charCode);
		return false;
	}

	pFace->glyph->advance.x >>= 6;
	pFace->glyph->advance.y >>= 6;

	if(pFace->glyph->bitmap_left > pFace->glyph->advance.x) {
		pFace->glyph->bitmap_left = 0;
	}

	int x = pFace->glyph->bitmap_left;
	int y = Height + g_baseline_offset - pFace->glyph->bitmap_top;

	// printf("Index = %3d | Height = %d | g_baseline_offset = %d | bitmap_top = %2d | y = %d\n", Index, Height, g_baseline_offset, pFace->glyph->bitmap_top, y);

	if(x < 0) {
		printf("[0x%04X, 0x%04X] Ignore x : %d\n", Index, charCode, x);
		x = 0;
	}

	if(y < 0) {
		printf("[0x%04X, 0x%04X] Ignore y : %d\n", Index, charCode, y);
		y = 0;
	}

	if(Width < pFace->glyph->bitmap.width) {
		printf("[0x%04X, 0x%04X] Ignore width : %d\n", Index, charCode, pFace->glyph->bitmap.width);
		pFace->glyph->bitmap.width = Width;
	}

	///test
	if(Index == 0x141)
		printf("[0x%04X, 0x%04X] x: %d, y: %d, w: %d, h: %d\n", Index, charCode, x, y, pFace->glyph->bitmap.width, pFace->glyph->bitmap.rows);
	///test
	if(Height < pFace->glyph->bitmap.rows) {
		printf("[0x%04X, 0x%04X] Ignore rows : %d\n", Index, charCode, pFace->glyph->bitmap.rows);
		pFace->glyph->bitmap.rows = Height;
	}

	if(Width < x + pFace->glyph->bitmap.width) {
		const int Diff = x + pFace->glyph->bitmap.width - Width;
		printf("[0x%04X, 0x%04X] Modify x : %d + %d = %d / %d => %d\n", Index, charCode, x, pFace->glyph->bitmap.width, x + pFace->glyph->bitmap.width, Width, x - Diff);
		x -= Diff;
	}

	if(Height < y + pFace->glyph->bitmap.rows) {
		const int Diff = y + pFace->glyph->bitmap.rows - Height;
		printf("[0x%04X, 0x%04X] Modify y : %d + %d = %d / %d => %d\n", Index, charCode, y, pFace->glyph->bitmap.rows, y + pFace->glyph->bitmap.rows, Height, y - Diff);
		y -= Diff;
	}

	if(Width < x + pFace->glyph->bitmap.width) {
		printf("[0x%04X, 0x%04X] Invalid width : %d < %d + %d\n", Index, charCode, Width, x, pFace->glyph->bitmap.width);
		return false;
	}

	if(Height < y + pFace->glyph->bitmap.rows) {
		printf("[0x%04X, 0x%04X] Invalid height : %d < %d + %d\n", Index, charCode, Height, y, pFace->glyph->bitmap.rows);
		return false;
	}

	rWidth = x + pFace->glyph->bitmap.width;

	for(int iRow=0, iRowPitch=0, iOffsetY = Width * y ; iRow < static_cast<int>(pFace->glyph->bitmap.rows) ; iRow++, iRowPitch += pFace->glyph->bitmap.pitch, iOffsetY += Width) {
		for(int iCol=0 ; iCol < static_cast<int>(pFace->glyph->bitmap.width) ; iCol++ ) {
			const unsigned char iAlpha = pFace->glyph->bitmap.buffer[iRowPitch + iCol];
			if(iAlpha == 0) {
				continue;
			}

			rBuffer[iOffsetY + x + iCol] = iAlpha;
		}
	}

	return true;
}

bool WriteFile(const std::string &strPathname, const std::string &strBuffer)
{
	FILE *pFile = fopen(strPathname.c_str(), "wb");
	if(pFile == NULL) {
		return false;
	}

	fwrite(strBuffer.c_str(), strBuffer.size(), 1, pFile);
	fclose(pFile);
	return true;
}

void nearestResampling(const uchar *pSrc, const int srcWidth, const int srcHeight, const int srcPitch, uchar *pDst, const int dstWidth, const int dstHeight)
{
	const int RatioX = static_cast<int>((srcWidth << 16) / dstWidth) + 1;
	const int RatioY = static_cast<int>((srcHeight << 16) / dstHeight) + 1;

	for(int Y = 0 ; Y < dstHeight ; Y++) {
		for(int X = 0 ; X < dstWidth ; X++) {
			const int X2 = X * RatioX >> 16;
			const int Y2 = Y * RatioY >> 16;
			pDst[Y * dstWidth + X] = pSrc[Y2 * srcPitch + X2] ;
		}
	}
}

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

	// k150030 comment : printf Character
	// for(int Y = 0 ; Y < pHeight ; Y++) {
		// for(int X = 0 ; X < pWidth ; X++) {
			// printf("%02X ", buffer[X][Y]);
		// }
		// printf("\n");
	// }

	uint Left = pWidth;

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

	return (Width > 0) ? Width : 0;
}


int main()
{
	if(InitFreetype() == false) {
		return 0;
	}

	const int Width = SET_WIDTH;
	const int Height = SET_HEIGHT;
	const int Length = Width * Height;

	// k150030 add
	#ifdef _CREATE_EXTEND_BASE_
	const int WidthExtend = SET_WIDTH_EXTEND;
	const int HeightExtend = SET_HEIGHT_EXTEND;
	const int LengthExtend = WidthExtend * HeightExtend;
	#endif

	SetCharSize(Width, Height);


	std::string strFontData = "unsigned char ttx_font_data[] = {";
	std::string strFontWidth = "unsigned char ttx_font_width[] = {\n";
	// k150030 add
	#ifdef _CREATE_EXTEND_BASE_
	std::string strFontDataExtend = "unsigned char ttx_font_data[] = {";
	std::string strFontWidthExtend = "unsigned char ttx_font_width[] = {\n";
	#endif

	const unsigned int Count = sizeof(ttx_font_width) / sizeof(unsigned char);
	for(unsigned int Index = 0 ; Index < Count ; Index++) {
		int charWidth = 0;
		unsigned char Pixmap[Length];
		memset(Pixmap, 0, Length);

		if(RenderFont(Index, Pixmap, Width, Height, charWidth) == true) {
		}
		else {
			// unsigned char *pFontData = ttx_font_data + Index * Length;

			for(int Row = 0 ; Row < Height ; Row++) {
				// memcpy(Pixmap + Row * Width, pFontData + Row * Width, Width);
				memset(Pixmap + Row * Width, 0, Width);
			}

			// charWidth = ttx_font_width[Index];
			charWidth = 0;
		}

		strFontData += boost::io::str(boost::format("\n// ### %d (0x%x)\n") % Index % Index);

		std::string strHexData, strDescription;
		for(int Y = 0 ; Y < Height ; Y++) {
			for(int X = 0 ; X < Width ; X++) {
				if(X == 0) {
					strHexData = "";
					strDescription = " //";
				}

				const unsigned char Value = Pixmap[Y*Width + X];
				strHexData += boost::io::str(boost::format("0x%02X,") % static_cast<unsigned int>(Value));
				strDescription += Value == 0xFF ? 'O' : Value == 0 ? '.' : '*';
			}

			strFontData += strHexData;
			strFontData += strDescription;
			strFontData += "\n";
		}

		// k150030 add
		#ifdef _CREATE_EXTEND_BASE_
		unsigned char PixmapExtend[LengthExtend];
		memset(PixmapExtend, 0, LengthExtend);
		nearestResampling(Pixmap, Width, Height, Width, PixmapExtend, WidthExtend, HeightExtend);

		int charWidthExtend = CalculateFontWidth(PixmapExtend, WidthExtend, HeightExtend, LengthExtend);

		strFontDataExtend += boost::io::str(boost::format("\n// ### %d (0x%x)\n") % Index % Index);

		std::string strHexDataExtend, strDescriptionExtend;
		for(int Y = 0 ; Y < HeightExtend ; Y++) {
			for(int X = 0 ; X < WidthExtend ; X++) {
				if(X == 0) {
					strHexDataExtend = "";
					strDescriptionExtend = " //";
				}

				const unsigned char Value = PixmapExtend[Y*WidthExtend + X];
				strHexDataExtend += boost::io::str(boost::format("0x%02X,") % static_cast<unsigned int>(Value));
				strDescriptionExtend += Value == 0xFF ? 'O' : Value == 0 ? '.' : '*';
			}

			strFontDataExtend += strHexDataExtend;
			strFontDataExtend += strDescriptionExtend;
			strFontDataExtend += "\n";
		}
		#endif

		if(Index != 0 && Index % 8 == 0) {
			strFontWidth += "\n";
			#ifdef _CREATE_EXTEND_BASE_
			strFontWidthExtend += "\n";
			#endif
		}

		strFontWidth += boost::io::str(boost::format("/*0x%03X*/ %2d, ") % Index % charWidth);
		#ifdef _CREATE_EXTEND_BASE_
		strFontWidthExtend += boost::io::str(boost::format("/*0x%04X*/ %2d, ") % Index % charWidthExtend);
		#endif
	}

	strFontData += "};\n";
	strFontWidth += "\n};\n";
	#ifdef _CREATE_EXTEND_BASE_
	strFontDataExtend += "};\n";
	strFontWidthExtend += "\n};\n";
	#endif

	// k150030 add
	#ifdef _CREATE_EXTEND_BASE_
	WriteFile("ttxosd_font_gray_extend_base_data.h", strFontDataExtend);
	WriteFile("ttxosd_font_gray_extend_base_width.h", strFontWidthExtend);
	#elif defined _GRAY_EXTEND_
	WriteFile("ttxosd_font_gray_extend_data.h", strFontData);
	WriteFile("ttxosd_font_gray_extend_width.h", strFontWidth);
	#else
	std::string FileNameFontData = boost::io::str(boost::format("ArialUniItalicData%2d.h") %SET_HEIGHT);
	std::string FileNameFontWidth = boost::io::str(boost::format("ArialUniItalicWidth%2d.h") %SET_HEIGHT);
	WriteFile(FileNameFontData, strFontData);
	WriteFile(FileNameFontWidth, strFontWidth);
//	WriteFile("ttxosd_font_gray_data.h", strFontData);
//	WriteFile("ttxosd_font_gray_width.h", strFontWidth);
	#endif

	UninitFreetype();

	return 0;
}

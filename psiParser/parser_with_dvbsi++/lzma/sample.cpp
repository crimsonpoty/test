#include <iostream>
#include <fstream>      // std::ifstream
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Alloc.h"
#include "LzmaDec.h"

using namespace std;

#define LZMA_COMPRESSED_DATA_START_OFFSET	5

typedef std::vector<unsigned char> UncompressedDataVector;
typedef UncompressedDataVector::iterator UncompressedDataListIterator;

UncompressedDataVector UncompressedData;
UncompressedDataListIterator UncompressedDataVectorIt;

unsigned char DummySizeFixedHeaderBuffer[13]={0x5D, 0x00, 0x10, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)

void LzmaDecompress(unsigned char *InStreamBuffer, long long StreamBufferLength)
{
	UncompressedData.clear();
	long long LzmaCompressedDataLength=StreamBufferLength-5;
	unsigned char *EditedLzmaCompressedData=new unsigned char[LzmaCompressedDataLength];
	// 압축 Data 부분만 추출
	memcpy(EditedLzmaCompressedData, &InStreamBuffer[LZMA_COMPRESSED_DATA_START_OFFSET], LzmaCompressedDataLength);

	UInt64 unpackSize;
	CLzmaDec state;

	unpackSize = 0;
	for (int i = 0; i < 8; i++) {
		// Telefonica 압축스트림의 경우 Header를 고정값으로 처리함.
		unpackSize += (UInt64)DummySizeFixedHeaderBuffer[LZMA_PROPS_SIZE + i] << (i * 8);
	}
	// 초기화 부분
	LzmaDec_Construct(&state);
	LzmaDec_Allocate(&state, DummySizeFixedHeaderBuffer, LZMA_PROPS_SIZE, &g_Alloc);

	int thereIsSize = (unpackSize != (UInt64)(Int64)-1);
	Byte *outBuf= new Byte[OUT_BUF_SIZE];
	size_t inPos = 0, inSize = LzmaCompressedDataLength, outPos = 0;
	LzmaDec_Init(&state);
	for (;;) {
		SRes res;
		SizeT inProcessed = inSize - inPos;
		SizeT outProcessed = OUT_BUF_SIZE - outPos;
		ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
		ELzmaStatus status;
		if (thereIsSize && outProcessed > unpackSize) {
			outProcessed = (SizeT)unpackSize;
			finishMode = LZMA_FINISH_END;
		}
		// 압축해제 진행
		res = LzmaDec_DecodeToBuf(&state, outBuf + outPos, &outProcessed,
				EditedLzmaCompressedData + inPos, &inProcessed, finishMode, &status);
		inPos += inProcessed;
		outPos += outProcessed;
		unpackSize -= outProcessed;
		// vector에 압축해제된 Data를 추가함.
		UncompressedDataVectorIt = UncompressedData.end();
		UncompressedData.insert(UncompressedDataVectorIt, outBuf , outBuf+outProcessed);

		outPos = 0;

		if ((res != SZ_OK || thereIsSize) && unpackSize == 0) {
			break;
		}

		if (inProcessed == 0 && outProcessed == 0) {
			break;
		}
	}
	delete outBuf;

	LzmaDec_Free(&state, &g_Alloc);
	delete EditedLzmaCompressedData;
}


int main(int numArgs, const char *args[])
{
	int res;

	if (numArgs < 2 || numArgs > 3 ) {
		cout << args[0] << ": usage: " << args[0] << " [Input File] [Output File]" << endl;
		cout << "ex) "<< args[0] << " TelefonicaIntrawayMessagingLzmaCompressedStream.raw Dump.txt" << endl;
		return 0;
	}

	ifstream is(args[1], std::ifstream::binary);
	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		long long StreamBufferLength = is.tellg();

		cout << "File Size is " << StreamBufferLength << " Bytes !" << endl;

		is.seekg(0, is.beg);
		char *InStreamBuffer=new char[StreamBufferLength];
		is.read(InStreamBuffer, StreamBufferLength);

		LzmaDecompress((unsigned char*)InStreamBuffer, StreamBufferLength);

		delete InStreamBuffer;
	} else {
		return 0;
	}
	// 압축 해제된 것을 파일에 씀.
	ofstream outfile(args[2], std::ofstream::binary);
	unsigned char non_compressed_private_data_buffer;

	for (UncompressedDataListIterator It=UncompressedData.begin(); It != UncompressedData.end(); It++) {
		non_compressed_private_data_buffer=(unsigned char)(*It);
		outfile.write((const char*)&non_compressed_private_data_buffer, 1);
	}

	UncompressedData.clear();
	outfile.close();
	is.close();
}

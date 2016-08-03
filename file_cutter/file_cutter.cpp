#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <string>
#include <iostream>

using namespace std;


const uint32_t BUFFER_SIZE = 1024 * 1024;


enum ECUT_MODE
{
	eECUT_MODE_BEGIN,
	eECUT_MODE_MIDDLE,
	eECUT_MODE_END,
	eECUT_MODE_AUTO
};


void printUsage(char *fc);
uint32_t getCutSizeFromString(string &strCutSize);
void getCutModeString(string &strMode, ECUT_MODE eMode);
void getSizeByUnit(string &strSize, uint32_t Size);
void printProgress(float Percent);


int main(int argc, char *argv[])
{
	string strInputFile, strOutputFile;
	ECUT_MODE eCUT_MODE = eECUT_MODE_AUTO;
	uint32_t CutSize = 0;
	uint32_t CutPoint = 0;
	string strCutSize("");
	string strCutPoint("");

	int opt = 0;
	while(-1 != (opt = getopt(argc, argv, "hi:o:m:l:p:"))) {
		switch(opt)
		{
			case 'i':
				strInputFile = optarg;
				break;

			case 'o':
				strOutputFile = optarg;
				break;

			case 'm':
				eCUT_MODE = static_cast<ECUT_MODE>(atoi(optarg));
				break;

			case 'l':
				strCutSize = optarg;
				break;

			case 'p':
				strCutPoint = optarg;
				break;

			case '?':
				if('f' == optopt) {
					puts("input file name");
				}
				break;

			default:
				printUsage(argv[0]);
				return -1;
		}
	}

	// 입력 파라미터에 대한 예외 처리
	if(0 == strInputFile.size()) {
		printUsage(argv[0]);
	}

	if(0 == strOutputFile.size() || 0 == strOutputFile.compare(strInputFile)) {
		strOutputFile = strInputFile;
		strOutputFile += ".out";
	}


	FILE *fp = fopen(strInputFile.c_str(), "rb");
	FILE *ofp = fopen(strOutputFile.c_str(), "wb");
	if(!(fp && ofp)) {
		puts("File not found");
		return -1;
	}

	fseek(fp, 0L, SEEK_END);	// for 파일 사이즈 계산
	uint32_t FileLength = ftell(fp);
	// printf("FileLength: %uB, %dKB, %dMB\n", FileLength, FileLength/1024);

	if(FileLength <= 0) {
		printf("%s is null\n", strInputFile.c_str());
		return -1;
	}

	// 파일 크기 입력 처리
	CutSize = getCutSizeFromString(strCutSize);
	CutPoint = getCutSizeFromString(strCutPoint);

	// CutSize 및 CutPoint예외처리
	if(CutSize <= 0 || CutSize > FileLength) {
		puts("Cut Size is wrong");
		return -1;
	}

	if(eECUT_MODE_MIDDLE == eCUT_MODE) {
		if(CutPoint <= 0 || (CutPoint + CutSize) > FileLength) {
			puts("Cut Point is wrong");
			return -1;
		}
	}

	// CUT_MODE에 따른 fp 위치(fseek) 처리
	int SeekerPoint = 0;
	switch(eCUT_MODE)
	{
		case eECUT_MODE_MIDDLE:
			SeekerPoint = CutPoint;
			break;
		case eECUT_MODE_END:
			SeekerPoint = FileLength - CutSize;
			break;
		case eECUT_MODE_BEGIN:
		case eECUT_MODE_AUTO:
		default:
			SeekerPoint = 0;
			break;
	}
	fseek(fp, SeekerPoint, 0);


	char *Buffer = NULL;
	printProgress(0.0);

	if(CutSize && CutSize <= FileLength) {
		if(CutSize < BUFFER_SIZE) {
			Buffer = new char[CutSize];
			assert(Buffer);
			fread(Buffer, CutSize, 1, fp);
			fwrite(Buffer, CutSize, 1, ofp);
		}
		else {
			fclose(ofp);
			ofp = fopen(strOutputFile.c_str(), "ab");
			if(!ofp) {
				puts("Fail to write in file");
				return -1;
			}

			Buffer = new char[BUFFER_SIZE];
			assert(Buffer);
			uint32_t RemainSize = CutSize;
			int nR, nW = 0;

			while(RemainSize) {
				if(RemainSize > BUFFER_SIZE) {
					nR = fread(Buffer, BUFFER_SIZE, 1, fp);
					nW = fwrite(Buffer, BUFFER_SIZE, 1, ofp);
					RemainSize -= BUFFER_SIZE;
					printProgress(static_cast<float>(CutSize - RemainSize) / static_cast<float>(CutSize));
				}
				else {
					nR = fread(Buffer, RemainSize, 1, fp);
					nW = fwrite(Buffer, RemainSize, 1, ofp);
					RemainSize = 0;
				}

				if(nR != nW || nR <= 0 || nW <= 0) {
					puts("Fail to write in file");
					return -1;
				}
			}
		}
	}

	printProgress(1.0);
	puts("");

	delete Buffer;
	fclose(fp);
	fclose(ofp);


	string CutModeString("");
	string CutSizeString("");
	getCutModeString(CutModeString, (ECUT_MODE)eCUT_MODE);
	getSizeByUnit(CutSizeString, CutSize);

	// 실행 결과 출력
	printf("Input File\t: %s\n", strInputFile.c_str());
	printf("Output File\t: %s\n", strOutputFile.c_str());
	printf("Cut Mode\t: %s\n", CutModeString.c_str());
	printf("Cut Size\t: %s\n", CutSizeString.c_str());
	puts("");

	return 0;
}


void printUsage(char *fc)
{
	printf("[Usage] %s:\n", fc);
	puts("\t-i : input file name");
	puts("\t-o : output file name");
	puts("\t-m : file cut mode\te.g.,) 0: begin(default), 1: middle, 2:end");
	puts("\t-l : file size for cut\te.g.,)123, 128K, 900M, 1G");
	puts("\t-p : if MIDDLE in file cut mode, input starting point for cut");
}

uint32_t getCutSizeFromString(string &strCutSize)
{
	// TODO: float나 double형 지원
	if(0 == strCutSize.size()) {
		return 0;
	}

	char unit = strCutSize[strCutSize.size() - 1];
	string strNumber = strCutSize;
	strNumber.erase(strNumber.end() - 1);

	for(auto it = strNumber.begin(); it != strNumber.end(); it++) {
		if(0 == isdigit(*it)) {
			return 0;
		}
	}

	uint32_t Number = atoi(strNumber.c_str());
	uint64_t Multiplier = 1;

	switch(unit)
	{
		case 'K': case 'k':
			Multiplier = 1024;
			break;
		case 'M': case 'm':
			Multiplier = 1024 * 1024;
			break;
		case 'G': case 'g':
			Multiplier = 1024 * 1024 * 1024;
			break;
		default:
			Multiplier = 1;
			break;
	}

	return Number *= Multiplier;
}

void getCutModeString(string &strMode, ECUT_MODE eMode)
{
	strMode.clear();

	switch(eMode)
	{
		case eECUT_MODE_MIDDLE:
			strMode = "MIDDLE";
			break;

		case eECUT_MODE_END:
			strMode = "END";
			break;

		case eECUT_MODE_BEGIN:
		case eECUT_MODE_AUTO:
			strMode = "BEGIN";
			break;
	}
}

void getSizeByUnit(string &strSize, uint32_t Size)
{
	strSize.clear();

	double Divider = Size;
	int Count = 0;

	while(static_cast<int>(Divider / 1024)) {
		Divider /= 1024;
		Count++;
		// 무한 루프 방지
		if(Count > 10) break;
	}

	char tBuf[100] = {0,};

	if(Count) {
		sprintf(tBuf, "%.1f", Divider);
	}
	else {
		sprintf(tBuf, "%d", static_cast<int>(Divider));
	}

	strSize = tBuf;

	switch(Count)
	{
		case 1:
			strSize.append("KB");
			break;

		case 2:
			strSize.append("MB");
			break;

		case 3:
			strSize.append("GB");
			break;

		case 0:
		default:
			strSize.append("B");
			break;
	}
}

void printProgress(float Percent)
{
	if(Percent > 1.0) {
		Percent = 1.0;
	}

	const int BarWidth = 70;

	cout << '[';
	int pos = BarWidth * Percent;
	for (int i = 0; i < BarWidth; ++i) {
		if (i < pos) cout << '=';
		else if (i == pos) cout << '>';
		else cout << ' ';
	}
	cout << "] " << int(Percent * 100.0) << " %\r";
	cout.flush();
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <string>
#include <iostream>

using namespace std;

//#define BUFFER_SIZE		(1024 * 1024)
#define BUFFER_SIZE		(1024)


// �̰� ��������
enum ECUT_MODE
{
	eECUT_MODE_BEGIN,
	eECUT_MODE_MIDDLE,
	eECUT_MODE_END,
	eECUT_MODE_AUTO
};


void printUsage(char *fc);
void getCutModeString(string &strMode, ECUT_MODE eMode);
void getSizeByUnit(string &strSize, uint32_t Size);
void printProgress(float Percent);

int main(int argc, char *argv[])
{
	string InputFileName, OutputFileName;
	ECUT_MODE eCUT_MODE = eECUT_MODE_AUTO;
	uint32_t CutSize = 0;
	uint32_t CutPoint = 0;

	int opt = 0;
	while(-1 != (opt = getopt(argc, argv, "hi:o:m:l:p:"))) {
		switch(opt)
		{
			case 'i':
				InputFileName = optarg;
				break;

			case 'o':
				OutputFileName = optarg;
				break;

			case 'm':
				eCUT_MODE = static_cast<ECUT_MODE>(atoi(optarg));
				break;

			case 'l':
				CutSize = atoi(optarg);
				break;

			case 'p':
				CutPoint = atoi(optarg);
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

	// �Է� �Ķ���Ϳ� ���� ���� ó��
	// ���� ũ��� �� �Է� �޾ƾ� ��(���� ũ�⿡�� MB�� GB���� ó�� ����� ��)
	if(!(InputFileName.size() && OutputFileName.size())) {
		printUsage(argv[0]);
	}
	// else if()
	else {
	}


	FILE *fp = fopen(InputFileName.c_str(), "rb");
	FILE *ofp = fopen(OutputFileName.c_str(), "wb");
	if(!(fp && ofp)) {
		puts("File not found");
		return -1;
	}

	// �о�� ������ ���� fp ��ġ(fseek) ó��

	int CutMode = static_cast<int>(eCUT_MODE);	// eCUT_MODE ó��
	CutPoint = 0;
	fseek(fp, 0L, SEEK_END);	// ���� ������
	uint32_t FileLength = ftell(fp);
	printf("FileLength: %uB, %dKB, %dMB\n", FileLength, FileLength/1024, FileLength/(BUFFER_SIZE));
	fseek(fp, CutPoint, CutMode);


	// TODO : CutSize ó�� (�Է� �� ����ó��)
	// CutSize = FileLength;
	char *Buffer = NULL;

	// progress ǥ���ϴ°� ��� �ϴ°�?

	if(CutSize && CutSize <= FileLength) {
		if(CutSize < BUFFER_SIZE) {
			Buffer = new char[CutSize];
			assert(Buffer);
			fread(Buffer, CutSize, 1, fp);
			fwrite(Buffer, CutSize, 1, ofp);
		}
		else {
			fclose(ofp);
			ofp = fopen(OutputFileName.c_str(), "ab");
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

	delete Buffer;
	fclose(fp);
	fclose(ofp);

	string CutModeString("");
	string CutSizeString("");
	getCutModeString(CutModeString, (ECUT_MODE)eCUT_MODE);
	getSizeByUnit(CutSizeString, CutSize);

	// ���� ��� ���
	printf("InputFileName: %s\n", InputFileName.c_str());
	printf("OutputFileName: %s\n", OutputFileName.c_str());
	printf("CutMode: %s\n", CutModeString.c_str());
	printf("CutSize: %s\n", CutSizeString.c_str());
	// printf("CutPoint: %d\n", CutPoint);

	return 0;
}


void printUsage(char *fc)
{
	printf("[Usage] %s:\n", fc);
	puts("\t-i : input file name");
	puts("\t-o : output file name");
	puts("\t-m : file cut mode\te.g.,) 0: begin(default), 1: middle, 2:end");
	puts("\t-b : file size for cut");
	puts("\t-p : if MIDDLE in file cut mode, input starting point for cut");
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
		// ���� ���� ����
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
		case 0:
			strSize.append("B");
			break;

		case 1:
			strSize.append("KB");
			break;

		case 2:
			strSize.append("MB");
			break;

		case 3:
			strSize.append("GB");
			break;

		case 4:
			strSize.append("TB");
			break;

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>

using namespace std;


enum ECUT_MODE
{
	eECUT_MODE_BEGIN,
	eECUT_MODE_MIDDLE,
	eECUT_MODE_END,
	eECUT_MODE_AUTO
};


void printUsage(char *fc);




int main(int argc, char *argv[])
{

	string InputFileName, OutputFileName;
	ECUT_MODE eCUT_MODE = eECUT_MODE_AUTO;
	unsigned int CutSize = 0;
	unsigned int CutPoint = 0;

	int opt = 0;
	while(-1 != (opt = getopt(argc, argv, "hi:o:m:b:p:"))) {
		switch(opt) {
			case 'i':
				InputFileName = optarg;
				break;

			case 'o':
				OutputFileName = optarg;
				break;

			case 'm':
				eCUT_MODE = static_cast<ECUT_MODE>(atoi(optarg));
				break;

			case 'b':
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
	else {	// ���� ���� ��, ������ �ɷ�...
		printf("InputFileName: %s\n", InputFileName.c_str());
		printf("OutputFileName: %s\n", OutputFileName.c_str());
		printf("eCUT_MODE: %d\n", eCUT_MODE);
		printf("CutSize: %d\n", CutSize);
		printf("CutPoint: %d\n", CutPoint);
	}


	FILE *fp = fopen(InputFileName.c_str(), "rb");
	FILE *ofp = fopen(OutputFileName.c_str(), "wb");
	if(!(fp && ofp)) {
		puts("File not found");
		return -1;
	}

	// �о�� ������ ���� fp ��ġ(fseek) ó��

	int CutMode = 0;	// eCUT_MODE ó��
	CutPoint = 0;
	fseek(fp, 0L, SEEK_END);	// ���� ������
	unsigned int FileLength = ftell(fp);
	printf("FileLength: %uB, %dKB, %dMB\n", FileLength, FileLength/1024, FileLength/(1024*1024));
	fseek(fp, CutPoint, CutMode);


	// TODO : CutSize ó�� (�Ƹ� �Է� �� ����ó��??)
	CutSize = FileLength;
	char *Buffer = NULL;

	// progress ǥ���ϴ°� ��� �ϴ°�?

	if(CutSize && CutSize <= FileLength) {
		if(CutSize < 1024*1024) {
			Buffer = new char[CutSize];
			fread(Buffer, CutSize, 1, fp);
			fwrite(Buffer, CutSize, 1, ofp);
		}
		else {
			fclose(ofp);
			ofp = fopen(OutputFileName.c_str(), "ab");
			if(!ofp) {
				puts("Fail to write in file1");
				return -1;
			}

			Buffer = new char[1024*1024];
			unsigned int RemainSize = CutSize;
			int nR, nW = 0;

			while(RemainSize) {
				if(RemainSize > 1024*1024) {
					nR = fread(Buffer, 1024*1024, 1, fp);
					nW = fwrite(Buffer, 1024*1024, 1, ofp);
					RemainSize -= 1024*1024;
				}
				else {
					nR = fread(Buffer, RemainSize, 1, fp);
					nW = fwrite(Buffer, RemainSize, 1, ofp);
					RemainSize = 0;
				}

				if(nR != nW || nR <= 0 || nW <= 0) {
					puts("Fail to write in file3");
					return -1;
				}
			}
		}
	}

	delete Buffer;
	fclose(fp);
	fclose(ofp);


	return 0;
}



void printUsage(char *fc)
{
	printf("[Usage] %s:\n", fc);
	puts("\t-i : input file name");
	puts("\t-o : output file name");
	puts("\t-m : file cut mode\te.g.,) 0: begin, 1: middle, 2:end");
	puts("\t-b : file size for cut");
	puts("\t-p : if MIDDLE in file cut mode, input starting point for cut");
}

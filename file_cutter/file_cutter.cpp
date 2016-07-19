#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
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

	string inputFileName, outputFileName;
	ECUT_MODE eCUT_MODE = eECUT_MODE_AUTO;
	unsigned int cutSize = 0;
	unsigned int cutPoint = 0;

	int opt = 0;
	while(-1 != (opt = getopt(argc, argv, "hi:o:m:b:p:"))) {
		switch(opt) {
			case 'i':
				inputFileName = optarg;
				break;

			case 'o':
				outputFileName = optarg;
				break;

			case 'm':
				eCUT_MODE = static_cast<ECUT_MODE>(atoi(optarg));
				break;

			case 'b':
				cutSize = atoi(optarg);
				break;

			case 'p':
				cutPoint = atoi(optarg);
				break;

			case '?':
				if('f' == optopt) {
					cout << "input file name" << endl;
				}
				break;

			default:
				printUsage(argv[0]);
				return -1;
		}
	}	

	///> 입력 파라미터에 대한 예외 처리
	if(!(inputFileName.size() && outputFileName.size())) {
		printUsage(argv[0]);
	}
	// else if()
	else {
		cout << "inputFileName : " << inputFileName << endl;
		cout <<	"outputFileName : " << outputFileName << endl;
		cout <<	"eCUT_MODE : " << eCUT_MODE << endl;
		cout <<	"cutSize : " << cutSize << endl;
		cout <<	"cutPoint : " << cutPoint << endl;
	}
	
	// TODO : ifstream을 사용하지 말고, fopen으로 파일을 핸들링하고 fwrite시, append모드로 한다거나 하는 작업으로 진행하면 될듯...

	ifstream ifs;
	ifs.open(inputFileName, fstream::in | fstream::binary);
	
	if(ifs.is_open()) {
		ofstream ofs(outputFileName, fstream::binary);
		
		ofs << ifs.rdbuf();
		ofs.close();
	}
	
	ifs.close();

	return 0;
}



void printUsage(char *fc)
{
	cout << "[Usage] " << fc << ": " << endl;
	cout << "\t-i : " << "input file name" << endl;
	cout << "\t-o : " << "output file name" << endl;
	cout << "\t-m : " << "file cut mode" << "\te.g.,) 0: begin, 1: middle, 2:end" << endl;
	cout << "\t-b : " << "file size for cut" << endl;
	cout << "\t-p : " << "if MIDDLE in file cut mode, input starting point for cut" << endl;
}
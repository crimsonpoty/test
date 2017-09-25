#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

struct test
{
	int a;
	int b;
	int c;
};

int main(int argc, char** argv) {

	std::vector<test> Temp;

	test t[] = {
		{1, 2, 3},
		{4, 5, 6},
		{1, 8, 4}
	};

	for(int i = 0; i < (int)(sizeof(t)/sizeof(test)); i++) {
		Temp.push_back(t[i]);
	}

	std::for_each(Temp.begin(), Temp.end(), [] (const test n) {
		std::cout << n.a << " ";
		std::cout << n.b << " ";
		std::cout << n.c <<std::endl;
	});

	int s1 = 1;
	int s3 = 3;
	// std::vector<test>::const_iterator it = std::find_if(Temp.begin(), Temp.end(), [&s1, &s3] (const test n) {
		// return (n.a == s1) && (n.c == s3);
	// });

	std::vector<test>::const_iterator it;
	for(it = Temp.begin(); it != Temp.end(); it++) {
		if(it->a == s1 && it->c == s3)	break;
	}

	if(it == Temp.end()) {
		return -1;
	}

	std::cout << it->b <<std::endl;

	std::cout << std::endl << std::endl << "=========================================" << std::endl << std::endl;

	Temp.clear();

	test iterTest[] = {
		{0, 1, 2},
		{1, 1, 2},
		{2, 1, 2},
		{3, 1, 2},
		{4, 1, 2},
		{5, 1, 2},
		{6, 1, 2},
		{7, 1, 2},
		{8, 1, 2},
	};

	for(int i = 0; i < (int)(sizeof(iterTest)/sizeof(test)); i++) {
		Temp.push_back(iterTest[i]);
	}

	for(auto it = ++Temp.begin(); it != Temp.end(); it+=2) {
		std::cout << (it+1)->a << std::endl;
	}

	std::cout << std::endl << std::endl << "=============== capa test =============" << std::endl << std::endl;

	std::cout << "size = " << Temp.size() << ", capacity = " <<  Temp.capacity() << ", max_size = " <<  Temp.max_size() << std::endl;
	// Temp.clear();
	std::cout << "clear ! | size = " << Temp.size() << ", capacity = " <<  Temp.capacity() << ", max_size = " <<  Temp.max_size() << std::endl;

	std::cout << std::endl << std::endl << "=============== access test =============" << std::endl << std::endl;

	for(int i = 0; i < 20; i++) {
		std::cout << iterTest[i].a << std::endl;
	}

	std::cout << std::endl << std::endl << "=========================================" << std::endl << std::endl;


	std::cout << std::endl << std::endl << "================ erase test =============" << std::endl << std::endl;
	std::vector<int> EraseTest;
	for(int i = 0; i < 9; i ++) {
		EraseTest.push_back(i);
	}

	for(auto it = EraseTest.begin(); it != EraseTest.end();) {
		if((*it) % 2 == 0) {
			it++;
		}
		else {
			EraseTest.erase(it);
		}
	}

	auto itT = EraseTest.begin();
	for(; itT != EraseTest.end(); itT++) {
		std::cout << (*itT) << " ";
	}
	std::cout << "EraseTest.end: " << (*itT) << (*(--itT)) << std::endl;

	std::cout << std::endl << std::endl << "=========================================" << std::endl << std::endl;
	std::cout << std::endl << std::endl << "=============== find test ===============" << std::endl << std::endl;
	auto itEraseTest = EraseTest.end();
	std::cout << "EraseTest.end: " << (*itEraseTest) << std::endl;
	std::cout << std::endl << std::endl << "=========================================" << std::endl << std::endl;

	std::cout << std::endl << std::endl << "=============== insert test ===============" << std::endl << std::endl;

	std::vector<int> tVector;
	int tVectorSize = 43;
	for(int i = 0; i < tVectorSize; i++) {
		tVector.push_back(i);
	}

	const int FrequencyUnit = 5;
	int Quotient = static_cast<int>(tVectorSize / FrequencyUnit);
	int Remainder = tVectorSize % FrequencyUnit;
	std::vector<std::vector<int>> m_vFrequency;

	m_vFrequency.assign(Quotient, std::vector<int>(FrequencyUnit, 0));
	for(int i = 0; i < Quotient; i++) {
		for(int j = 0; j < FrequencyUnit; j++) {
			m_vFrequency[i][j] = tVector[i * FrequencyUnit + j];
		}
	}

	if(Remainder) {
		m_vFrequency.push_back(std::vector<int>(Remainder, 0));
		for(int i = 0; i < Remainder; i++) {
			m_vFrequency[Quotient][i] = tVector[Quotient * FrequencyUnit + i];
		}
	}

	puts("");

	for(auto it = m_vFrequency.begin(); it != m_vFrequency.end(); it++) {
		for(auto it2 = (*it).begin(); it2 != (*it).end(); it2++) {
			std::cout << (*it2) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl << "=========================================" << std::endl << std::endl;


	std::cout << std::endl << std::endl << "=============== replace test ===============" << std::endl << std::endl;

	std::vector<int> replaceVector(9,1);
	replaceVector[5] = 0;
	for(auto & x: replaceVector)
		std::cout << x;
	std::cout << std::endl;
	replaceVector[std::distance(replaceVector.begin(), std::find(replaceVector.begin(), replaceVector.end(), 0))] = 9;
	for(auto & x: replaceVector)
		std::cout << x;
	std::cout << std::endl;

	std::cout << std::endl << std::endl << "=========================================" << std::endl << std::endl;

	if(argc != 2) {
		std::cout << "Input compare number" << std::endl;
		return -1;
	}

	it = Temp.begin();
	for(; it != Temp.end(); it++) {
		if(it->a == std::atoi(argv[1])) break;
	}

	if(it == Temp.end()) {
		std::cout << "The end" << std::endl;
	}
	else {
		std::cout << it->a << std::endl;
	}

	std::cout << std::endl << std::endl << "=============== = Back Up ===============" << std::endl << std::endl;

	std::vector<test> Backup;

	Backup.clear();
	Backup = Temp;

	for(auto it = Backup.begin(); it != Backup.end(); it++) {
		std::cout << it->a << " " << it->b << " " << it->c << " " << std::endl;
	}

	std::vector<test> Copy;
	std::copy(Temp.begin(), Temp.end(), std::back_inserter(Copy));

	std::cout << std::endl << std::endl << "================ copy backup ============" << std::endl << std::endl;

	for(auto it = Copy.begin(); it != Copy.end(); it++) {
		std::cout << it->a << " " << it->b << " " << it->c << " " << std::endl;
	}

	std::cout << std::endl << std::endl << "=============== = Back Up (changed) =====" << std::endl << std::endl;

	Temp[3].a = 0;

	for(auto it = Backup.begin(); it != Backup.end(); it++) {
		std::cout << it->a << " " << it->b << " " << it->c << " " << std::endl;
	}

	std::cout << std::endl << std::endl << "================ copy backup (changed) ==" << std::endl << std::endl;

	for(auto it = Copy.begin(); it != Copy.end(); it++) {
		std::cout << it->a << " " << it->b << " " << it->c << " " << std::endl;
	}

	std::cout << std::endl << std::endl << "================ original Temp ==========" << std::endl << std::endl;

	for(auto it = Temp.begin(); it != Temp.end(); it++) {
		std::cout << it->a << " " << it->b << " " << it->c << " " << std::endl;
	}



	return 0;
}

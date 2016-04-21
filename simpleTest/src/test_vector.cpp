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

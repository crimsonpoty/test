#include <iostream>
#include <unistd.h>

void printProgress(float Percent)
{
	if(Percent > 1.0) {
		Percent = 1.0;
	}
	
	const int BarWidth = 70;

	std::cout << '[';
	int pos = BarWidth * Percent;
	for (int i = 0; i < BarWidth; ++i) {
		if (i < pos) std::cout << '=';
		else if (i == pos) std::cout << '>';
		else std::cout << ' ';
	}
	std::cout << "] " << int(Percent * 100.0) << " %\r";
	std::cout.flush();
	
	if(1.0 == Percent) {
		std::cout << std::endl;
	}
}

int main()
{
	float progress = 0.0;
	while(progress <= 1.5) {
		printProgress(progress);
		progress += 0.09;
		usleep(100 * 1000);
		// if(progress > 1.0) {
			// printProgress(1.0);
			// break;
		// }
	}
	std::cout << progress << std::endl;	
	
	return 0;
}
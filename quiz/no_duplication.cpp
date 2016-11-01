#include <stdio.h>
#include <stdint.h>

bool inspec(int* d, size_t size)
{
	if(size <= 0) {
		return true;
	}

	for(int i = 1; i < (int)size; i++) {
		if(!d[0] || !d[i])
			return false;

		// printf("compare %d == %d, size(%d)\n", d[0], d[i], size);

		if(d[0] == d[i])
			return false;
	}
	return inspec(++d, --size);
}

bool inspec2(int a2, int a4)
{
	if(a2 < 10 || a2 > 99)
		return false;
	if(a4 < 10 || a4 > 99)
		return false;
	return true;
}

int main()
{
	int a0 = 10;
	int a1 = 1;
	int a2 = 10;
	int a3 = 10;
	int a4 = 0;
	int d[9] = {0, };

	// printf("%d\n", inspec(d, 9));

	for(a0 = 10; a0 < 99; a0++) {
		for(a1 = 1; a1 < 9; a1++) {
			a2 = a0 * a1;
			for(a3 = 10; a3 < 99; a3++) {
				a4 = a2 + a3;

				// solution
				// if(a0 == 17 && a1 == 4 && a2 == 68 && a3 == 25 && a4 == 93)
					// continue;

				if(!inspec2(a2, a4))
					continue;

				d[0] = (int)(a0 / 10);
				d[1] = a0 % 10;
				d[2] = a1;
				d[3] = (int)(a2 / 10);
				d[4] = a2 % 10;
				d[5] = (int)(a3 / 10);
				d[6] = a3 % 10;
				d[7] = (int)(a4 / 10);
				d[8] = a4 % 10;

				if(inspec(d, 9)) {
					printf("result : a0(%d), a1(%d), a2(%d), a3(%d), a4(%d)\n", a0, a1, a2, a3, a4);
					return 1;
				}
			}
		}
	}

	printf("result : a0(%d), a1(%d), a2(%d), a3(%d), a4(%d)\n", a0, a1, a2, a3, a4);

	return 0;
}
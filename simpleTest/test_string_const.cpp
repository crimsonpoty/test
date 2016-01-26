#include <stdio.h>
#include <string.h>

using namespace std;

// int main(int argc, char *argv[]) {
int main() {
	
	char Name[15]="Kim Sang Hyung";
	char temp0[5];
	char temp1[6];
	char temp2[6];	
	
	memcpy(temp0, &Name[0], 4);
	temp0[4] = '\0';
	// temp[0][4] = NULL;
	memcpy(temp1, &Name[4], 5);
	temp1[5] = '\0';
	memcpy(temp2, &Name[9], 5);
	temp2[5] = '\0';

	printf("%s | size=%ld\n", Name, sizeof(Name));
	printf("%s | size=%ld\n", temp0, sizeof(temp0));
	printf("%s | size=%ld\n", temp1, sizeof(temp1));
	printf("%s | size=%ld\n", temp2, sizeof(temp2));

	return 0;
}
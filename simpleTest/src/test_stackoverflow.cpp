#include <stdio.h>
#include <unistd.h>

unsigned int gId[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

unsigned int gloV = 0xFFFFFFFF;

void infiniteLoop(void)
{
	while(1) {
		// int stack0 = 0;	int stack1 = 0;	int stack2 = 0;	int stack3 = 0;	int stack4 = 0;	int stack5 = 0;	int stack6 = 0;	int stack7 = 0;	int stack8 = 0;	int stack9 = 0;
		// int stack10 = 0;int stack11 = 0;int stack12 = 0;int stack13 = 0;int stack14 = 0;int stack15 = 0;int stack16 = 0;int stack17 = 0;int stack18 = 0;int stack19 = 0;
		// int stack20 = 0;int stack21 = 0;int stack22 = 0;int stack23 = 0;int stack24 = 0;int stack25 = 0;int stack26 = 0;int stack27 = 0;int stack28 = 0;int stack29 = 0;
		// int stack30 = 0;int stack31 = 0;int stack32 = 0;int stack33 = 0;int stack34 = 0;int stack35 = 0;int stack36 = 0;int stack37 = 0;int stack38 = 0;int stack39 = 0;
		// int stack40 = 0;int stack41 = 0;int stack42 = 0;int stack43 = 0;int stack44 = 0;int stack45 = 0;int stack46 = 0;int stack47 = 0;int stack48 = 0;int stack49 = 0;
		// int stack50 = 0;int stack51 = 0;int stack52 = 0;int stack53 = 0;int stack54 = 0;int stack55 = 0;int stack56 = 0;int stack57 = 0;int stack58 = 0;int stack59 = 0;
		// int stack60 = 0;int stack61 = 0;int stack62 = 0;int stack63 = 0;int stack64 = 0;int stack65 = 0;int stack66 = 0;int stack67 = 0;int stack68 = 0;int stack69 = 0;
		// int stack70 = 0;int stack71 = 0;int stack72 = 0;int stack73 = 0;int stack74 = 0;int stack75 = 0;int stack76 = 0;int stack77 = 0;int stack78 = 0;int stack79 = 0;
		// int stack80 = 0;int stack81 = 0;int stack82 = 0;int stack83 = 0;int stack84 = 0;int stack85 = 0;int stack86 = 0;int stack87 = 0;int stack88 = 0;int stack89 = 0;
		// int stack90 = 0;int stack91 = 0;int stack92 = 0;int stack93 = 0;int stack94 = 0;int stack95 = 0;int stack96 = 0;int stack97 = 0;int stack98 = 0;int stack99 = 0;
		
		// stack0 = stack1 = stack2 = stack3 = stack4 = stack5 = stack6 = stack7 = stack8 = stack9 = 0;
		// stack10 = stack11 = stack12 = stack13 = stack14 = stack15 = stack16 = stack17 = stack18 = stack19 = 0;
		// stack20 = stack21 = stack22 = stack23 = stack24 = stack25 = stack26 = stack27 = stack28 = stack29 = 0;
		// stack30 = stack31 = stack32 = stack33 = stack34 = stack35 = stack36 = stack37 = stack38 = stack39 = 0;
		// stack40 = stack41 = stack42 = stack43 = stack44 = stack45 = stack46 = stack47 = stack48 = stack49 = 0;
		// stack50 = stack51 = stack52 = stack53 = stack54 = stack55 = stack56 = stack57 = stack58 = stack59 = 0;
		// stack60 = stack61 = stack62 = stack63 = stack64 = stack65 = stack66 = stack67 = stack68 = stack69 = 0;
		// stack70 = stack71 = stack72 = stack73 = stack74 = stack75 = stack76 = stack77 = stack78 = stack79 = 0;
		// stack80 = stack81 = stack82 = stack83 = stack84 = stack85 = stack86 = stack87 = stack88 = stack89 = 0;
		// stack90 = stack91 = stack92 = stack93 = stack94 = stack95 = stack96 = stack97 = stack98 = stack99 = 0;
		
		// printf("gId(%lu)\n", gId);
		
		usleep(100);
		
		infiniteLoop();
		
		break;
	}
}

int main() {
	
	printf("%p | %X\n", &gloV, gloV);
	
	unsigned int* change = &gloV;
	*change = 0;

	printf("%p | %X\n", &gloV, gloV);
	
	return 0;
}

/*
 * test_debug_macro.cpp
 *
 *  Created on: 2017. 9. 21.
 *      Author: crims
 */

#include <stdio.h>
#include <vector>

using namespace std;

#define DBG_TRACE(fmt, args...)	printf("[TRACE: %s() at \"%s\"::line(%d)] " fmt"\n", __FUNCTION__, __FILE__, __LINE__, ## args);
#define DBG_TRACE_D1(x)	do { for(auto & p: x) printf("%d ", p); printf("\n");} while(false);
#define DBG_TRACE_D2(x)	do { for(auto & p1: x) { for(auto & p2: p1) printf("%d ", p2); printf("\n"); } printf("\n");} while(false);

int main()
{
	vector<int> v(9);
	vector<vector<int> > v2(9, vector<int>(9));

	for(int i = 0; i < 9; i++)
		v[i] = i + 1;

	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			v2[i][j] = i + j;

	DBG_TRACE_D1(v)
	DBG_TRACE_D2(v2)


	return 0;
}

/*
 * test_array.cpp
 *
 *  Created on: 2017. 9. 25.
 *      Author: crimson_88@naver.com
 */

#include <iostream>
#include <array>

using namespace std;

typedef array<array<int, 9>, 9> SudokuArray;
typedef array<array<int*, 9>, 9> pSudokuArray;

int main()
{
	SudokuArray a;

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			a[i][j] = j;
		}
	}

	cout << "Original:" << endl;
	for(auto & i: a) {
		for(auto & j: i) {
			cout << j << " ";
		}
		cout << endl;
	}

	pSudokuArray row;
	pSudokuArray col;
	pSudokuArray cell;

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			int x = (i / 3) * 3 + (j / 3);
			int y = (i % 3) * 3 + (j % 3);
			row[i][j] = &a[i][j];
			col[i][j] = &a[j][i];
			cell[x][y] = &a[i][j];
		}
	}

	cout << "Row" << endl;
	for(auto & i: row) {
		for(auto & j: i) {
			cout << *j << " ";
		}
		cout << endl;
	}

	cout << "Col" << endl;
	for(auto & i: col) {
		for(auto & j: i) {
			cout << *j << " ";
		}
		cout << endl;
	}

	cout << "Cell" << endl;
	for(auto & i: cell) {
		for(auto & j: i) {
			cout << *j << " ";
		}
		cout << endl;
	}

	return 0;
}

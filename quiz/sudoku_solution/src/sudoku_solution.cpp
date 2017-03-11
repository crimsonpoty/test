//============================================================================
// Name        : sudoku_solution.cpp
// Author      : crimson
// Version     : 0.1
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <numeric>      // std::accumulate
#include <functional> // less

using namespace std;


class CSudoku
{
public:
	CSudoku();

	void Input();
	void Prepare();
	bool isFillSub(int Row, int Col, int Target);
	int Solve();
	bool UniqueCheck(vector<int> & vRef);
	bool FinalInspection();
	void Print();

public:
	vector<vector<int>> mBox;
	vector<vector<int>> mHorizontal;
	vector<vector<int>> mVertical;
	vector<vector<vector<int>>> mSub;
};


CSudoku::CSudoku()
{
	mBox.assign(9, vector<int>(0));

	vector<vector<int>> initVector (9, vector<int>(9, 0));
	mHorizontal = initVector;
	mVertical = initVector;

	mSub.assign(9, vector<vector<int>>(9));
}

void CSudoku::Input()
{
	char _input[50];

	for(int i = 0; i < 9; i++) {
		// ���κ��� �Է¹ޱ�
		memset(_input, 0x00, sizeof(_input));
		cin.getline(_input, sizeof(_input));

		// ������ �������� ���ڿ� �и�
		stringstream ss(_input);
		string buffer;
		vector<string> tokens;

		while(ss >> buffer) {
			tokens.push_back(buffer);
		}

		for(int j = 0; j < 9; j++) {
			mHorizontal[i][j] = atoi(tokens[j].c_str());
		}
	}
}

void CSudoku::Prepare()
{
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			// �Է¹��� �� ��Ҹ� �� ��ҿ� ����
			mVertical[j][i] = mHorizontal[i][j];

			// �Է¹��� �� ��Ҹ� �ڽ� ��ҿ� ����
			int row = (i / 3) * 3;
			int col = j / 3;
			mBox[row + col].push_back(mHorizontal[i][j]);
		}
	}

	// �� ���� �ĺ��׸� ä���
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			for(int k = 1; k <= 9; k++) {
				if(isFillSub(i, j, k)) {
					mSub[i][j].push_back(k);
				}
			}
		}
	}
}

bool CSudoku::isFillSub(int Row, int Col, int Target)
{
	bool check = false;

	// ä������ ���� �˻�
	if(0 == mHorizontal[Row][Col])  {
		int bRow = (Row / 3) * 3;
		int bCol = Col / 3;

		if(find(mBox[bRow + bCol].begin(), mBox[bRow + bCol].end(), Target)		== mBox[bRow + bCol].end()
			&& find(mHorizontal[Row].begin(), mHorizontal[Row].end(), Target)	== mHorizontal[Row].end()
			&& find(mVertical[Col].begin(), mVertical[Col].end(), Target)		== mVertical[Col].end()
			) {
			check = true;
		}
	}

	return check;
}

int CSudoku::Solve()
{
	// TODO: ���� Hard�� �ذ����� ����, Solve ������ ��� �ʿ�
	// ���1: ���� Ǫ�� �� ó�� 2���� ä��� ������� Ǯ ���ΰ�
	// ���2: ��Ǫ�� ��Ȳ���� ��� ���� ��� ó���� ���ΰ�

	int count = 0;

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			if(1 == mSub[i][j].size()) {
				int value = mSub[i][j][0];
				int row = (i / 3) * 3 + (j / 3);
				int col = (i % 3) * 3 + (j % 3);

				// �ĺ����� 1���� ��, ������ ����
				mBox[row][col] = value;
				mHorizontal[i][j] = value;
				mVertical[j][i] = value;

				mSub[i][j].clear();

				// ä���� ���� �ĺ������� ���ִ� �κ�
				for(int k = 0; k < 9; k++) {
					auto it = find(mSub[i][k].begin(), mSub[i][k].end(), value);
					if(it != mSub[i][k].end()) {
						mSub[i][k].erase(it);
					}

					it = find(mSub[k][j].begin(), mSub[k][j].end(), value);
					if(it != mSub[k][j].end()) {
						mSub[k][j].erase(it);
					}
				}

				count++;
			}
		}
	}

	return count;
}

bool CSudoku::UniqueCheck(vector<int> & vRef)
{
	// �ߺ� ���� ����
	vector<int> v(vRef);
	std::sort(v.begin(), v.end(), std::less<int>());
	auto pos = std::unique(v.begin(), v.end() );
	v.erase(pos, v.end());

	// ��ȿ�� �˻�
	if(45 != accumulate(v.begin(), v.end(), 0)) {
		return false;
	}

	return true;
}

bool CSudoku::FinalInspection()
{
	bool complete = true;

	for(int i = 0; i < 9; i++) {
		complete = UniqueCheck(mBox[i]);
		complete = UniqueCheck(mHorizontal[i]);
		complete = UniqueCheck(mVertical[i]);

		for(int j = 0; j < 9; j++) {
			if(!mSub[i][j].empty()) {
				complete = false;
			}
		}
	}
	return complete;
}

void CSudoku::Print()
{
#if 1
	cout << "Output: " << endl << endl;

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			cout << mHorizontal[i][j] << " ";
			if(0 == (j + 1) % 3) cout << " ";
		}
		if(0 == (i + 1) % 3) cout << endl;
		cout << endl;
	}
	cout << endl;
#else
	cout << "Rectangle: " << endl;

//	for_each(mBox.begin(), mBox.end(), [] (const int i) {
//		cout << i << " ";
//	});
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			cout << mBox[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Horizontal: " << endl;

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			cout << mHorizontal[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Vertical: " << endl;

	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			cout << mVertical[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
#endif
}


int main() {
	CSudoku _sudoku;

	_sudoku.Input();
	_sudoku.Prepare();

#if 0 // test mSub
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			cout << "(" << i << "," << j << ")" << endl;
			for(int k = 0; k < _sudoku.mSub[i][j].size(); k++) {
				cout << _sudoku.mSub[i][j][k] << " ";
			}
			cout << endl;
		}
	}
#endif

	int count = 0;
	while(_sudoku.Solve()) {
		if(++count > 1000) break;
	};

	cout << endl << endl << "count: " << count << " / ";

	if(_sudoku.FinalInspection()) {
		_sudoku.Print();
	}
	else {
		cout << "Do not solve yet..." << endl;
	}

	return 0;
}


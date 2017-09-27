/* Sudoku.i */
%module cppSudoku
%{
#include "Sudoku.h"
%}

%include std_string.i

class CSudoku
{
public:
	CSudoku();
	void Input(std::string InputStr);
	bool Solve();
	bool Inspection();
	std::string GetSolvedSudoku();
};

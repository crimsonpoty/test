rem @echo off
rem assume that source file(Sudoku.cpp) already was compiled in SRC_PATH.

set OBJ_PATH="C:\Users\crims\workspace\repo\git\practice\sudoku_solution\Debug\src"
set SRC_PATH="C:\Users\crims\workspace\repo\git\practice\sudoku_solution\src"
set PY_INCLUDE_PATH="C:\Users\crims\AppData\Local\Programs\Python\Python36-32\include"
set PY_LIB_PATH="C:\Users\crims\AppData\Local\Programs\Python\Python36-32\libs"

REM copy "%OBJ_PATH%\Sudoku.o" .

swig -c++ -python Sudoku.i

REM [Windows]
g++ -c Sudoku_wrap.cxx -fPIC -Wall -I%PY_INCLUDE_PATH% -I%SRC_PATH%
g++ -o _cppSudoku.pyd %OBJ_PATH%"\Sudoku.o" Sudoku_wrap.o -Wall -shared -lpython36 -L%PY_LIB_PATH%

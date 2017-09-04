rem @echo off

set SRC_PATH="C:\Users\crims\workspace\repo\git\practice\sudoku_solution\src"
set OBJ_PATH="%CD%\src"
set INCLUDE_PATH=%OBJ_PATH%
set PY_INCLUDE_PATH="C:\Users\crims\AppData\Local\Programs\Python\Python36-32\include"
set PY_LIB_PATH="C:\Users\crims\AppData\Local\Programs\Python\Python36-32\libs"

if not exist "src" (mkdir "src")

copy "%SRC_PATH%\Sudoku.h" src
copy "%SRC_PATH%\Sudoku.cpp" src

g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -o %OBJ_PATH%"\Sudoku.o" %OBJ_PATH%"\Sudoku.cpp" 

swig -c++ -python Sudoku.i

REM [pyd lib file for Windows]
g++ -c Sudoku_wrap.cxx -fPIC -Wall -I%PY_INCLUDE_PATH% -I%INCLUDE_PATH%
g++ -o _cppSudoku.pyd %OBJ_PATH%"\Sudoku.o" Sudoku_wrap.o -Wall -shared -lpython36 -L%PY_LIB_PATH%

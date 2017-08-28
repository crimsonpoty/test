g++ -c Multiple.cpp -Wall

swig -c++ -python Multiple.i

REM [Windows]
g++ -c Multiple_wrap.cxx -fPIC -Wall -I"C:\Users\crims\AppData\Local\Programs\Python\Python36-32\include"
g++ -o _cppMultiple.pyd Multiple.o Multiple_wrap.o -Wall -shared -lpython36 -L"C:\Users\crims\AppData\Local\Programs\Python\Python36-32\libs"

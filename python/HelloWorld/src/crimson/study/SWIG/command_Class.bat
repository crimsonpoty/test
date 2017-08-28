g++ -c Class.cpp -Wall

swig -c++ -python Class.i

REM [Windows]
g++ -c Class_wrap.cxx -fPIC -Wall -I"C:\Users\crims\AppData\Local\Programs\Python\Python36-32\include"
g++ -o _cppClass.pyd Class.o Class_wrap.o -Wall -shared -lpython36 -L"C:\Users\crims\AppData\Local\Programs\Python\Python36-32\libs"

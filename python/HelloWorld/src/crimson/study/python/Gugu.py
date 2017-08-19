# -*- coding: UTF-8 -*-
'''
Created on 2017. 8. 16.

@author: crims
'''

def Gugu(_input):
    retList = []
    
    for i in range(1, 10):
        element = i * int(_input)
        retList.append(element)
        
    return retList


if __name__ == '__main__':
    while True:
        _input = input("출력할 단을 입력하세요(0을 입력하면 종료됩니다.): ")
        if int(_input) == 0:
            break
        
        a = Gugu(int(_input))        
            
        print("결과값: %s" % a)
            
# -*- coding: UTF-8 -*-
'''
Created on 2017. 8. 15.

@author: crims
'''

def sum(arg1, arg2):
    return arg1 + arg2


def safe_sum(arg1, arg2):
    if type(arg1) != type(arg2):
        print("더할 수 있는 값이 아닙니다.")
        return
    else:
        return sum(arg1, arg2)

# for test
if __name__ == "__main__":
    print(safe_sum(3, 4))
    print(safe_sum(10, 2))
    print(safe_sum(1, 'a'))
    
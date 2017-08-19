# -*- encoding: UTF-8 -*-
# memo.py

'''
Created on 2017. 8. 19.

@author: crims
'''

import sys
import os

option = sys.argv[1]
if option == "-w" or option == "-a":
    memo = sys.argv[2]

fileName = "Memo.txt"
 
def memoRead():
    if os.path.exists(fileName) and option == '-r':
        f = open(fileName, option[1])
        msg = f.read()
        print(msg)
        f.close()
    else:
        print("Do not exist %s" % fileName)
     
def memoWrite():
    if option == "-w" or option == "-a":
        f = open(fileName, option[1])
        f.write(memo)
        f.write('\n')
        f.close()
 
if __name__ == '__main__':
    if option == "-r":
        memoRead()
    elif option == "-w" or option == "-a":
        memoWrite()
    else:
        print("Do not support option: %s" % option)
     
     
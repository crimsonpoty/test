# -*- conding: UTF-8 -*-

'''
Created on 2017. 8. 19.

@author: crims
'''

import sys
from PyQt5.Qt import QApplication
from PyQt5.QtGui import QWidget

if __name__ == '__main__':
    a = QApplication(sys.argv)
    
    w = QWidget()
    w.resize(320, 240)
    w.setWindowTitle("Hello World!")
    w.show()
    
    sys.exit(a.exec_())
    
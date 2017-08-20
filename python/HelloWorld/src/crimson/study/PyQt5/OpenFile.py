# -*- conding: UTF-8 -*-

'''
Created on 2017. 8. 19.

@author: crims
'''
import sys
from PyQt5.Qt import QApplication
from PyQt5.QtGui import QFileOpenEvent
from PyQt5.QtWidgets import QWidget

app = QApplication(sys.argv)

w = QWidget
w.resize(700, 80)
w.setWindowTitle("OpenFile")

fileName = QFileDialog.getOpenFileName(w, 'Open File', '', '')
fileName = QFileOpenEvent.openFile(QFile, Union)    

print(len(fileName))

textbox = QLineEdit(w)
textbox.move(20, 20)
textbox.resize(660, 40)
textbox.setText(fileName)

w.show()

sys.exit(app.exec_())
# -*- conding: UTF-8 -*-

'''
Created on 2017. 8. 19.

@author: crims
'''

import sys
from PyQt5.QtWidgets import QWidget, QApplication, QMainWindow

class App(QMainWindow):
    def __init__(self):
        super().__init__()
        self.title = "Window Test"
        self.left = 200
        self.top = 200
        self.width = 640
        self.height = 480
        self.initUI()
        
    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        self.statusBar().showMessage("Message in statusbar")
        self.show()
        

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = App()
    sys.exit(app.exec_())
    
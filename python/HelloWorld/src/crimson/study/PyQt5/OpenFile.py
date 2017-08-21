# -*- conding: UTF-8 -*-

'''
Created on 2017. 8. 19.

@author: crims
'''
import sys
from PyQt5.QtWidgets import *

class fileDialog(QWidget):
    def __init__(self):
        super().__init__()
        self.fileName = None
        self.initUI()
    
    def initUI(self): 
        self.setWindowTitle("File Dialog")
        self.setGeometry(200, 200, 640, 480)
        textbox = QLineEdit(self)
        textbox.move(20, (480 // 2) - 20 )
        textbox.resize(600, 40)
        self.fileName = self.openFileNameDialog()
        self.openFileNamesDialog()
        self.saveFileDialog()
        
        textbox.setText(self.fileName)
        self.show()
            
    def openFileNameDialog(self):    
        options = QFileDialog.Options()
#         options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getOpenFileName(self, "QFileDialog.getOpenFileName()", "","All Files (*);;Python Files (*.py)", options=options)
        if fileName:
            print(fileName)
            return fileName
        
    
    def openFileNamesDialog(self):    
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        files, _ = QFileDialog.getOpenFileNames(self, "QFileDialog.getOpenFileNames()", "","All Files (*);;Python Files (*.py)", options=options)
        if files:
            print(files)
    
    def saveFileDialog(self):    
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getSaveFileName(self, "QFileDialog.getSaveFileName()","","All Files (*);;Text Files (*.txt)", options=options)
        if fileName:
            print(fileName)


app = QApplication(sys.argv)
ex = fileDialog()
sys.exit(app.exec_())
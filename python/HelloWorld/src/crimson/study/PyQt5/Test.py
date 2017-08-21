# -*- coding: UTF-8 -*-

'''
Created on 2017. 8. 20.

@author: crims
'''
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import pyqtSlot, QCoreApplication

class MyDialog(QDialog):
    def __init__(self):
        super(QDialog, self).__init__()
        
        # 다이얼로그 제목 설정
        self.setWindowTitle("My Dialog")
         
        # 레이블, Edit 버튼(텍스트), 선택 버튼, 버튼 컨트롤 
        lblName = QLabel("Name")
        self.textBox = QLineEdit()
        btnSelect = QRadioButton("Select")
        btnOk = QPushButton("OK")
        btnShowText = QPushButton("Show Text", self)
         
        # 레이아웃
        layout = QVBoxLayout()
        layout.addWidget(lblName)
        layout.addWidget(self.textBox)
        layout.addWidget(btnSelect)
        layout.addWidget(btnOk)
        layout.addWidget(btnShowText)
         
        # 다이얼로그 레이아웃 지정
        self.setLayout(layout)
         
        # 이벤트 핸들러 등록
        btnOk.clicked.connect(self.btnOkClicked)
        btnShowText.clicked.connect(self.btnShowTextClicked)
        
        self.show()
    
    @pyqtSlot()         
    def btnOkClicked(self): # btnOk 버튼 클릭 시 이벤트
        # 메시지박스 띄우기
        btnReply = QMessageBox.question(self, "Question", "Are you OK?", QMessageBox.Yes | QMessageBox.No | QMessageBox.Cancel)
        if btnReply == QMessageBox.Yes:
            print("Yes clicked")
        elif btnReply == QMessageBox.No:
            print("No clicked")
        elif btnReply == QMessageBox.Cancel:
            print("Cancel clicked")

    @pyqtSlot()         
    def btnShowTextClicked(self): # btnShowText 버튼 클릭 시 이벤트
        infoText = self.textBox.text()
        # 메시지박스 띄우기
        QMessageBox.information(self, "Info", infoText)
         
        
class MyWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.title = "My Window"
        self.left = 200
        self.top = 200
        self.width = 640
        self.height = 480
        self.initUI()
        
    def initUI(self):
        # 윈도우 특성 설정
        self.setWindowTitle(self.title)    # 윈도우 제목 설정
        self.setGeometry(self.left, self.top, self.width, self.height)    # 윈도우 위치, 크기 설정
#         self.setWindowIcon("sample.png")    # 아이콘 설정    
        self.statusBar().showMessage("준비")
        
        # 메뉴바 설정
        mainMenu = self.menuBar()
        fileMenu = mainMenu.addMenu("File")
        editMenu = mainMenu.addMenu("Edit")
        viewMenu = mainMenu.addMenu("View")
        searchMenu = mainMenu.addMenu("Search")
        toolsMenu = mainMenu.addMenu("Tools")
        helpMenu = mainMenu.addMenu("Help")
        
        exitInFileMenu = QAction(QIcon(), "Exit", self)
        exitInFileMenu.setShortcut("Ctrl+Q")
        exitInFileMenu.setStatusTip("Exit application")
        exitInFileMenu.triggered.connect(self.close)
        fileMenu.addAction(exitInFileMenu)        
        
        # btn1 추가
        btn1 = QPushButton("메시지 버튼", self)
        btn1.setToolTip("이 버튼을 누르면 <b>메시지 박스</b>가 나옴")
#         btn1.resize(btn1.sizeHint())
        leftMargin = 20
        bottomMargin = 80
        btn1Left = self.width - 220 - leftMargin
        btn1Top =  self.height - bottomMargin
        btn1.move(btn1Left, btn1Top)
        
        # btnQuit 추가
        btnQuit = QPushButton("종료", self)
        btnQuitLeft = btn1Left + 100
        btnQuitTop = btn1Top
        btnQuit.move(btnQuitLeft, btnQuitTop)
        

        # 이벤트 핸들러 등록
        btn1.clicked.connect(self.btnClicked)
        btnQuit.clicked.connect(QCoreApplication.instance().quit)
        
        # 윈도우에 화면 표시
        self.show()
    
    @pyqtSlot()    
    def btnClicked(self):
        QMessageBox.information(self, "Info", "This if just information")
        

def main():
    app = QApplication(sys.argv)
    
    # 다이얼로그 실행
#     ex = MyDialog()
#     sys.exit(ex.exec_())
    
    # 메인윈도우 실행
    ex = MyWindow()
    sys.exit(app.exec_())
    
if __name__ == '__main__':
    main()
    



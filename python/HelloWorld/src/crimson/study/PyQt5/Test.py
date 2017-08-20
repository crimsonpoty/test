# -*- coding: UTF-8 -*-

'''
Created on 2017. 8. 20.

@author: crims
'''
import sys
from PyQt5.QtGui import *
from PyQt5.QtCore import QCoreApplication
# from PyQt4.QtGui import QApplication, QDialog, QLabel, QLineEdit, QPushButton,\
#     QVBoxLayout

class MyDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        
        # 레이블, Edit 버튼(텍스트), 선택 버튼, 버튼 컨트롤 
        lblName = QLabel("Name")
        self.editName = QLineEdit()
        btnSelect = QRadioButton("Select")
        btnOk = QPushButton("OK")
        
        # 레이아웃
        layout = QVBoxLayout()
        layout.addWidget(lblName)
        layout.addWidget(self.editName)
        layout.addWidget(btnSelect)
        layout.addWidget(btnOk)
        
        # 다이얼로그 레이아웃 지정
        self.setLayout(layout)
        
        # 이벤트 핸들러 등록
        btnOk.clicked.connect(self.btnOkClicked)
        
    def btnOkClicked(self): # btnOk의 클릭 시 이벤트
        name = self.editName.text()
        # 메시지박스 띄우기
        QMessageBox.information(self, "Info", name)
        
        
class MyWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        
        # 윈도우 특성 설정
        self.setWindowTitle("My Window")    # 윈도우 제목 설정
        self.setGeometry(600, 600, 400, 400)    # 윈도우 위치, 크기 설정
#         self.setWindowIcon("sample.png")    # 아이콘 설정    
        self.statusBar().showMessage("준비")
        
        # btn1 추가
        btn1 = QPushButton("메시지 버튼", self)
        btn1.setToolTip("이 버튼을 누르면 <b>메시지 박스</b>가 나옴")
        btn1.resize(btn1.sizeHint())
        btn1.move(50, 50)
        btn1.clicked.connect(self.btnClicked)
        
        # btnQuit 추가
        btnQuit = QPushButton("종료", self)
        btnQuit.move(50, 100)
        btnQuit.clicked.connect(QCoreApplication.instance().quit)
        
        # 윈도우에 화면 표시
        self.show()
        
    def btnClicked(self):
        QMessageBox.information(self, "버튼", "버튼 클릭!")
        

def main():
    # 다이얼로그 실행
#     app = QApplication([])
#     dialog = MyDialog()
#     dialog.show()
#     app.exec_()
    
    # 메인윈도우 실행
    app = QApplication(sys.argv)
    ex = MyWindow()
    sys.exit(app.exec_())
    
if __name__ == '__main__':
    main()
    



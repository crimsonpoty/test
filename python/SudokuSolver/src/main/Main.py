# -*- coding: UTF-8 -*-

'''
Created on 2017. 8. 31.

@author: crims
'''

import sys

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *

from cpp import cppSudoku
from learning import TrimImage
from learning import DigitRecognition


class Display(QDialog):
    def __init__(self):
        super().__init__()
        
        # 스도쿠 숫자 초기화
        self.numbers = "0" * 81
        
        # 윈도우 속성 설정
        self.title = "Sudoku Solver"
        self.left = 100
        self.top = 100
        self.width = 500
        self.height = 500
        
        # 윈도우 제목 창 버튼 설정(Hint 버튼(?) 제거)
        self.setWindowFlags(Qt.WindowCloseButtonHint)        
        
        self.initUI()
        
    def initUI(self):
        self.setWindowTitle(self.title)
        
        # 윈도우 위치 및 크기(가로, 세로) 설정
        self.setGeometry(self.left, self.top, self.width, self.height)
        
        # 그리드 레이아웃 설정        
        self.createGridLayout()
        
        # 수평(버튼) 레이아웃 설정
        self.createdHorizontalLayout()
        
        windowLayout = QVBoxLayout()
        
        windowLayout.addWidget(self.gridGroupBox)
        windowLayout.addWidget(self.horizontalGroupBox)
        windowLayout.setStretch(0, 5)
        windowLayout.setStretch(1, 1)
        self.setLayout(windowLayout)

        self.show()
        
    def createGridLayout(self):
        self.gridGroupBox = QGroupBox()
        layout = QGridLayout()
        layout.setSpacing(0)
        
        # 레이블 설정 관련
        lblFont = QFont('', 24)
        styleStrOdd = "background-color: white; border: 1px solid black;"
        styleStrEven = "background-color: lightgray; border: 1px solid black;"
        
        # 레이블 생성
        self.lblList = []
        for i in range(9):
            tmpList = []
            for j in range(9):
                tmpList.append(QLabel("0"))
            self.lblList.append(tmpList)
            
        i = 0
        for lbl1 in self.lblList:
            j = 0        
            for lbl2 in self.lblList[i]:
               # 레이블 위치 지정
               lbl2.setGeometry(QRect(i * 50, j * 50, 50, 50))
               
               # 레이블 얼라인 설정
               lbl2.setAlignment(Qt.AlignCenter)
                     
               # 레이블 폰트 설정
               lbl2.setFont(lblFont)            
                      
               # 레이블 배경색 설정
               if self.calCellNum(i, j) % 2 == 0:
                   lbl2.setStyleSheet(styleStrOdd)
               else:
                   lbl2.setStyleSheet(styleStrEven)
               
               # 레이블을 그리드박스에 추가            
               layout.addWidget(lbl2, i, j)                
               j += 1
            i += 1
        
        self.gridGroupBox.setLayout(layout) 
        
    def calCellNum(self, row, col):
        return (row // 3) * 3 + (col // 3)
    
    def setNumbers(self, strInput):
        if 81 == len(strInput):
            self.numbers = strInput
            self.refreshDisplay()
        else:
            print("Input Value is wrong length(%d), it should be %d" % (len(strInput), len(self.numbers)))
    
    def refreshDisplay(self):
        for i in range(9):
            for j in range(9):
                self.lblList[i][j].setText(self.numbers[(i * 9) + j])
        
    def createdHorizontalLayout(self):
        self.horizontalGroupBox = QGroupBox()
        layout = QHBoxLayout()

        # 버튼 생성
        btnOpen = QPushButton("Open")
        btnSolve = QPushButton("Solve")
        btnClose = QPushButton("Close")
        
        # 버튼 사이즈 조정
        btnOpen.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)
        btnSolve.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)
        btnClose.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)
        
        # 이벤트 핸들러 등록
        btnOpen.clicked.connect(self.BtnOpenclicked)
        btnSolve.clicked.connect(self.BtnSolveclicked)
        btnClose.clicked.connect(QCoreApplication.instance().quit)
        
        # 버튼 위젯 등록
        layout.addWidget(btnOpen)
        layout.addWidget(btnSolve)
        layout.addWidget(btnClose)
        
        self.horizontalGroupBox.setLayout(layout)
        
    def openFileNameDialog(self):
        options = QFileDialog.Options()
        fileName, _ = QFileDialog.getOpenFileName \
            (self, "Open", "","PNG, JPEG, Bitmap (*.png;*.jpg;*.jpeg;*.bmp);;All File (*)", options=options)
#             (self, "Open", "","PNG (*.png);;JPEG (*.jpg;*.jpeg);;Bitmap (*.bmp);;All File (*)", options=options)

        if fileName:
            return fileName
    
    @pyqtSlot()
    def BtnOpenclicked(self):
        # for test
#         strInput = "080000001000840300200010067020057009017000080090060075630000700070586000100009600"
#         self.setNumbers(strInput)

#         imgPath = 'C:/Users/crims/workspace/repo/git/test/python/SudokuSolver/src/main/learning/pictures/IMG_01.png'    # for test
        
        imgPath = self.openFileNameDialog()
        samplesPath = 'learning/data/DigitSamples.data'
        responsesPath = 'learning/data/DigitResponses.data'

        # 숫자가 있는 9x9의 사각형 추출
        if imgPath:   
            _t = TrimImage.TrimImage(imgPath)
            im = _t.extract()
              
            # 추출된 사각형을 learning한 데이터를 사용하여 숫자 문자열로 받기   
            _d = DigitRecognition.DigitRecognition(im, samplesPath, responsesPath)
            self.setNumbers(_d.getSudokuString())
         
    @pyqtSlot()
    def BtnSolveclicked(self):
        if 0 == int(self.numbers):
            QMessageBox.information(self, "Hint", "Please, Open Sudoku Picture before press solve button")
        else:
            _sudoku = cppSudoku.CSudoku()
            _sudoku.Input(self.numbers)
            result = _sudoku.Solve()
            
            if(result):
                self.setNumbers(_sudoku.GetSolvedSudoku())
            else:
                QMessageBox.warning(self, "Warning", "Solved Result is False")


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Display()
    sys.exit(app.exec_())
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
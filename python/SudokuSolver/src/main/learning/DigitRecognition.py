# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 2.

@author: crims
'''

import numpy as np
import cv2
import imutils


# 숫자 인식(Digit Recognition) 예제 참조
# 출처: https://stackoverflow.com/questions/9413216/simple-digit-recognition-ocr-in-opencv-python
class DigitRecognition():
    def __init__(self, imgData, samplesDataPath, responsesDataPath):        
        # 이미지 데이터 변수 초기화
        self.im = imgData
        
        # 이미지 프로세싱을 위해 필터 사용
        im_gray = cv2.cvtColor(self.im, cv2.COLOR_BGR2GRAY)
        im_gray_blurred = cv2.GaussianBlur(im_gray, (5, 5), 0)
        self.thresh = cv2.adaptiveThreshold(im_gray_blurred, 255, 1, 1, 11, 2)
    
        # 러닝된 데이터 로드
        self.samples = np.loadtxt(samplesDataPath, np.float32)
        self.responses = np.loadtxt(responsesDataPath, np.float32)
        
        self.model = cv2.ml.KNearest_create()
        self.model.train(self.samples, cv2.ml.ROW_SAMPLE, self.responses)
        
        # 윤곽선 추출
        self.cnts = cv2.findContours(self.thresh.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        self.cnts = self.cnts[0] if imutils.is_cv2() else self.cnts[1]
        
        # findContours의 결과는 우하단에서 부터 시작되기에 좌상단 시작으로 정렬
        self.cnts.sort(key=lambda x: self.get_contour_precedence(x, self.thresh.shape[1]))    
        
    def getSudokuString(self):        
        # 스도쿠 숫자 변수 초기화
        strNumbers = '0' * 81
        
        # for test
#         tmpStr = ""     
        
        for c in self.cnts:
            if cv2.contourArea(c) > 100:    # 작은 윤곽선 걸러내기
                [x,y,w,h] = cv2.boundingRect(c)
                
                if  45 < h < 50:    # 사용된 이미지에서 숫자 크기: 48
                    cv2.rectangle(self.im, (x,y), (x+w,y+h), (0,255,0), 2)
                    roi = self.thresh[y:y+h, x:x+w]
                    roismall = cv2.resize(roi, (10, 10))
                    roismall = roismall.reshape((1, 100))
                    roismall = np.float32(roismall)
                    [retval, results, neigh_resp, dists] = self.model.findNearest(roismall, k = 1)
                    
                    num = str(int((results[0][0])))
                    # for test
#                     tmpStr += num
                    
                    row = x // 80   # x, y좌표를 이용하여 셀의 행열화(셀 크기: 80)
                    col = y // 80
                    index = row + col * 9 
                    
                    # python에서 string은 index로 수정할 수 없기에 아래와 같이 함수를 사용해서 해당 인덱스의 문자를 변경
                    replaceCharInStr = lambda str, index, char: str[:index] + char + str[index + 1:]
                    strNumbers = replaceCharInStr(strNumbers, index, num)
#                     print("index(%d)-num(%s): %s" % (index, num, strNumbers))
        
#         print(tmpStr)
#         print(strNumbers)
        return strNumbers
                    
    # 좌상단부터 우하단까지 순위로 정렬하는 함수
    # 출처: https://stackoverflow.com/questions/39403183/python-opencv-sorting-contours                    
    def get_contour_precedence(self, contour, cols):
        tolerance_factor = 48   # 숫자 크기
        origin = cv2.boundingRect(contour)
        return ((origin[1] // tolerance_factor) * tolerance_factor) * cols + origin[0]
    
    
    
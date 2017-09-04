# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 2.

@author: crims
'''

import numpy as np
import cv2
import imutils

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
        
    def getSudokuString(self):
        # 윤곽선 추출
        cnts = cv2.findContours(self.thresh.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if imutils.is_cv2() else cnts[1]
        cnts.sort(key=lambda x: self.get_contour_precedence(x, self.thresh.shape[1]))    
        
        # 스도쿠 숫자 변수 초기화
        strNumbers = '0' * 81
        
        # for test
        tmpStr = ""     
        
        for c in cnts:
            if cv2.contourArea(c) > 100:
                [x,y,w,h] = cv2.boundingRect(c)
                if  45 < h < 50:
                    cv2.rectangle(self.im, (x,y), (x+w,y+h), (0,255,0), 2)
                    roi = self.thresh[y:y+h, x:x+w]
                    roismall = cv2.resize(roi, (10, 10))
                    roismall = roismall.reshape((1, 100))
                    roismall = np.float32(roismall)
                    [retval, results, neigh_resp, dists] = self.model.findNearest(roismall, k = 1)
                    
                    num = str(int((results[0][0])))
                    # for test
                    tmpStr += num
                    
                    row = x // 80
                    col = y // 80
                    index = row + col * 9
                    replaceCharInStr = lambda index, char, str: str[:index] + char + str[index + 1:]
                    strNumbers = replaceCharInStr(index, num, strNumbers)
#                     print("index(%d)-num(%s): %s" % (index, num, strNumbers))
        
#         print(tmpStr)
#         print(strNumbers)
        return strNumbers
                    
    def get_contour_precedence(self, contour, cols):
        tolerance_factor = 48   # 숫자 크기
        origin = cv2.boundingRect(contour)
        return ((origin[1] // tolerance_factor) * tolerance_factor) * cols + origin[0]
    
    
    
# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 1.

@author: crims
'''

import sys

import numpy as np
import cv2
import imutils

import TrimImage


class DigitLearning():
    def __init__(self, imgData):
        # 이미지 데이터 변수 초기화
        self.im = imgData

        # 이미지 프로세싱을 위해 필터 사용
        im_gray = cv2.cvtColor(self.im, cv2.COLOR_BGR2GRAY)
        im_gray_blurred = cv2.GaussianBlur(im_gray, (5, 5), 0)
        self.thresh = cv2.adaptiveThreshold(im_gray_blurred, 255, 1, 1, 11, 2)
    
    def saveLearningData(self):
        # 윤곽선 추출
        cnts = cv2.findContours(self.thresh.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if imutils.is_cv2() else cnts[1]
        cnts.sort(key=lambda x: self.get_contour_precedence(x, self.thresh.shape[1]))    
        
        samples =  np.empty((0,100))
        responses = []
        keys = [i for i in range(48, 58)]
        
        for c in cnts:
            if cv2.contourArea(c) > 100:
                [x,y,w,h] = cv2.boundingRect(c)
        
                if 45 < h < 50:
                    cv2.rectangle(self.im, (x,y), (x+w,y+h), (0,0,255), 2)
                    roi = self.thresh[y:y+h,x:x+w]
                    roismall = cv2.resize(roi,(10,10))
                    cv2.imshow("Learning", self.im)
                    key = cv2.waitKey(0)
        
                    if key == 27:  # ESC누르면 종료
                        sys.exit()
                    elif key in keys:
                        print(chr(key))
                        responses.append(int(chr(key)))
                        sample = roismall.reshape((1, 100))
                        samples = np.append(samples, sample, 0)
        
        responses = np.array(responses,np.float32)
        responses = responses.reshape((responses.size, 1))
        print("training complete")
        
        np.savetxt('data/DigitSamples.data', samples)
        np.savetxt('data/DigitResponses.data', responses)
        
    def get_contour_precedence(self, contour, cols):
        tolerance_factor = 48   # 숫자 크기
        origin = cv2.boundingRect(contour)
        return ((origin[1] // tolerance_factor) * tolerance_factor) * cols + origin[0]


if __name__ == '__main__':
    imgList = []

    for i in range(1, 31):
        imgList.append("pictures/IMG_%02d.png" % i) 
    
    for s in imgList:
        print(s)
        _TrimImage = TrimImage.TrimImage(s)
        im = _TrimImage.extract()
        _DigitLearning = DigitLearning(im)
        _DigitLearning.saveLearningData()

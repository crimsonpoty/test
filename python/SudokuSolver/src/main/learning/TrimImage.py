# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 1.

@author: crims
'''

import sys

import numpy as np
import cv2
import imutils


# 윤곽 모양 구분 클래스 (출처: http://sosal.kr/1067 [so_sal])
class ShapeDetector:    
    def __init__(self):
        pass
    
    def detect(self, c):
        # initialize the shape name and approximate the contour
        shape = "unidentified"
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.04 * peri, True)
        
        # if the shape is a triangle, it will have 3 vertices
        if len(approx) == 3:
            shape = "triangle"

        # if the shape has 4 vertices, it is either a square or a rectangle
        elif len(approx) == 4:
            # compute the bounding box of the contour and use the bounding box to compute the aspect ratio
            (x, y, w, h) = cv2.boundingRect(approx)
            ar = w / float(h)
        
            # a square will have an aspect ratio that is approximately equal to one, otherwise, the shape is a rectangle
            shape = "square" if ar >= 0.95 and ar <= 1.05 else "rectangle"
        
        # if the shape is a pentagon, it will have 5 vertices
        elif len(approx) == 5:
            shape = "pentagon"
        
        # otherwise, we assume the shape is a circle
        else:
            shape = "circle"
        
        # return the name of the shape
        return shape
    
    
class TrimImage():
    def __init__(self, imagePath):
        # 이미지 경로 변수 초기화
        self.imagePath = imagePath

        # 이미지 불러오기
        self.im = cv2.imread(imagePath)
        
        # 이미지 프로세싱을 위해 필터 사용
        im_gray = cv2.cvtColor(self.im, cv2.COLOR_BGR2GRAY)
        im_gray_blurred = cv2.GaussianBlur(im_gray, (5, 5), 0)
        self.thresh = cv2.adaptiveThreshold(im_gray_blurred, 255, 1, 1, 11, 2)
        
    def extract(self, bFilteredImage = False, bOutput = False):
        # findContours로 윤곽선 추출
        cnts = cv2.findContours(self.thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if imutils.is_cv2() else cnts[1]
        
        sd = ShapeDetector()

        # 추출된 윤곽선 표시
        for c in cnts:
            shape = sd.detect(c)
            
            if not (shape == 'square' or shape == 'rectangle'):
                continue
            
            c = c.astype("int")
            [x,y,w,h] = cv2.boundingRect(c)
            
            if 730 < w < 735 and 730 < h < 735:     # 숫자가 있는 9x9의 정사각형의 크기: 732
                if bFilteredImage:
                    retIm = self.thresh[y:y+h,x:x+w]
                else:
                    retIm = self.im[y:y+h,x:x+w]
                     
                if bOutput: cv2.imwrite(self.imagePath.replace("IMG", "trimed_images/Trimed_IMG"), retIm)
                    
                return retIm


if __name__ == '__main__':
    pass


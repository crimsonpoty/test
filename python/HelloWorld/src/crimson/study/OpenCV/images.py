# -*- coding:UTF-8 -*-

'''
Created on 2017. 8. 29.

@author: crims
'''

import numpy as np
import cv2  # @UnresolvedImport

print("numpy_version: " + np.__version__)
print("opencv_version: " + cv2.__version__)

img1Path = "pictures/mountain.jpg"
img2Path = "pictures/koala.jpg"
img3Path = "pictures/seeside.jpg"

# 이미지 읽기
img1 = cv2.imread(img1Path, cv2.IMREAD_UNCHANGED)
img2 = cv2.imread(img2Path, cv2.IMREAD_UNCHANGED)
img3 = cv2.imread(img3Path, cv2.IMREAD_UNCHANGED)

# 이미지 윈도우 이름, 속성 설정
cv2.namedWindow('image1', cv2.WINDOW_NORMAL)
cv2.namedWindow('image2', cv2.WINDOW_AUTOSIZE)
cv2.namedWindow('image3', cv2.WINDOW_AUTOSIZE)

# 이미지 윈도우 띄우기
cv2.imshow('image1', img1)
cv2.imshow('image2', img2)
cv2.imshow('image3', img3)

cv2.waitKey(0)      # 키보드 입력 대기
cv2.destroyAllWindows()     # 모든 윈도우 창 닫기



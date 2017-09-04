# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 1.

@author: crims
'''

import sys

import numpy as np
import cv2
import imutils

import ShapeDetector

# 이미지 불러오기
im = cv2.imread("pictures/IMG_2428.png")

# 이미지 프로세싱을 위한 필터 사용
im_gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
im_gray_blurred = cv2.GaussianBlur(im_gray, (5, 5), 0)
thresh = cv2.adaptiveThreshold(im_gray_blurred,255,1,1,11,2)

# 필터의 선택적 적용을 위한 테스트
ret,thresh1 = cv2.threshold(im_gray_blurred,127,255,cv2.THRESH_BINARY)
ret,thresh2 = cv2.threshold(im_gray_blurred,127,255,cv2.THRESH_BINARY_INV)
ret,thresh3 = cv2.threshold(im_gray_blurred,127,255,cv2.THRESH_TRUNC)
ret,thresh4 = cv2.threshold(im_gray_blurred,127,255,cv2.THRESH_TOZERO)
ret,thresh5 = cv2.threshold(im_gray_blurred,127,255,cv2.THRESH_TOZERO_INV)

# 이미지 비교를 위해 필터 적용된 이미지 합치기
images_row1 = np.hstack((im_gray_blurred, thresh1, thresh2))   # 수평으로 이미지 튜플 합치기
images_row2 = np.hstack((thresh3, thresh4, thresh5))
images_combined = np.vstack((images_row1, images_row2))     # 수직으로 이미지 튜플 합치기


# findContours로 윤곽선 추출
cnt = cv2.findContours(thresh2.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cnt2 = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
# print(len(cnt))
cnt = cnt[0] if imutils.is_cv2() else cnt[1]
cnt2 = cnt2[0] if imutils.is_cv2() else cnt2[1]
# print(len(cnt))


sd = ShapeDetector.ShapeDetector()

# 추출된 윤곽선 표시 및 출력
for c in cnt:
    shape = sd.detect(c)
    
    if not (shape == 'square' or shape == 'rectangle'):
        continue
    
#     c = c.astype("float")
    c = c.astype("int")
    x,y,w,h = cv2.boundingRect(c)
    
    if not (730 < w < 735 and 730 < h < 735):
        continue
    
#     cv2.rectangle(im,(x,y),(x+w,y+h),(3,255,4),2)
#     print("w(%d), h(%d)" % (w, h))
#     cv2.imshow("contours image", im)

    cv2.imwrite("trimed_img.png", im[y:y+h,x:x+w])
    
#     if cv2.waitKey(0) == 27:    # ESC 누르면 종료
#         break


# 추출된 윤곽선 표시
for c in cnt2:
    shape = sd.detect(c)
    
    if not (shape == 'square' or shape == 'rectangle'):
        continue
    
    c = c.astype("int")
    x,y,w,h = cv2.boundingRect(c)
    
    if not (730 < w < 735 and 730 < h < 735):
        continue
    
    cv2.imwrite("trimed_img2.png", im[y:y+h,x:x+w])


# 이미지 보기
# cv2.imshow("image", im)
# cv2.imshow("image", im_gray)
# cv2.imshow("image", im_gray_blurred)
# cv2.namedWindow('combined image', cv2.WINDOW_NORMAL)        # 이미지 사이즈가 커서 윈도우 크기 조정을 위해 윈도우 속성 설정
# cv2.imshow("combined image", images_combined)
# cv2.imshow("contours image", thresh2)

# 키보드 입력 대기
# cv2.waitKey(0)


if __name__ == '__main__':
    pass
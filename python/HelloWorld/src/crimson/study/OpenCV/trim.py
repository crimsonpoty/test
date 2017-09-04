# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 1.

@author: crims
'''

import cv2

def imgTrim(img, x, y, w, h):
    trimedImg = img[y:y+h, x:x+w]
    return trimedImg


if __name__ == '__main__':
    im = cv2.imread("pictures/IMG_2428.png")
    trimedImg = imgTrim(im, 9, 193, 732, 732)
    cv2.imshow('trimed image', trimedImg)
    cv2.waitKey(0)
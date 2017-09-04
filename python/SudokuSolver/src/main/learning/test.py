# -*- coding: UTF-8 -*-

'''
Created on 2017. 9. 1.

@author: crims
'''

import numpy as np
import cv2
import imutils

import TrimImage
import DigitRecognition


### TrimImage test ###

# imgList = []
# 
# for i in range(1, 31):
#     imgList.append("pictures/IMG_%02d.png" % i) 
#     
# for s in imgList:
#     _TrimImage = TrimImage.TrimImage(s)
#     ret = _TrimImage.extract(False, True)
#     print(type(ret))
     
     
### DigitRecognition test ###

imgPath = "pictures/IMG_01.png"
samplesPath = "data/DigitSamples.data"
responsesPath = "data/DigitResponses.data"

_t = TrimImage.TrimImage(imgPath)
im = _t.extract()

_d = DigitRecognition.DigitRecognition(im, samplesPath, responsesPath)
print(_d.getSudokuString())
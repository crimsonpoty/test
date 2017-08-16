'''
Created on 2017. 8. 16.

@author: crims
'''

import webbrowser, time

if __name__ == '__main__':
    webbrowser.open_new("https:google.com")
    time.sleep(1)
    webbrowser.open_new_tab("http://www.naver.com")
    time.sleep(1)
    webbrowser.open("http://www.daum.net")
    
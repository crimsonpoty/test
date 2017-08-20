# -*- encoding: UTF-8 -*-

'''
Created on 2017. 8. 20.

@author: crims
'''
from tkinter import Tk, Label, Entry, Button, _exit
from distutils import command


root = Tk()

lbl = Label(root, text="이름")
# lbl.pack()  # 위젯의 부모 위젯에 패킹하여 불필요한 공간 없애기
lbl.grid(row=0, column=0)   # row, column을 사용하여 위치 지정

txt = Entry(root)
# txt.pack()
txt.grid(row=0, column=1)

# 버튼 클릭 이벤트 핸들러
def okClicked():
    _exit()

# btn = Button(root, text="OK")
# btn.pack()
btn = Button(root, text="OK", width=15, command=okClicked)
btn.grid(row=1, column=1)

root.mainloop()

if __name__ == '__main__':
    pass
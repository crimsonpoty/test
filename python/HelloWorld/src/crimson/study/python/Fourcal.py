# -*- coding: UTF-8 -*-
'''
Created on 2017. 8. 15.

@author: crims
'''

# make Fourcal using class

class Fourcal:
    def __init__(self, num1, num2):
        self.num1 = num1
        self.num2 = num2
    def sum(self):
        return self.num1 + self.num2
    def sub(self):
        return self.num1 - self.num2
    def mul(self):
        return self.num1 * self.num2
    def div(self):
        return self.num1 / self.num2
    

# 객체 호출
a = Fourcal(4, 2)
# if __name__ == "__main__":
#     print(a.sum())
#     print(a.sub())
#     print(a.mul())
#     print(a.div())
#     print()

print(a.sum())
print(a.sub())
print(a.mul())
print(a.div())
print()


# 클래스 호출
b = Fourcal(1, 2)
# if __name__ == "__main__":
#     print(Fourcal.sum(b))
#     print(Fourcal.sub(b))
#     print(Fourcal.mul(b))
#     print(Fourcal.div(b))
     
print(Fourcal.sum(b))
print(Fourcal.sub(b))
print(Fourcal.mul(b))
print(Fourcal.div(b))

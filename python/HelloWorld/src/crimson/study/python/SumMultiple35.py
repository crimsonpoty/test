'''
Created on 2017. 8. 16.

@author: crims
'''

def SumMultiple(num1, num2):
    list3 = []
    list5 = []

    for i in range(1, 1000):
        a = i * num1
        if a < 1000:
            list3.append(a)
        else:
            break

    for i in range(1, 1000):
        a = i * num2
        if a < 1000:
            list5.append(a)
        else:
            break
        
#     print(list3)
#     print(list5)
    
    set3 = set(list3)
    set5 = set(list5)
    resultSet = set3 | set5
    
#     print(resultSet)
    
    sum = 0
    for i in resultSet:
        sum += i
        
    return sum

if __name__ == '__main__':
    result = SumMultiple(3, 5)
    print("Result: %d" % result)
    
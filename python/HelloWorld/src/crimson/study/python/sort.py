def getKey0(item):
    return item[0]

def getKey1(item):
    return item[1]

a = [(2, 3), (6, 7), (3, 34), (24, 64), (1, 43)]

a = sorted(a, key=getKey1)
b = sorted(a, key=lambda item: item[1])
print(a)
print(a)

# -*- coding: UTF-8 -*-

# write
f = open("newText.txt", 'w')

for i in range(1, 11):
    data = "%d번째 줄입니다.\n" % i
    f.write(data)

f.close()


# append
f = open("newText.txt", 'a')

for i in range(11, 20):
    data = "%d번째 줄입니다.\n" % i
    f.write(data)
    
f.close()


# read
f = open("newText.txt", 'r')

# print("f.readline()")
# while True:
#     line = f.readline()
#     if not line: break
#     print(line)
    
# print("f.readlines()")
# lines = f.readlines()
# print(lines)
# for x in lines:
#     print(x)
    
print("f.read()")
lines = f.read()
print(lines)

f.close()


# with
with open("newText.txt", 'w') as f:
    f.write("This is study")

with open("newText.txt", 'r') as f:
    data = f.read()
    print(data)
    
    
    
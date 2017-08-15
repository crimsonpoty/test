'''
Created on 2017. 8. 7.

@author: crims
'''


# test for Declaration

a, b = 'python', 'life'
(c, d) = ('python', 'life')

print(a)
print(b)
print(c)
print(d)


#test for swap

c, d = d, c
print(c)
print(d)


#test for copy
from copy import copy
a = [1, 2, 3]
b = copy(a)
print(a is b)
print(a)
print(b)

c = a[:]
print(a == c)

exit()

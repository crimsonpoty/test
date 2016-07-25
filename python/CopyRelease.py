# hello.py

# input_file = raw_input("File name: ")
# f = open(input_file, 'r')
# f = open("Config.mk", 'r')

# lines = f.readlines()
# print(lines)

# lines = filter(lambda e: e is "export" and len(e) > 0, lines)
# print(lines)
# f.close()

# import re
# a = ["export", "aaa", "bbb", "export ", ""]
# b = []
# for i in a
	# b.append(re.findall(r"export\w*", a))
# b = re.findall(r"export\w*", "export lajsdklf exportskdjfl")
# print(b)

import os
libraries = os.system("cd /home/k150030/project/svn/_temp/BuildTestFinal/benjamin;make print_ldflags")

from time import time


def test_loop(count):
	start_time = time()
	
	for i in range(10 ** count): pass
	
	return time() - start_time


if __name__ == '__main__':
	for i in range(10):
		print("10-e{:02d}: {}".format(i, test_loop(i)))
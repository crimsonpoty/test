'''
Created on 2017. 10. 26.

 For test nfs

@author: ywkim
'''


REMOTE_PATH = 'NFS_TEST'
LOCAL_PATH = '/Users/imedsyn/tmp'
# LOCAL_PATH = '/home/ywkim/nfs/tmp'
FILE_NAME = 'test.ied'

class CCp:
	def __init__(self, local_file):
		try:
			f = open(local_file, 'rb')
		except FileNotFoundError as e:
			print(str(e))
		else:
			self.data = f.read()
			f.close()

		
	def __exit__(self):
		pass		
	
	def fileSave(self, remote_path):
		try:
			f = open(remote_path, 'wb')
		except FileNotFoundError as e:
			print(str(e))
		else:
			f.write(self.data)
			f.close()


if __name__ == '__main__':
	
	import time
	start_time = time.time()
    
	_c = CCp(LOCAL_PATH + '/' + FILE_NAME)
	_c.fileSave(LOCAL_PATH + '/' + REMOTE_PATH + '/' + FILE_NAME)
	
	diff_time = time.time() - start_time
	print(diff_time)

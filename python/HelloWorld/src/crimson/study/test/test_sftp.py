'''
Created on 2017. 10. 25.

 For test sftp

@author: ywkim
'''

import pysftp
import time

HOST_NAME = '172.16.3.36'
USER_NAME = 'ywkim'
PASSWORD = 'gksQldi1!'
REMOTE_PATH = 'NFS_TEST'
LOCAL_PATH = '/Users/imedsyn/tmp'
# LOCAL_PATH = '/home/ywkim/nfs/tmp'
FILE_NAME = 'test.ied'


class CSftp:
	def __init__(self):
		# set host, user, passwd
		self.hostname = '172.16.3.36'
		self.username = 'ywkim'
		self.password = 'gksQldi1!'
		self.ret = False
# 		self.hostname = HOST_NAME
# 		self.username = USER_NAME
# 		self.password = PASSWORD
	
	def __exit__(self):
# 		self.sftp.close()
		pass
	
	def fileSave(self, local_file, remote_path):
		# connect to sftp server
		try:
			self.sftp = pysftp.Connection(self.hostname, username=self.username, password=self.password)
		except:
			print("[SFTP]: Can not connect")
		else:
			try:
				with self.sftp.cd(remote_path):
					self.ret = self.sftp.put(local_file)
			except:
				print("[SFTP]: put file is fail")
			else:
				self.sftp.close()
				
		return self.ret
	
        
        
if __name__ == '__main__':
    # direct transfer
#	with pysftp.Connection(HOST_NAME, username=USER_NAME, password=PASSWORD) as sftp:
#		with sftp.cd(REMOTE_PATH):             # temporarily chdir to public
#			sftp.put(LOCAL_PATH + '/' + FILE_NAME)  # upload file to public/ on remote

	start_time = time.time()
	
	# transfer with CSftp
	_sftp = CSftp()
	if _sftp.fileSave(LOCAL_PATH + '/' + FILE_NAME, REMOTE_PATH):
		diff_time = time.time() - start_time
		print(diff_time)
	
	
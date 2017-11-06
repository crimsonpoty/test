'''
Created on 2017. 10. 26.

 For test sftp_paramiko

@author: ywkim
'''

from paramiko import Transport, SFTPClient
import tempfile
import time

HOST_NAME = '172.16.3.36'
USER_NAME = 'ywkim'
PASSWORD = 'gksQldi1!'
REMOTE_PATH = 'NFS_TEST'
LOCAL_PATH = '/Users/imedsyn/tmp'
# LOCAL_PATH = '/home/ywkim/nfs/tmp'
FILE_NAME = 'test.ied'


class CSftpParamiko:
	def __init__(self):
		# set host, user, passwd
		self.hostname = '172.16.3.36'
		self.username = 'ywkim'
		self.password = 'gksQldi1!'
# 		self.hostname = HOST_NAME
# 		self.username = USER_NAME
# 		self.password = PASSWORD
        
	def __exit__(self):
		pass
        
	def fileSave(self, local_file, remote_path):
		# connect to sftp server
		try:
			self.transport = Transport((self.hostname, 22))
			self.sftp = self.transport.connect(username=self.username, password=self.password)
			self.sftp_client = SFTPClient.from_transport(self.transport)
		except:
			print("[SFTP_P]: connect fail")
			
		try:
			self.sftp_client.put(local_file, remote_path) 
		except:
			print("[SFTP_P]: put fail")

		try:
			self.sftp_client.close()
			self.transport.close()
		except:
			print("[SFTP_P]: close fail")
		
		
		
if __name__ == '__main__':
	start_time = time.time()
	
	_sftp = CSftpParamiko()
	_sftp.fileSave(LOCAL_PATH + '/' + FILE_NAME, REMOTE_PATH + '/' + FILE_NAME)
	
	diff_time = time.time() - start_time
	print(diff_time)
         

    
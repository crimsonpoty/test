'''
Created on 2017. 10. 27.

 For test smb

@author: ywkim
'''

from smb.SMBConnection import SMBConnection
import tempfile
import time

USER_NAME = 'ywkim'
PASSWORD = 'gksQldi1!'
HOST_IP = '172.16.3.36'
PORT = 445
REMOTE_PATH = 'NFS_TEST'
LOCAL_PATH = '/Users/imedsyn/tmp'
# LOCAL_PATH = '/home/ywkim/nfs/tmp'
FILE_NAME = 'test.ied'

class CSmb:
	def __init__(self):
		pass
		
	def __exit__(self):
		pass
		
	def fileSave(self, remote_path, local_file):
		try:
			self.conn = SMBConnection('ywkim', 'gksQldi1!', 'dev', 'nas5', domain='', use_ntlm_v2=True)
			self.conn.connect('172.16.3.36', 445)
# 			self.conn = SMBConnection(USER_NAME, PASSWORD, 'dev', 'nas5', domain='', use_ntlm_v2=True)
# 			self.conn.connect(HOST_IP, PORT)
		except:
			print("[SMB]: Can not connect to NAS Server")
		else:
			try:
				f = open(local_file, 'rb')
				f.seek(0)
				self.conn.storeFile(remote_path, FILE_NAME, f)
			except:
				print("[SMB]: Can not transfer file to NAS Server")
				
			try:
				self.conn.close()
			except:
				print("[SMB]: Can not close connection")
			
		
		
if __name__ == '__main__':
	start_time = time.time()
	_smb = CSmb()
	_smb.fileSave(REMOTE_PATH, LOCAL_PATH + '/' + FILE_NAME)
	diff_time = time.time() - start_time
	print(diff_time)
	
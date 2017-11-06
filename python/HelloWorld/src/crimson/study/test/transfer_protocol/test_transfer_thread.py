'''
Created on 2017. 10. 27.

	For test transfer protocol using thread
	
@author: ywkim
'''

import threading
import time
import test_smb, test_nfs, test_sftp, test_sftp_paramiko
import daemon


running_time = [[] for i in range(4)]
is_done = [[False for i in range(3)] for i in range(4)]

def running(protocol_index, thread_index, func, src_file, dest_file, sleep_time = 1, max_count = 100):
	global running_time
	count = 0
	while count < max_count:
		time.sleep(sleep_time)
		print("[Running] thread: %d, protocol: %d, sleep_time: %.1f, count: %d" % (thread_index, protocol_index, sleep_time, count))
		test_protocol(func, src_file, dest_file, running_time[protocol_index])
		count += 1
		
	is_done[protocol_index][thread_index] = True

def test_protocol(func, src_file, dest_file, time_list):
	start_time = time.time()
	func(src_file, dest_file)
	time_list.append(time.time() - start_time)

def run_smb(src_file, dest_file):
	_smb = test_smb.CSmb()
	_smb.fileSave(dest_file, src_file)
        
def run_nfs(src_file, dest_file):
	_nfs = test_nfs.CCp(src_file)
	_nfs.fileSave(dest_file)
        
def run_sftp(src_file, dest_file):
	_sftp = test_sftp.CSftp()
	_sftp.fileSave(src_file, dest_file)
        
def run_sftp_p(src_file, dest_file):
	_sftp_p = test_sftp_paramiko.CSftpParamiko()
	_sftp_p.fileSave(src_file, dest_file)
        
def floatMean(list):
	ret = False
	
	try:
		ret = sum(list) / float(len(list))
	except ZeroDivisionError as e:
		print(str(e))
		
	return ret


if __name__ == "__main__":
	remote_path = 'NFS_TEST'
# 	local_path = '/Users/imedsyn/tmp'
	local_path = '/home/ywkim/nfs/tmp'
	info = [[ "test%d.ied" % i for i in range(1, 4)], [5, 10, 20], 1]	# [file_name, sleep_time, max_count]
	th0 = [None for i in range(3)]
	th1 = [None for i in range(3)]
	th2 = [None for i in range(3)]
	th3 = [None for i in range(3)]

	
	for i in range(3):
		try:
			th0[i] = threading.Thread( \
				target=running, \
				args=(0, i, run_smb, local_path + '/' + info[0][i], remote_path, info[1][i], info[2],), \
				daemon=True) 
			th1[i] = threading.Thread( \
				target=running, \
				args=(1, i, run_nfs, local_path + '/' + info[0][i], '/mnt/nas/nfs/' + info[0][i], info[1][i], info[2],), \
				daemon=True) 
			th2[i] = threading.Thread( \
				target=running, \
				args=(2, i, run_sftp, local_path + '/' + info[0][i], remote_path + '/sftp', info[1][i], info[2],), \
				daemon=True) 
			th3[i] = threading.Thread( \
				target=running, \
				args=(3, i, run_sftp_p, local_path + '/' + info[0][i], remote_path + '/sftp_p/' + info[0][i], info[1][i], info[2],), \
				daemon=True) 
			
			th0[i].start()
			th1[i].start()
			th2[i].start()
			th3[i].start()
		except Exception as e:
			print(str(e))

	for i in range(3):	
		th0[i].join()
		th1[i].join()
		th2[i].join()
		th3[i].join()
		
	while True:
		time.sleep(1)
		if not False in is_done[0]: break
		if not False in is_done[1]: break
		if not False in is_done[2]: break
		if not False in is_done[3]: break
	
	for i in range(4):
		print(running_time[i])
		print("protocol: %d, Average time: %.2f" % (i, floatMean(running_time[i])))
	
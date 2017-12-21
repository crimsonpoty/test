import sys
import os
from math import sqrt, log10
from time import time

import matplotlib
matplotlib.use("TkAgg")  # for prevent crash matplotlib and tkinter
import matplotlib.pyplot as plt
import numpy as np
import pyfftw as fftw
from tkinter import filedialog


# add paths
root_path = "/home/ywkim/nfs/workspace/isyncbrain_denoise/src"
sys.path.append("{}/eeglab/popfunc".format(root_path))
sys.path.append("{}/eeglab/adminfunc".format(root_path))
sys.path.append("{}/eeglab/sigprofunc".format(root_path))
sys.path.append("{}/eeglab/firfilt".format(root_path))
sys.path.append("{}/eeglab/cleanrawdata".format(root_path))
sys.path.append("{}/eeglab/ADJUST".format(root_path))
sys.path.append("{}/eeglab/fieldtrip".format(root_path))
sys.path.append("{}/eeglab/dipfit".format(root_path))
sys.path.append("{}/brain_net_viewer".format(root_path))
sys.path.append("{}/interface".format(root_path))
sys.path.append("{}/matlab".format(root_path))
sys.path.append("{}/matlab/optim".format(root_path))
sys.path.append("{}/matlab/signal".format(root_path))
sys.path.append("{}/sensor_level_analysis".format(root_path))
sys.path.append("{}/source_level_analysis".format(root_path))
sys.path.append("{}/utils".format(root_path))
sys.path.append("{}/structure".format(root_path))

from isb_spectopo import spectopo


## CONSTANT
IS_PRINT = False
IS_OUTPUT = True




## SNR

# calculate root mean square
def rms(array):
	
	ret = 0.0
	
	if isinstance(array, (np.ndarray, np.generic)):
		square_array = np.absolute(array) ** 2
		ret = sqrt(square_array.sum() / square_array.size)
	else:
		square_array = [abs(n) ** 2 for n in array]
		ret = sqrt(sum(square_array) / len(square_array)) 
		
	return ret


# calculate snr, must be same length raw_data and cleaned_data
def SNR(raw_data, cleaned_data):

	if not isinstance(raw_data, (np.ndarray, np.generic)):
		raw_data = np.array(raw_data)
	if not isinstance(cleaned_data, (np.ndarray, np.generic)):
		cleaned_data = np.array(cleaned_data)
		
	rms_raw = rms(raw_data)
	
	if len(raw_data) is len(cleaned_data):
		# difference raw and cleaned data with time sync
		noise_data = raw_data - cleaned_data
		rms_noise = rms(noise_data)
	else:
		rms_cleaned = rms(cleaned_data) 
		rms_noise = rms_raw - rms_cleaned
		
	if rms_noise < 0.0:
		_snr = -1	# wrong input data 
	else:
		_snr = 20 * log10(rms_raw / rms_noise)
	
	return _snr


## translate to freq domain using fft
def find_nearest(array, value):
	
	idx = (np.abs(array-value)).argmin()
	return idx 


# will be deprecate because using spectopo
def fft(array, FS=250, freq_limit=30):
	
	# y axis in time domain
	if isinstance(array, (np.ndarray, np.generic)):
		y = array
	else:
		y = np.array(array)
	
	# length of the signal
	n = array.size
		
	# x axis in freq. domain
	k = np.arange(n)
	tmp_x_fft = (k * FS) / n
	x_fft = tmp_x_fft[range(int(n/2))]  # one side freq. range
	idx = find_nearest(x_fft, freq_limit)
	
	# y axis in freq. domain
	
	# normalization
	NORMALIZATION = n # 100
	
	# fft with numpy
# 	tmp_y_fft = np.fft.fft(y) / NORMALIZATION  # fft computing and normalization 
	
	# fft with pyfftw
	tmp_y_fftw = fftw.builders.fft(y, axis=0, planner_effort="FFTW_MEASURE")
	tmp_y_fft = tmp_y_fftw() / NORMALIZATION
	
	# for scale
# 	tmp_y_fft = tmp_y_fft * 1000
	
	tmp_y_fft = tmp_y_fft[range(int(n/2))]  # one side freq. range
	
	# freq. limit
	x_fft = tmp_x_fft[range(idx)]
	y_fft = tmp_y_fft[range(idx)]
	del tmp_x_fft, tmp_y_fft
	
	return x_fft, y_fft


def cut_side(array, FS=250, secs=5): 
	
	cut_size = int(FS*secs)
	
	return array[cut_size-1:len(array)-cut_size]


def arrange_matrix(array, reverse=False):
	
	if not reverse:
		if array.shape[0] is not 19:
			return np.transpose(array)
		else:
			return array
	else:
		if array.shape[1] is not 19:
			return np.transpose(array)
		else:
			return array


def find_last_slash(str):
	
	if str[-1] is '/':
		tmp_str = str[:-1]
	else:
		tmp_str = str
		
	ret_idx = 0
	
	while tmp_str:
		tmp_idx = tmp_str.find('/')
		
		if tmp_idx >= 0:
			ret_idx += tmp_idx + 1  
			tmp_str = tmp_str[tmp_idx+1:]
		else:
			break
		
	return ret_idx
	
	
def draw_3_plot(x_list, y_list, x_limit, y_limit, is_show=False, fig_size=(12, 15), is_save=False, fig_name='fft.png'):
	
	# arg validation
	if not x_list or not y_list:
		return
	
# 	if not len(x_list) == 3 or not len(y_list) == 3:
# 		return
	
	fig = plt.figure(figsize=fig_size)
	
	for i in range(len(x_list)):
		ax = fig.add_subplot(len(x_list), 1, i+1)
		ax.set_xlim(0, x_limit[i])
		ax.set_ylim(0, y_limit[i])
		ax.margins(0, 1)
		ax.set_title(str(i), fontsize=16, fontweight="bold")
		ax.set_xlabel('Freq. (Hz)')
		ax.set_ylabel('power (uV)')
		ax.grid()
		ax.plot(x_list[i], abs(y_list[i]))
		
	if is_save:
		fig.savefig(fig_name)
	
	if is_show:
		plt.show()
		
		
# return reshaped ndarray. for example, (5, 18)
# it is just ways for 2, 3 columns. if you need another columns, do customization
def reshape_for_anova(array, column=3):
	
	if column is 3:
		ret_list = [[] for i in range(column)]
		
		for i in range(int(array.shape[1]/column)):
		
			idx_x = i * column
			tmp_matrix = [[] for i in range(column)]
			
			for j in range(array.shape[0]):
				tmp_list = [[] for i in range(column)]
				
				# 3 columns
				tmp_list[0].append(array[j][idx_x+0])
				tmp_list[0].append(array[j][idx_x+1])
				tmp_list[0].append(array[j][idx_x+2])
				
				# 2 columns(0,1)
				tmp_list[1].append(array[j][idx_x+0])
				tmp_list[1].append(array[j][idx_x+1])
				
				# 2 columns(0,2)
				tmp_list[2].append(array[j][idx_x+0])
				tmp_list[2].append(array[j][idx_x+2])
					
				tmp_matrix[0].append(tmp_list[0])
				tmp_matrix[1].append(tmp_list[1])
				tmp_matrix[2].append(tmp_list[2])
			
			ret_list[0].append(tmp_matrix[0])
			ret_list[1].append(tmp_matrix[1])
			ret_list[2].append(tmp_matrix[2])

		return np.array(ret_list[0]), np.array(ret_list[1]), np.array(ret_list[2])
	
		
	elif column is 2:
		ret_list = []
		
		for i in range(int(array.shape[1]/column)):
			idx_x = i * column
			tmp_matrix = []
				
			for j in range(array.shape[0]):
				tmp_list = []
				
				tmp_list.append(array[j][idx_x+0])
				tmp_list.append(array[j][idx_x+1])
					
				tmp_matrix.append(tmp_list)
				
			ret_list.append(tmp_matrix)
		
		return np.array(ret_list)
		
	else:
		return np.array([])
	

def reshape_with_icc(array, row=29, col=3):
	
	num_ch = 19
	
	if (row is 29 or row is 5) and col is 3:
		tmp_array = [[] for i in range(row)]
		
		for i in range(row):
			for j in range(num_ch):
				for k in range(col):
					tmp_array[i].append(array[i+j*row][k])
					
		return np.array(tmp_array)
		
	elif (row is 1) and col is 3:
		return np.array(array)
	else:
		return np.array([])

# calculate icc(2,1) using anova
def icc(array):
	
	# check 2-dimension
	if array.ndim is not 2:
		return -1
	
	# means
	total_mean = array.mean()
	row_mean = array.mean(axis=1)
	col_mean = array.mean(axis=0)
	
	# sum of square
	ssb, sse = 0, 0
	
	for i in range(array.shape[0]):
		for j in range(array.shape[1]):
			ssb += (array[i][j] - total_mean) ** 2
			sse += (array[i][j] - row_mean[i] - col_mean[j] + total_mean) ** 2
		
	# mean of square
	dfs = array.shape[0] - 1
	mss = ssb / dfs
	dfe = (array.shape[0] - 1) * (array.shape[1] - 1)
	mse = sse / dfe
	k = array.shape[1] - 1
	
	_icc = (mss - mse) / (mss + k * mse)
	
	return _icc		
	
		
# open or save file using Tkinter
def open_file(dialog_title):
	return filedialog.askopenfilename(title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))


def open_files(dialog_title):
	return filedialog.askopenfilenames(title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))


def save_file(dialog_title):
	return filedialog.asksaveasfilename(title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))


## Run SNR calculation
def run_time():
	## assign path and load file list
	root_path = "{}/time".format(os.getcwd())
	
	'''
	## for test
	files_raw_data = ['{}/raw_lpf/701 EO_001.txt'.format(root_path)]
	files_cln_data = ['{}/engine/701 EO_001.txt'.format(root_path)]
	files_man1_data = ['{}/manual1/701 EO_001.txt'.format(root_path)]
	files_man2_data = ['{}/manual2/701 EO_001.txt'.format(root_path)]
	result_name = '{}/result_test.txt'.format(root_path)
	
	## assign path with manual 
	# load raw data files
	files_raw_data = open_files("Open raw data files")

	# load cleaned(engine) data files
	files_cln_data = open_files("Open cleaned data files")
	
	# load manual1 data files
	files_man1_data = open_files("Open manual1 data files")
	
	# load manual2 data files
	files_man2_data = open_files("Open manual2 data files")
	
	# input result file name
	result_name = save_file("Input result file name")

	'''	
	
	path_raw_data = "{}/raw_lpf".format(root_path)
	files_raw_data = ["{}/{}".format(path_raw_data, f) for f in os.listdir(path_raw_data) if f[-3:] == 'txt']
	path_cln_data = "{}/engine".format(root_path)
	files_cln_data = ["{}/{}".format(path_cln_data, f) for f in os.listdir(path_cln_data) if f[-3:] == 'txt']
	path_man1_data = "{}/manual1".format(root_path)
	files_man1_data = ["{}/{}".format(path_man1_data, f) for f in os.listdir(path_man1_data) if f[-3:] == 'txt']
	path_man2_data = "{}/manual2".format(root_path)
	files_man2_data = ["{}/{}".format(path_man2_data, f) for f in os.listdir(path_man2_data) if f[-3:] == 'txt']
	result_name = '{}/result.txt'.format(root_path)
	
	# sorting
	files_raw_data = sorted(files_raw_data)
	files_cln_data = sorted(files_cln_data)
	files_man1_data = sorted(files_man1_data)
	files_man2_data = sorted(files_man2_data)
	
	# channel	
	# Fp1, Fp2, F7, F3, Fz, F4, F8, T3, C3, Cz, C4, T4, T5, P3, Pz, P4, T6, O1, O2
	ch = {0:'Fp1', 1:'Fp2', 2:'F7', 3:'F3', 4:'Fz', 5:'F4', 6:'F8', 7:'T3', 8:'C3', 9:'Cz',
			10:'C4', 11:'T4', 12:'T5', 13:'P3', 14:'Pz', 15:'P4', 16:'T6', 17:'O1', 18:'O2'}
	
	with open(result_name, 'w') as f:
		for idx_file in range(len(files_raw_data)):
			data_raw = arrange_matrix(np.loadtxt(files_raw_data[idx_file]))
			data_cln = arrange_matrix(np.loadtxt(files_cln_data[idx_file]))
			data_man1 = arrange_matrix(np.loadtxt(files_man1_data[idx_file]))
			data_man2 = arrange_matrix(np.loadtxt(files_man2_data[idx_file]))
		
			for idx_ch in ch.keys():
				snr_cln = SNR(data_raw[idx_ch], data_cln[idx_ch])
				snr_man1 = SNR(data_raw[idx_ch], data_man1[idx_ch])
				snr_man2 = SNR(data_raw[idx_ch], data_man2[idx_ch])
				
				if IS_PRINT: print("Ch({:>3}): {:08.5f}\t{:08.5f}\t{:08.5f} [dB]".format(
					ch.get(idx_ch), snr_cln, snr_man1, snr_man2))
				if IS_OUTPUT: f.write("{:08.5f}\t{:08.5f}\t{:08.5f}\t".format(
					snr_cln, snr_man1, snr_man2))
				
			if IS_OUTPUT: f.write("\n") 
	

## Run fft(spectopo) calculation
def run_freq():
	'''
	## for test
	root_path = "{}/fft".format(os.getcwd())
# 	files_raw_data = ['{}/raw/701 EO_001.txt'.format(os.getcwd())]
	files_cln_data = ['{}/engine/701 EO_001.txt'.format(root_path)]
	files_man1_data = ['{}/manual1/701 EO_001.txt'.format(root_path)]
	files_man2_data = ['{}/manual2/701 EO_001.txt'.format(root_path)]
	result_name = '{}/result_test.txt'.format(root_path)
	
	# load raw data files
	files_raw_data = open_files("Open raw data files")
	'''	

	# load cleaned(engine) data files
	files_cln_data = open_files("Open cleaned data files")
	
	# load manual1 data files
	files_man1_data = open_files("Open manual1 data files")
	
	# load manual2 data files
	files_man2_data = open_files("Open manual2 data files")
	
	# sorting
	files_cln_data = sorted(files_cln_data)
	files_man1_data = sorted(files_man1_data)
	files_man2_data = sorted(files_man2_data)
	
	# input result file name
	result_name = save_file("Input result file name")
		
	with open(result_name, 'w') as f:
		for i_dir in range(len(files_cln_data)):
			# print current processing file
			print(files_cln_data[i_dir][find_last_slash(files_cln_data[i_dir]):])
			
			# load data
# 			data_raw = arrange_matrix(np.loadtxt(files_raw_data[i_dir]))
			data_cln = arrange_matrix(np.loadtxt(files_cln_data[i_dir]))
			data_man1 = arrange_matrix(np.loadtxt(files_man1_data[i_dir]))
			data_man2 = arrange_matrix(np.loadtxt(files_man2_data[i_dir]))
			
			tmp_spectopo, tmp_x_cln, _ = spectopo(data_cln, 0, 250, "nfft", 512, "winsize", 512, "overlap", 256, "plot", "off")
			ret_cln = 10 ** (tmp_spectopo / 10)
			tmp_spectopo, tmp_x_man1, _ = spectopo(data_man1, 0, 250, "nfft", 512, "winsize", 512, "overlap", 256, "plot", "off")
			ret_man1 = 10 ** (tmp_spectopo / 10)
			tmp_spectopo, tmp_x_man2, _ = spectopo(data_man2, 0, 250, "nfft", 512, "winsize", 512, "overlap", 256, "plot", "off")
			ret_man2 = 10 ** (tmp_spectopo / 10)
				
			# Fp1, Fp2, F7, F3, Fz, F4, F8, T3, C3, Cz, C4, T4, T5, P3, Pz, P4, T6, O1, O2
			ch = {0:'Fp1', 1:'Fp2', 2:'F7', 3:'F3', 4:'Fz', 5:'F4', 6:'F8', 7:'T3', 8:'C3', 9:'Cz', \
				10:'C4', 11:'T4', 12:'T5', 13:'P3', 14:'Pz', 15:'P4', 16:'T6', 17:'O1', 18:'O2'}
			
			for i_ch in range(len(ch)):
# 				print(ch.get(i_ch))
				# cut 5 secs
				
				# apply HPF
								
				# draw fft figure
#				tmp_x_list = [tmp_x_raw, tmp_x_cln]				
#				tmp_y_list = [ret_raw, ret_cln]				
#				draw_3_plot(tmp_x_list, tmp_y_list, [30,30], [0.4,0.4], True)

				# delta:1-4, theta: 4-8, alpha:8-13, beta1: 13-21, beta2: 21-30
				band = ((1, 4), (4, 8), (8, 13), (13, 21), (21, 30))
				
				# average power each band
				for idx in band:
					idx_band_cln = np.where((tmp_x_cln > idx[0]) & (tmp_x_cln <= idx[1]))[0]
					idx_band_man1 = np.where((tmp_x_man1 > idx[0]) & (tmp_x_man1 <= idx[1]))[0]
					idx_band_man2 = np.where((tmp_x_man2 > idx[0]) & (tmp_x_man2 <= idx[1]))[0]
					_1st_idx_cln = idx_band_cln[0]
					_2nd_idx_cln = idx_band_cln[-1]
					_1st_idx_man1 = idx_band_man1[0]
					_2nd_idx_man1 = idx_band_man1[-1]
					_1st_idx_man2 = idx_band_man2[0]
					_2nd_idx_man2 = idx_band_man2[-1]
					
					x_len_cln = tmp_x_cln[_2nd_idx_cln-1] - tmp_x_cln[_1st_idx_cln]
					x_len_man1 = tmp_x_man1[_2nd_idx_man1-1] - tmp_x_man2[_1st_idx_man1]
					x_len_man2 = tmp_x_man2[_2nd_idx_man2-1] - tmp_x_man2[_1st_idx_man2]

					tmp_band_cln = ret_cln[i_ch][_1st_idx_cln:_2nd_idx_cln]
					tmp_band_man1 = ret_man1[i_ch][_1st_idx_man1:_2nd_idx_man1]
					tmp_band_man2 = ret_man2[i_ch][_1st_idx_man2:_2nd_idx_man2]
					
					average_power_cln = np.absolute(tmp_band_cln).mean() * x_len_cln
					average_power_man1 = np.absolute(tmp_band_man1).mean() * x_len_man1
					average_power_man2 = np.absolute(tmp_band_man2).mean() * x_len_man2
					
					if IS_PRINT: print("{:09.5f}\t{:09.5f}\t{:09.5f}".format(
						average_power_cln, average_power_man1, average_power_man2))
					if IS_OUTPUT: f.write("{:09.5f}\t{:09.5f}\t{:09.5f}\t".format(
						average_power_cln, average_power_man1, average_power_man2))
				
				'''
				# average power each Hz from 1 to 30
				idx_Hz = [(i, i+1) for i in range(1,30)]
				
				for idx in idx_Hz: 
# 					_1st_idx_raw = find_nearest(tmp_x_raw, idx_Hz[idx[0]])
# 					_2nd_idx_raw = find_nearest(tmp_x_raw, idx_Hz[idx[1]])
					_1st_idx_cln = find_nearest(tmp_x_cln, idx_Hz[idx[0]])
					_2nd_idx_cln = find_nearest(tmp_x_cln, idx_Hz[idx[1]])
					_1st_idx_man1 = find_nearest(tmp_x_man1, idx_Hz[idx[0]])
					_2nd_idx_man1 = find_nearest(tmp_x_man1, idx_Hz[idx[1]])
					_1st_idx_man2 = find_nearest(tmp_x_man2, idx_Hz[idx[0]])
					_2nd_idx_man2 = find_nearest(tmp_x_man2, idx_Hz[idx[1]])
					
# 					x_len_raw = tmp_x_raw[_2nd_idx_raw-1] - tmp_x_raw[_1st_idx_raw]
					x_len_cln = tmp_x_cln[_2nd_idx_cln-1] - tmp_x_cln[_1st_idx_cln]
					x_len_man1 = tmp_x_man1[_2nd_idx_man1-1] - tmp_x_man1[_1st_idx_man1]
					x_len_man2 = tmp_x_man2[_2nd_idx_man2-1] - tmp_x_man2[_1st_idx_man2]
					
# 					tmp_raw = ret_raw[_1st_idx_raw:_2nd_idx_raw]
					tmp_cln = ret_cln[i_ch][_1st_idx_cln:_2nd_idx_cln]
					tmp_man1 = ret_man1[i_ch][_1st_idx_man1:_2nd_idx_man1]
					tmp_man2 = ret_man2[i_ch][_1st_idx_man2:_2nd_idx_man2]
					
# 					average_power_raw = np.absolute(tmp_raw).mean() * x_len_raw
					average_power_cln = np.absolute(tmp_cln).mean() * x_len_cln
					average_power_man1 = np.absolute(tmp_man1).mean() * x_len_man1
					average_power_man2 = np.absolute(tmp_man2).mean() * x_len_man2
					
					if IS_PRINT: print("{:>3} {:02d} {:09.5f}\t{:09.5f}\t{:09.5f}".format(
						ch.get(i_ch), idx[0], average_power_cln, average_power_man1, average_power_man2))
					if IS_OUTPUT: f.write("{:09.5f}\t{:09.5f}\t{:09.5f}\t".format(
						average_power_cln, average_power_man1, average_power_man2))
				'''
					
			if IS_OUTPUT: f.write("\n")


def run_icc(str):
	
	root_path = "{}/{}".format(os.getcwd(), str)
	if str == 'time':
		result_file_name = "{}/result_for_time.txt".format(root_path)
		output_file_name = "{}/result_icc_for_time.txt".format(root_path)
	else:
		result_file_name = "{}/result.txt".format(root_path)
		output_file_name = "{}/result_icc.txt".format(root_path)
	a = np.loadtxt(result_file_name)
	'''
	result_file_name = open_file("Open result files")
	output_file_name = save_file("Input result of icc file name")
	a = np.loadtxt(result_file_name)
	'''
	column = 3
	b = reshape_for_anova(a, column)
	num_hz = int(b[0].shape[0])
	"num_hz = 551"
	
	ret_icc = []
	for i in range(num_hz):
		tmp_icc = [icc(b[0][i]), icc(b[1][i]), icc(b[2][i])]
		ret_icc.append(tmp_icc)
		
		if IS_PRINT:
			print("{:09.5f}\t{:09.5f}\t{:09.5f}".format(tmp_icc[0], tmp_icc[1], tmp_icc[2]))
			if i and (i + 1) % 29 is 0: print("")

	
	if str == 'time':
		pitch = 1
	elif str == 'fft':
		pitch = 5
	else:
		pitch = 29
		
	print(pitch)
	ret_icc = reshape_with_icc(ret_icc, pitch, 3)		
	
	if IS_OUTPUT:
		with open(output_file_name, 'w') as f:
			for i in range(ret_icc.shape[0]):
				for j in range(ret_icc.shape[1]):
					f.write("{:09.5f}\t".format(ret_icc[i][j]))
				f.write("\n")



def reshape_for_time():
	## assign path and load file list
	root_path = "{}/time".format(os.getcwd())
	
	path_cln_data = "{}/engine".format(root_path)
	files_cln_data = ["{}/{}".format(path_cln_data, f) for f in os.listdir(path_cln_data) if f[-3:] == 'txt']
	path_man1_data = "{}/manual1".format(root_path)
	files_man1_data = ["{}/{}".format(path_man1_data, f) for f in os.listdir(path_man1_data) if f[-3:] == 'txt']
	path_man2_data = "{}/manual2".format(root_path)
	files_man2_data = ["{}/{}".format(path_man2_data, f) for f in os.listdir(path_man2_data) if f[-3:] == 'txt']
	result_name = '{}/result_for_time.txt'.format(root_path)
	
	# sorting
	files_cln_data = sorted(files_cln_data)
	files_man1_data = sorted(files_man1_data)
	files_man2_data = sorted(files_man2_data)
	
	# channel	
	# Fp1, Fp2, F7, F3, Fz, F4, F8, T3, C3, Cz, C4, T4, T5, P3, Pz, P4, T6, O1, O2
	ch = {0:'Fp1', 1:'Fp2', 2:'F7', 3:'F3', 4:'Fz', 5:'F4', 6:'F8', 7:'T3', 8:'C3', 9:'Cz',
			10:'C4', 11:'T4', 12:'T5', 13:'P3', 14:'Pz', 15:'P4', 16:'T6', 17:'O1', 18:'O2'}
	
	tmp_out = []
	for idx_file in range(len(files_cln_data)):
		data_cln = arrange_matrix(np.loadtxt(files_cln_data[idx_file]), True)
		data_man1 = arrange_matrix(np.loadtxt(files_man1_data[idx_file]), True)
		data_man2 = arrange_matrix(np.loadtxt(files_man2_data[idx_file]), True)
		
		for i in range(data_cln.shape[0]):
			tmp = []
			for j in range(data_cln.shape[1]):
				tmp.append(data_cln[i][j])
				tmp.append(data_man1[i][j])
				tmp.append(data_man2[i][j])
			tmp_out.append(tmp)
			
	np.savetxt(result_name, np.array(tmp_out))
				
				
				
if __name__ == '__main__':
	s_time = time()
# 	run_time()
	run_icc('time')
# 	reshape_for_time()
# 	run_freq()
# 	run_icc('fft')
	print("done: {:.2f}".format(time() - s_time))
	




























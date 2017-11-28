import matplotlib
matplotlib.use("TkAgg")  # for prevent crash matplotlib and tkinter
import matplotlib.pyplot as plt
import numpy as np
from math import sqrt, log10
from tkinter import filedialog


## CONSTANT
IS_FFT = True




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
def SNR(raw_data, cleaned_data, sync=False):

	if sync:
		# difference raw and cleaned data with time sync
		if isinstance(raw_data, (np.ndarray, np.generic)) and isinstance(cleaned_data, (np.ndarray, np.generic)):
			noise_data = raw_data - cleaned_data
		else:
			noise_data = []
			for i in range(len(raw_data)):
				noise_data.appen(raw_data[i] - cleaned_data[i])
		
		# rms
		rms_noise = rms(noise_data)
		
	else:
		# rms
		rms_raw = rms(raw_data)
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
	
	# y axis in freq. domain
	idx = find_nearest(x_fft, freq_limit)
	tmp_y_fft = np.fft.fft(y) / n  # fft computing and normalization 
	tmp_y_fft = tmp_y_fft[range(int(n/2))]  # one side freq. range
	
	# freq. limit
	x_fft = tmp_x_fft[range(idx)]
	y_fft = tmp_y_fft[range(idx)]
	del tmp_x_fft, tmp_y_fft
	
	return x_fft, y_fft


def cut_side(array, FS=250, secs=5): 
	
	cut_size = int(FS*secs)
	
	return array[cut_size-1:len(array)-cut_size]


def arrange_matrix(array):
	
	if array.shape[0] is not 19:
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
		
		
# open or save file using Tkinter
def open_file(dialog_title):
	return filedialog.askopenfilename(title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))


def open_files(dialog_title):
# 	return filedialog.askopenfilenames(initialdir="/", title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))
	return filedialog.askopenfilenames(title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))


def save_file(dialog_title):
	return filedialog.asksaveasfilename(title=dialog_title, filetypes=(("text files","*.txt"),("all files","*.*")))


## Run SNR calculation
# need customization
def run():
	# Fp1, Fp2, F7, F3, Fz, F4, F8, T3, C3, Cz, C4, T4, T5, P3, Pz, P4, T6, O1, O2
	ch = {0:'Fp1', 1:'Fp2', 2:'F7', 3:'F3', 4:'Fz', 5:'F4', 6:'F8', 7:'T3', 8:'C3', 9:'Cz', \
		10:'C4', 11:'T4', 12:'T5', 13:'P3', 14:'Pz', 15:'P4', 16:'T6', 17:'O1', 18:'O2'}
	band = {1:'delta', 4:'theta', 8:'alpha', 13:'beta1', 21:'beta2'}

	'''
	## for test
	files_raw_data = ['/Volumes/ywkim/workspace/isyncbrain/p_interface/analysis/snr_test/fft/raw/701 EO_001.txt']
	files_cln_data = ['/Volumes/ywkim/workspace/isyncbrain/p_interface/analysis/snr_test/fft/engine/701 EO_001.txt']
	result_name = '/Volumes/ywkim/workspace/isyncbrain/p_interface/analysis/snr_test/fft/engine/result.txt'
	'''		
	
	# load raw data files
	files_raw_data = open_files("Open raw data files")

	# load cleaned(engine) data files
	files_cln_data = open_files("Open cleaned data files")
	
	# input result file name
	result_name = save_file("Input result file name")
		
	with open(result_name, 'w') as f:
		for i_dir in range(len(files_raw_data)):
			# print current processing file
			print(files_raw_data[i_dir][find_last_slash(files_raw_data[i_dir]):])
			
			# load data
			data_raw = arrange_matrix(np.loadtxt(files_raw_data[i_dir]))
			data_cln = arrange_matrix(np.loadtxt(files_cln_data[i_dir]))
			
			for i_ch in range(len(ch)):
				# cut 5 secs
				
				if IS_FFT:  # fft
					tmp_x_raw, ret_raw = fft(data_raw[i_ch], 250, 30)
					tmp_x_cln, ret_cln = fft(data_cln[i_ch], 250, 30)
				else:
					# time
					ret_raw = data_raw[i_ch]
					ret_cln = data_cln[i_ch]
				
				if IS_FFT:
					pass
					# apply HPF
									
					# check fft figure
# 					tmp_x_list = [tmp_x_raw, tmp_x_cln]				
# 					tmp_y_list = [ret_raw, ret_cln]				
# 					draw_3_plot(tmp_x_list, tmp_y_list, [30,30], [0.4,0.4], True)
# 					exit()

				# SNR
# 				_snr = SNR(ret_raw, ret_cln)
# 				f.write("{:08.5f}\t".format(_snr))
				
				# average power each band
				idx_band = sorted(band.keys())
				
				for idx in range(len(band)):
					
					if idx_band[idx] is 21:
						_1st_idx = find_nearest(tmp_x_raw, idx_band[idx])
						_2nd_idx = find_nearest(tmp_x_raw, 30)
						x_len = 30 - idx_band[idx]
					else:
						_1st_idx = find_nearest(tmp_x_raw, idx_band[idx])
						_2nd_idx = find_nearest(tmp_x_raw, idx_band[idx+1])
						x_len = idx_band[idx+1] - idx_band[idx]
					
					tmp_band = ret_raw[_1st_idx:_2nd_idx]
					average_power = np.absolute(tmp_band).mean() / x_len
# 					print("{:08.5f}".format(average_power))
					f.write("{:08.5f}\t\t".format(average_power))
				
			f.write("\n")



if __name__ == '__main__':
	run()
	print("done")
	




























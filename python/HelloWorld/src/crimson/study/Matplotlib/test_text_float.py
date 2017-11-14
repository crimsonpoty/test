from multiprocessing import Pool
from multiprocessing import current_process
from functools import partial

import threading

from matplotlib import figure, rc
# from matplotlib.backends.backend_cairo import FigureCanvasCairo as FigureCanvas
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas

import os
import inspect
frame = inspect.currentframe()

import time

def testTextFloat(img_num):
	
	h = figure.Figure()
	h.set_size_inches(12.5, 7.5)
	canvas = FigureCanvas(h)

	rc("font", family='STIXGeneral')
	
	ax = h.add_axes([0, 0, 0.3, 0.5])
# 	print("[Line:{}] PID: {}, id: {}".format(frame.f_lineno, current_process(), id(ax)))
	ax.axis("off")
	ax.patch.set_alpha(0)
	fontdict={"size": 30, "fontweight": "bold"}
	
	for i in range(10):
			ax.text(0.05, 1.9 - (i * 0.15), 'test_{0:03d}'.format(i), fontdict=fontdict)
		
	with open("test_{}.jpg".format(img_num), "wb") as f:
		h.savefig(f, dpi=100, format='JPEG')
	

if __name__ == "__main__":

	# delete existing jpg files in current path
	cmd_list = [ "rm {}".format(f) for f in os.listdir(os.getcwd()) if str(f).find(".jpg") > 0 ]
	for cmd in cmd_list:
		os.system(cmd)

	# run with process(unsafety with multi-process for fontd in FigureCanvas)		
	start_num, end_num, size = 0, 2, 2
	for i in range(size):
		pfunc = partial(testTextFloat)
		with Pool(2) as pool:
			pool.map(pfunc, range(start_num, end_num))
		start_num += size
		end_num += size

	for i in range(size):
		pfunc = partial(testTextFloat)
		with Pool(2) as pool:
			pool.map(pfunc, range(start_num, end_num))
		start_num += size
		end_num += size

	for i in range(size):
		pfunc = partial(testTextFloat)
		with Pool(2) as pool:
			pool.map(pfunc, range(start_num, end_num))
		start_num += size
		end_num += size

	# run with unit method		
	testTextFloat(99)
	
	# run with thread
	th_start_num, th_end_num = 20, 24
	
	for idx in range(th_start_num, th_end_num):
		t = threading.Thread(target=testTextFloat, args=(idx,), daemon=True)
		t.start()
		
	for idx in range(th_start_num, th_end_num):
		t.join()
		
	print("Done")

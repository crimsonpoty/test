import multiprocessing
import matplotlib.pyplot as plt

fig = plt.figure()
fig_lock = multiprocessing.Lock()

def plot(i):
	fig = plt.gcf()
	plt.plot([],[])
	fig_lock.acquire()
	fig.savefig('%d.png' % i)
	fig_lock.release()

plot(0)
pool = multiprocessing.Pool(4)
pool.map(plot, range(10))

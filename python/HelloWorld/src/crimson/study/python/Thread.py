'''
Created on 2017. 8. 16.

@author: crims
'''
import threading
import time

def say(msg):
    while True:
        time.sleep(1)
        print(msg)
        
class MyThread(threading.Thread):
    def __init__(self, msg):
        threading.Thread.__init__(self)
        self.msg = msg
        self.daemon = True
        
    def run(self):
        while True:
            time.sleep(1.5)
            print(self.msg)
        
if __name__ == "__main__":
    a = MyThread("This is class")
    a.start()
    
    for msg in ["you", "need", "python"]:
        t = threading.Thread(target=say, args=(msg,))        
        t.daemon = True
        t.start()
        
    for i in range(50):
        time.sleep(0.1)
        print(i)

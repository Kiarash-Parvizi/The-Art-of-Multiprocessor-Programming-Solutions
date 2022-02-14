import time
from multiprocessing import Process, Semaphore

n = 5  # number of philosophers
m = 10 # number of times each philosopher eats

# make semaphores
sticks = [Semaphore(1) for _ in range(n)]
procs = []

# making each philosopher hungry m times
for i in range(n):
    def f(i):
        stick1, stick2 = (sticks[i],sticks[0]) if i == n-1 else (sticks[i+1], sticks[i])
        for _ in range(m):
            stick1.acquire()
            print("pickup stick1 | done by:", i)
            stick2.acquire()
            print("pickup stick2 | done by:", i)
            print("Philosopher", i, "is eating")
            stick1.release()
            time.sleep(0.005)
            stick2.release()
    # create and start a new thread
    procs.append(Process(target = f, args = [i], daemon=True))
    procs[-1].start()

for proc in procs: proc.join()

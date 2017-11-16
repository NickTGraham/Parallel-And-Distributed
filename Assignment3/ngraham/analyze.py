import numpy as np
import matplotlib.pyplot as plt

# take in average sequential time, array of parrallel times, array of thread numbers
# and the plot name
def plotter(sequential, parallel, threads, title):
    S = [sequential/x for x in parallel] # make array of T(1)/T(N)
    fig = plt.figure(figsize=(20,20))
    plt.plot(threads, S)
    plt.xlabel("Threads")
    plt.ylabel("S(N)")
    plt.title(title)
    fig.savefig("Graphs/" + title + ".png", dpi=fig.dpi)
    plt.close(fig)

def fileParser(filename, o = False):
    with open(filename) as dataFile:
        lines = dataFile.readlines()
        lines = [x.strip() for x in lines]
        if o:
            time_lines = lines[1::3]
            print(time_lines)
        else:
            time_lines = lines[0::2]
        total = 0
        num = 0
        for line in time_lines:
            s = line.split()
            total += float(s[1])
            num += 1
        return total/num

#Constants
threads = [1, 4, 16, 24, 32, 48, 64, 128]
mem_order = ['b', 's', 'd']

# data for standard make
base = "plain/"
mat10 = "matrix_10.dat"
mat2000 = "matrix_2000.dat"
g10 = fileParser(base + "gauss/matrix_10.dat.out")
g2000 = fileParser(base + "gauss/matrix_2000.dat.out")


cilk10 = [];
for thread in threads:
    cilk10.append(fileParser("plain2/" +"cilk/"+ mat10 + str(thread) + ".out"))

cilk2000 = [];
for thread in threads:
    cilk2000.append(fileParser("plain2/" +"cilk/"+ mat2000 + str(thread) + ".out"))

openmp10 = []
openmp2000 = []
mem_cnt = 0
for mem in mem_order:
    openmp10.append([])
    for thread in threads:
        openmp10[mem_cnt].append(fileParser(base+"openmp/"+ mat10 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

mem_cnt = 0
for mem in mem_order:
    openmp2000.append([])
    for thread in threads:
        openmp2000[mem_cnt].append(fileParser(base+"openmp/"+ mat2000 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

plotter(g10, cilk10, threads, "Cilk 10x10 Matrix")
plotter(g2000, cilk2000, threads, "Cilk 2000x2000 Matrix")
plotter(g10, openmp10[0], threads, "OpenMP 10x10 Blocked")
plotter(g10, openmp10[1], threads, "OpenMP 10x10 Static")
plotter(g10, openmp10[2], threads, "OpenMP 10x10 Dynamic")
plotter(g2000, openmp2000[0], threads, "OpenMP 2000x2000 Blocked")
plotter(g2000, openmp2000[1], threads, "OpenMP 2000x2000 Static")
plotter(g2000, openmp2000[2], threads, "OpenMP 2000x2000 Dynamic")

# Data with nop
base = "nop/"
mat10 = "matrix_10.dat"
mat2000 = "matrix_2000.dat"
g10 = fileParser(base + "gauss/matrix_10.dat.out")
g2000 = fileParser(base + "gauss/matrix_2000.dat.out")


cilk10 = [];
for thread in threads:
    cilk10.append(fileParser(base +"cilk/"+ mat10 + "_" + str(thread) + "_b.out"))

cilk2000 = [];
for thread in threads:
    cilk2000.append(fileParser(base +"cilk/"+ mat2000 + "_" + str(thread) + "_b.out"))

openmp10 = []
openmp2000 = []
mem_cnt = 0
for mem in mem_order:
    openmp10.append([])
    for thread in threads:
        openmp10[mem_cnt].append(fileParser(base+"openmp/"+ mat10 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

mem_cnt = 0
for mem in mem_order:
    openmp2000.append([])
    for thread in threads:
        openmp2000[mem_cnt].append(fileParser(base+"openmp/"+ mat2000 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

plotter(g10, cilk10, threads, "Cilk -O0 10x10 Matrix")
plotter(g2000, cilk2000, threads, "Cilk -O0 2000x2000 Matrix")
plotter(g10, openmp10[0], threads, "OpenMP -O0 10x10 Blocked")
plotter(g10, openmp10[1], threads, "OpenMP -O0 10x10 Static")
plotter(g10, openmp10[2], threads, "OpenMP -O0 10x10 Dynamic")
plotter(g2000, openmp2000[0], threads, "OpenMP -O0 2000x2000 Blocked")
plotter(g2000, openmp2000[1], threads, "OpenMP -O0 2000x2000 Static")
plotter(g2000, openmp2000[2], threads, "OpenMP -O0 2000x2000 Dynamic")

# Data with 1op
base = "1op/"
mat10 = "matrix_10.dat"
mat2000 = "matrix_2000.dat"
g10 = fileParser(base + "gauss/matrix_10.dat.out")
g2000 = fileParser(base + "gauss/matrix_2000.dat.out")


cilk10 = [];
for thread in threads:
    cilk10.append(fileParser(base +"cilk/"+ mat10 + "_" + str(thread) + "_b.out"))

cilk2000 = [];
for thread in threads:
    cilk2000.append(fileParser(base +"cilk/"+ mat2000 + "_" + str(thread) + "_b.out"))

openmp10 = []
openmp2000 = []
mem_cnt = 0
for mem in mem_order:
    openmp10.append([])
    for thread in threads:
        openmp10[mem_cnt].append(fileParser(base+"openmp/"+ mat10 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

mem_cnt = 0
for mem in mem_order:
    openmp2000.append([])
    for thread in threads:
        openmp2000[mem_cnt].append(fileParser(base+"openmp/"+ mat2000 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

plotter(g10, cilk10, threads, "Cilk -O1 10x10 Matrix")
plotter(g2000, cilk2000, threads, "Cilk -O1 2000x2000 Matrix")
plotter(g10, openmp10[0], threads, "OpenMP -O1 10x10 Blocked")
plotter(g10, openmp10[1], threads, "OpenMP -O1 10x10 Static")
plotter(g10, openmp10[2], threads, "OpenMP -O1 10x10 Dynamic")
plotter(g2000, openmp2000[0], threads, "OpenMP -O1 2000x2000 Blocked")
plotter(g2000, openmp2000[1], threads, "OpenMP -O1 2000x2000 Static")
plotter(g2000, openmp2000[2], threads, "OpenMP -O1 2000x2000 Dynamic")

# Data with 2op
base = "2op/"
mat10 = "matrix_10.dat"
mat2000 = "matrix_2000.dat"
g10 = fileParser(base + "gauss/matrix_10.dat.out")
g2000 = fileParser(base + "gauss/matrix_2000.dat.out")


cilk10 = [];
for thread in threads:
    cilk10.append(fileParser(base +"cilk/"+ mat10 + "_" + str(thread) + "_b.out"))

cilk2000 = [];
for thread in threads:
    cilk2000.append(fileParser(base +"cilk/"+ mat2000 + "_" + str(thread) + "_b.out"))

openmp10 = []
openmp2000 = []
mem_cnt = 0
for mem in mem_order:
    openmp10.append([])
    for thread in threads:
        openmp10[mem_cnt].append(fileParser(base+"openmp/"+ mat10 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

mem_cnt = 0
for mem in mem_order:
    openmp2000.append([])
    for thread in threads:
        openmp2000[mem_cnt].append(fileParser(base+"openmp/"+ mat2000 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

plotter(g10, cilk10, threads, "Cilk -O2 10x10 Matrix")
plotter(g2000, cilk2000, threads, "Cilk -O2 2000x2000 Matrix")
plotter(g10, openmp10[0], threads, "OpenMP -O2 10x10 Blocked")
plotter(g10, openmp10[1], threads, "OpenMP -O2 10x10 Static")
plotter(g10, openmp10[2], threads, "OpenMP -O2 10x10 Dynamic")
plotter(g2000, openmp2000[0], threads, "OpenMP -O2 2000x2000 Blocked")
plotter(g2000, openmp2000[1], threads, "OpenMP -O2 2000x2000 Static")
plotter(g2000, openmp2000[2], threads, "OpenMP -O2 2000x2000 Dynamic")

# Data with 3op
base = "3op/"
mat10 = "matrix_10.dat"
mat2000 = "matrix_2000.dat"
g10 = fileParser(base + "gauss/matrix_10.dat.out")
g2000 = fileParser(base + "gauss/matrix_2000.dat.out")


cilk10 = [];
for thread in threads:
    cilk10.append(fileParser(base +"cilk/"+ mat10 + "_" + str(thread) + "_b.out"))

cilk2000 = [];
for thread in threads:
    cilk2000.append(fileParser(base +"cilk/"+ mat2000 + "_" + str(thread) + "_b.out"))

openmp10 = []
openmp2000 = []
mem_cnt = 0
for mem in mem_order:
    openmp10.append([])
    for thread in threads:
        openmp10[mem_cnt].append(fileParser(base+"openmp/"+ mat10 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

mem_cnt = 0
for mem in mem_order:
    openmp2000.append([])
    for thread in threads:
        openmp2000[mem_cnt].append(fileParser(base+"openmp/"+ mat2000 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

plotter(g10, cilk10, threads, "Cilk -O3 10x10 Matrix")
plotter(g2000, cilk2000, threads, "Cilk -O3 2000x2000 Matrix")
plotter(g10, openmp10[0], threads, "OpenMP -O3 10x10 Blocked")
plotter(g10, openmp10[1], threads, "OpenMP -O3 10x10 Static")
plotter(g10, openmp10[2], threads, "OpenMP -O3 10x10 Dynamic")
plotter(g2000, openmp2000[0], threads, "OpenMP -O3 2000x2000 Blocked")
plotter(g2000, openmp2000[1], threads, "OpenMP -O3 2000x2000 Static")
plotter(g2000, openmp2000[2], threads, "OpenMP -O3 2000x2000 Dynamic")

# Data with ibm
base = "ibm/"
mat10 = "matrix_10.dat"
mat2000 = "matrix_2000.dat"
g10 = fileParser(base + "gauss/matrix_10.dat.out")
g2000 = fileParser(base + "gauss/matrix_2000.dat.out")

openmp10 = []
openmp2000 = []
mem_cnt = 0
for mem in mem_order:
    openmp10.append([])
    for thread in threads:
        openmp10[mem_cnt].append(fileParser(base+"openmp/"+ mat10 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

mem_cnt = 0
for mem in mem_order:
    openmp2000.append([])
    for thread in threads:
        openmp2000[mem_cnt].append(fileParser(base+"openmp/"+ mat2000 + "_" + str(thread) + "_" + mem + ".out", True))
    mem_cnt +=1

plotter(g10, openmp10[0], threads, "OpenMP IBM 10x10 Blocked")
plotter(g10, openmp10[1], threads, "OpenMP IBM 10x10 Static")
plotter(g10, openmp10[2], threads, "OpenMP IBM 10x10 Dynamic")
plotter(g2000, openmp2000[0], threads, "OpenMP IBM 2000x2000 Blocked")
plotter(g2000, openmp2000[1], threads, "OpenMP IBM 2000x2000 Static")
plotter(g2000, openmp2000[2], threads, "OpenMP IBM 2000x2000 Dynamic")

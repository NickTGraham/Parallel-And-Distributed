import numpy as np
import matplotlib.pyplot as plt
def plotter(sequential, parallel, threads, title):
    S = [sequential/x for x in parallel] # make array of T(1)/T(N)
    fig = plt.figure(figsize=(20,20))
    plt.plot(threads, S)
    plt.xlabel("Threads")
    plt.ylabel("S(N)")
    plt.title(title)
    fig.savefig("Graphs/" + title + ".png", dpi=fig.dpi)
    plt.close(fig)


threads = [1, 2, 4, 8, 16, 24, 32, 48]
small = [0.000314,  0.001667, 0.002048, 0.004799, 0.005287, 0.004669, 0.005823, 0.051447]
large = [2.113552,  1.695201, 1.690000, 3.287834, 5.136777, 5.178973, 5.152713, 5.797325]

plotter(6e-06, small, threads, "MPI 10x10 Speedup")
plotter(9.73597433333, large, threads, "MPI 2000x2000 Speedup")

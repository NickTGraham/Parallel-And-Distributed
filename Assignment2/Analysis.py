import numpy as np
import matplotlib.pyplot as plt
import csv

def plotData(Data, title):
    threads = [1, 4, 16, 24, 32, 48, 64]
    Locks = ["C++ Mutex", "TS", "TAS Backoff", "Test and Test and Set", "Ticket Lock", "Ticket Lock Backoff", "MCS", "K42 MCS", "CLH", "K42 CLH"]
    yrng = np.arange(0, 10, 1)
    fig = plt.figure(figsize=(20,20))
    plt.imshow(Data)
    plt.colorbar()
    plt.yticks([0,1,2,3,4,5,6],threads)
    plt.xticks(yrng, Locks)
    plt.title(title)
    fig.savefig("Graphs/" + title + ".png", dpi=fig.dpi)
with open("x86TestsLong.csv") as csvFile:
    dataReader = csv.reader(csvFile)
    Data = []
    i = j = 0
    for row in dataReader:
        Data.append([])
        for col in row:
            Data[i].append(float(col))
            j += 1
        i += 1
    Data = np.array(Data)
    Data = np.delete(Data, [0,1], axis=1)
    Data100 = Data[0::4]
    Data1000 = Data[1::4]
    Data10000 = Data[2::4]
    Data100000 = Data[3::4]

    plotData(Data100, "100 Count per Thread")
    plotData(Data1000, "1000 Count per Thread")
    plotData(Data10000, "10000 Count per Thread")
    plotData(Data100000, "100000 Count per Thread")

with open("ibmTestsLong.csv") as csvFile:
    dataReader = csv.reader(csvFile)
    Data = []
    i = 0
    for row in dataReader:
        Data.append([])
        for col in row:
            Data[i].append(float(col))
        i += 1

    Data = np.array(Data)
    Data = np.delete(Data, [0,1], axis=1)
    Data100 = Data[0::3]
    Data1000 = Data[1::3]
    Data10000 = Data[2::3]

    plotData(Data100, "100 Count per Thread IBM")
    plotData(Data1000, "1000 Count per Thread IBM")
    plotData(Data10000, "10000 Count per Thread IBM")

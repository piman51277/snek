import matplotlib.pyplot as plt
import numpy as np


def getFrequencies(filename):

    # import text file of ints as np array
    data = np.loadtxt(filename, dtype=int)

    maxV = max(data)
    minV = min(data)

    freq = np.zeros(maxV - minV + 1, dtype=int)

    for i in range(len(data)):
        freq[data[i] - minV] += 1

    return np.arange(minV, maxV + 1), freq


sz, freq = getFrequencies("data/resultsham.txt")
plt.plot(sz, freq, color="black")

sz, freq = getFrequencies("data/resultsunopalgo.txt")
plt.plot(sz, freq,  color="blue")

sz, freq = getFrequencies("data/resultsalgo.txt")
plt.plot(sz, freq,  color="red")

plt.xlim(0, 1500)

plt.legend(["Hamiltonian Cycle",
           "Modified Cycle", "Modified Cycle + Endgame Hamiltonian"])

plt.xlabel("Turns to Finish Game")
plt.ylabel("Frequency")
plt.title("Distribution of Game Lengths on 10^6 iterations")

plt.show()

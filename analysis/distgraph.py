import matplotlib.pyplot as plt
import numpy as np


def getFrequencies(filename):

    # import text file of ints as np array
    data = np.loadtxt(filename, dtype=int)

    maxV = max(data)

    freq = np.zeros(maxV + 1, dtype=int)

    for i in range(len(data)):
        freq[data[i]] += 1

    return freq, maxV


freq, maxV = getFrequencies("data/resultsham.txt")
plt.plot(np.arange(maxV + 1), freq, color="black")

freq, maxV = getFrequencies("data/resultsunopalgo.txt")
plt.plot(np.arange(maxV + 1), freq, color="blue")

freq, maxV = getFrequencies("data/resultsalgo.txt")
plt.plot(np.arange(maxV + 1), freq, color="red")

plt.legend(["Hamiltonian Cycle",
           "Modified Cycle", "Modified Cycle + Hamiltonian"])

plt.xlabel("Turns to Finish Game")
plt.ylabel("Frequency")
plt.title("Distribution of Game Lengths on 10^6 iterations")

plt.show()

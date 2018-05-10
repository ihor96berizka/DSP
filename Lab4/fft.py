import matplotlib.pyplot as plt
#import plotly.plotly as py
import numpy as np
from scipy.fftpack import fft
from scipy.fftpack import ifft

N = 200
fd = 400.0
T = 1.0 / fd
sig_freq = 50.0

#вхідний сигнал
t = np.linspace(0.0, N*T, N)
y = np.sin(sig_freq * 2.0 * np.pi * t)

#Фур'є перетворення
yf = fft(y)
#yf = np.resize(yf, N//2)
xf = np.linspace(0.0, 1.0 / (2.0 * T), N//2)

#зворотнє Фур'є перетворення
it = t
iy = ifft(yf)

#відображення графіків
fig, axis = plt.subplots(3, 1)
axis[0].plot(t, y)
axis[1].plot(xf, abs(np.resize(yf, N//2)))
axis[2].plot(it, iy)
plt.show()

import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ser = serial.Serial('COM11', 9600, timeout=1)

x_data, y_data = [], []
fig, ax = plt.subplots()
line, = ax.plot([], [])
ax.set_xlabel('Sample')
ax.set_ylabel('Sensor Output')
ax.set_ylim(-0.5, 1.5)

def init():
    line.set_data([], [])
    return line,

def update(frame):
    raw = ser.readline().decode().strip()
    try:
        val = int(raw)
    except:
        return line,
    x_data.append(len(x_data))
    y_data.append(val)
    x_min = max(0, len(x_data) - 100)
    ax.set_xlim(x_min, x_min + 100)
    line.set_data(x_data, y_data)
    return line,

ani = animation.FuncAnimation(fig, update, init_func=init, blit=True, interval=100)
plt.show()

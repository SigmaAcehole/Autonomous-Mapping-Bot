import matplotlib.pyplot as plt
import matplotlib.animation as animation
import requests

r = [100]
theta = [90]

fig = plt.figure(facecolor='#161616'); # BG color
ax = fig.add_subplot(projection='polar')


def updatePlot(x, incomingTheta, incomingR):
    response = requests.get('http://192.168.43.180/')   # Replace with the IP from bot
    dataString = response.text
    values = dataString.split()
    incomingTheta.append(float(values[0]))
    incomingR.append(float(values[1]))
    if (len(incomingTheta) > 2):
        if (incomingTheta[-1] < incomingTheta[-2]):
            incomingTheta.clear()
            incomingR.clear()
    ax.clear()
    ax.scatter(incomingTheta, incomingR, color = '#2AEF73') # Data point color
    ax.set_thetamin(0)
    ax.set_thetamax(180)
    ax.set_facecolor("#091147")   # Plot area color
    rlabels = ax.get_ymajorticklabels()
    for label in rlabels:
        label.set_color('white')
    thetalabels = ax.get_xmajorticklabels()
    for label in thetalabels:
        label.set_color('white')
    print(incomingTheta, incomingR)
    ax.grid(True)
    ax.set_title("Real Time Polar Plot", va='bottom', color = 'white')
    ax.xaxis.grid(True,color='#334CF3')    # Grid color


ani = animation.FuncAnimation(fig, updatePlot, fargs=(theta, r), frames=100, interval=400, repeat=True)
plt.show()
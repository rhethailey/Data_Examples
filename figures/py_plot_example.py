import numpy as np
import matplotlib.pyplot as plt

# Set the color palette
col = plt.cm.viridis(np.linspace(0, 1, 4))

# Set plot style
plt.rcParams['text.usetex'] = True
plt.rcParams['axes.labelsize'] = 20
plt.rcParams['axes.titlesize'] = 20
plt.rcParams['axes.linewidth'] = 3
plt.rcParams['axes.prop_cycle'] = plt.cycler(color=col)

# Data
dt = 0.01
t = np.arange(0, 2*np.pi, dt)
y1 = np.sin(t)
y2 = np.sin(2*t)
x = np.cos(t)

# Create figure
fig, ax = plt.subplots(figsize=(10, 6))

# Plot data
py1, = ax.plot(t, y1, label=r'$\sin(t)$')
py2, = ax.plot(t, y2, label=r'$\sin(2t)$')
px, = ax.plot(t, x, label=r'$\cos(t)$')

# Set legend
ax.legend(loc='upper center', fontsize=32, bbox_to_anchor=(0.5, -0.05), ncol=3)

# Set labels
ax.set_ylabel("Output", fontsize=32)
ax.set_xlabel("Time [s]", fontsize=32)

# Set axis limits and ticks
# # xlim = [0, 2*np.pi]
# # ylim = [-1, 1]*1.25
# ax.set_xlim(xlim)
# ax.set_ylim(ylim)
ax.set_xticks(np.arange(0, 2*np.pi+np.pi/2, np.pi/2))
ax.set_xticklabels([r'$0$', r'$\pi/2$', r'$\pi$', r'$3\pi/2$', r'$2\pi$'])
ax.set_yticks(np.arange(-1, 1.5, 0.5))

# Apply grid
ax.grid(True)

# Save figure
answer = input('Do you want to save your figs? (yes/no): ')

if answer.lower() == 'yes':
    fig.tight_layout()
    plt.savefig('figs/lazy_plot.pdf', format='pdf')

plt.show()

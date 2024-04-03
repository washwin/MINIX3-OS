import matplotlib.pyplot as plt
import numpy as np

categories = ['Process1.dat', 'Process2.dat', 'Process3.dat', 'test1.dat', 'test2.dat', 'test3.dat']
sjf_values = [551.428589, 166.333328, 797.250000, 8.666667, 151.000000, 10.000000]
srtf_values = [406.571442, 99.388885, 716.666687, 2.666667, 5.750000, 5.333333]

# Set the width of the bars
bar_width = 0.35

# Create an array of indices for each category
indices = np.arange(len(categories))

# Creating bar graphs
plt.bar(indices - bar_width/2, sjf_values, width=bar_width, color='blue', label='SJF')
plt.bar(indices + bar_width/2, srtf_values, width=bar_width, color='orange', label='SRTF')

# Adding labels and title
plt.xlabel('Process data files')
plt.ylabel('Time')
plt.title('Average Waiting Time')
plt.legend()

# Adjust the x-axis ticks to show category names
plt.xticks(indices, categories)

# Display the graph
plt.savefig('waitingtime.png')

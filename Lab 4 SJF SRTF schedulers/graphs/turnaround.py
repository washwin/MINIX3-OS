import matplotlib.pyplot as plt
import numpy as np
 
categories = ['Process1.dat', 'Process2.dat', 'Process3.dat', 'test1.dat', 'test2.dat', 'test3.dat']
sjf_values = [755.285706, 219.500000, 971.333313, 17.333334, 204.500000, 16.666666]
srtf_values = [610.428589,152.555557,890.750000, 11.333333,  59.250000,  12.000000]

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
plt.title('Average Turnaround Time')
plt.legend()

# Adjust the x-axis ticks to show category names
plt.xticks(indices, categories)

# Display the graph
plt.savefig('turnaround.png')

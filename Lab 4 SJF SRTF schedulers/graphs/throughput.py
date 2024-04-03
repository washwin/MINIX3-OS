import matplotlib.pyplot as plt
import numpy as np
 
categories = ['Process1.dat', 'Process2.dat', 'Process3.dat', 'test1.dat', 'test2.dat', 'test3.dat']
sjf_values = [0.004899, 0.018730, 0.005739, 0.107143, 0.018605, 0.150000]
srtf_values = [0.004899,0.018595,0.005739, 0.103448,  0.018692, 0.136364]

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
plt.title('System Throughput')
plt.legend()

# Adjust the x-axis ticks to show category names
plt.xticks(indices, categories)

# Display the graph
plt.savefig('throughput.png')

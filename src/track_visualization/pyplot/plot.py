import matplotlib.pyplot as plt

# Sample 3D points (x, y, z)
points = [
    (1, 1, 3),
    (1, 2, 6),
    (1, 3, 2),
    (1, 4, 1),
    (0, 4, -1)
]

# Extract x and y coordinates, ignoring z
x_coords = [point[0] for point in points]
y_coords = [point[1] for point in points]

# Create the plot
plt.scatter(x_coords, y_coords, marker='x', color='red')

# Add labels and title
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('2D Plot of Points with X Markers')

# Add grid
plt.grid(True)

# Show the plot
plt.show()

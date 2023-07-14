import matplotlib.pyplot as plt
import numpy as np
import csv

# Read data from CSV file
filename = './data/sat_gpa_data.csv'

x = []  # Independent variable
y = []  # Dependent variable

with open(filename, 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip header row
    for row in reader:
        x.append(float(row[0]))
        y.append(float(row[1]))

# scale the freaking X axis :)
min_x = min(x)
max_x = max(x)
x = [(data - min_x) / (max_x - min_x) for data in x]

# Fit a linear regression model
coefficients = np.polyfit(x, y, 1)
intercept, slope = coefficients

print(f"y = {slope} * x + {intercept}")

# Generate points for the regression line
regression_line_x = np.linspace(min(x), max(x), 100)
regression_line_y = np.polyval(coefficients, regression_line_x)

rust_regression_line_y =  2.9764156 +  regression_line_x * 0.6901345

# Plot the data points and the regression line
plt.scatter(x, y, color='blue', label='Data')

plt.plot(regression_line_x, regression_line_y, color='green', label='Python Calc Linear Regression')

plt.plot(regression_line_x, rust_regression_line_y, color='red', label='Rust SGD Linear Regression')

plt.xlabel('Independent Variable')
plt.ylabel('Dependent Variable')
plt.title('Linear Regression')

plt.legend()
plt.grid(True)
plt.show()


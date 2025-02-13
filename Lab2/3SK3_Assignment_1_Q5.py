# Compute the true percent relative error for each approximation
# Write a Matlab code to plot

# Mehak Shah - shahm77 - 400478491

import numpy as np
import matplotlib.pyplot as plt
from math import log

def f(x):
    return x**3 - 2*np.log(x)

def taylor_approximation(x, order):
    # Corrected coefficients calculated at x=1
    coefficients = [1, 1, 8/2, 2/6, 12/24]  # [f(1), f'(1), f''(1)/2!, f'''(1)/3!, f⁴(1)/4!]
    result = 0
    for n in range(order + 1):
        result += coefficients[n] * (x - 1)**n
    return result

# Calculate actual value and approximations
x_actual = 3
y_actual = f(x_actual)

orders = range(5)  # 0 through 4th order
approximations = [taylor_approximation(x_actual, order) for order in orders]
errors = [(abs(y_actual - approx)/abs(y_actual))*100 for approx in approximations]

# Create visualization
plt.figure(figsize=(12, 6))

# Plot the function and approximations
x = np.linspace(0.5, 3.5, 1000)
plt.subplot(1, 2, 1)
plt.plot(x, f(x), 'k-', label='Actual function')
colors = ['red', 'blue', 'green', 'purple', 'orange']
for order, color in zip(orders, colors):
    x_plot = np.linspace(0.8, 3.2, 100)
    y_plot = [taylor_approximation(xi, order) for xi in x_plot]
    plt.plot(x_plot, y_plot, '--', color=color, label=f'{order}th order')
plt.plot(1, f(1), 'ko', label='Base point')
plt.plot(3, f(3), 'ko', label='Target point')
plt.grid(True)
plt.legend()
plt.title('Taylor Series Approximations')
plt.xlabel('x')
plt.ylabel('f(x)')

# Plot the true percent relative errors
plt.subplot(1, 2, 2)
plt.bar(orders, errors, color=colors)
plt.grid(True)
plt.title('True Percent Relative Error')
plt.xlabel('Order of Approximation')
plt.ylabel('Error (%)')

# Print numerical results
print("\nNumerical Results:")
print("Order | Approximation | True Relative Error (%)")
print("-" * 45)
for order, approx, error in zip(orders, approximations, errors):
    print(f"{order:5d} | {approx:12.4f} | {error:18.4f}")

plt.tight_layout()
plt.show()

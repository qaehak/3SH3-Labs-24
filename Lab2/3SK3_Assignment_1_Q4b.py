# Goal determine number of terms required to achieve et for pi/6 and pi/3
# Relative error (et) <= 1e-9

# Mehak Shah - shahm77 - 400478491

import math

def cos_mclaurin(x, tol=1e-9):
    term = 1  # First term in the series
    cos_x = term  # Initialize sum
    n = 0  # Start at the first term
    
    while abs(term) > tol * abs(cos_x):  # Check relative error
        n += 1
        term *= -x**2 / ((2*n - 1) * (2*n))  # Compute next term using Maclaurin series
        cos_x += term
    
    return cos_x, n

# Given values
x1 = math.pi / 6  # 30 degrees
x2 = math.pi / 3  # 60 degrees

# Compute cosine using McLaurin series
cos_x1, terms_x1 = cos_mclaurin(x1)
cos_x2, terms_x2 = cos_mclaurin(x2)

# Compare with built-in math.cos
actual_cos_x1 = math.cos(x1)
actual_cos_x2 = math.cos(x2)

# Compute relative error
relative_error_x1 = ((actual_cos_x1 - cos_x1)/cos_x1)
relative_error_x2 = ((actual_cos_x2 - cos_x2)/cos_x2)

print(f"pi/6:\ncos({x1}) ≈ {cos_x1}\nActual: {actual_cos_x1}\nTerms used: {terms_x1}\nRelative error: {relative_error_x1}\n")
print(f"pi/3:\ncos({x2}) ≈ {cos_x2}\nActual: {actual_cos_x2}\nTerms used: {terms_x2}\nRelative error: {relative_error_x2}\n")

# Mehak Shah - shahm77 - 400478491

def find_smallest_positive():
    # Start with x = 1
    x = 1.0
    
    # Keep multiplying x by 0.5 until we can't get any smaller
    # without reaching zero
    while x > 0.0:
        x_prev = x
        x = x / 2.0
    
    # x_prev is the smallest positive number before we hit zero
    return x_prev

# Run the calculation and print the result
x_min = find_smallest_positive()
print(f"Smallest positive number: {x_min}")

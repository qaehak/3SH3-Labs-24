# Mehak Shah - shahm77 - 400478491

def find_machine_epsilon():
    # Start with epsilon = 1
    epsilon = 1.0
    
    # Keep dividing epsilon by 2 until we find the smallest value
    # where 1 + epsilon is still distinguishable from 1
    while (1.0 + epsilon) > 1.0:
        epsilon_prev = epsilon
        epsilon = epsilon / 2.0
    
    # The last value of epsilon that worked is epsilon_prev
    return epsilon_prev

# Run the calculation and print the result
epsilon_mach = find_machine_epsilon()
print(f"Machine Epsilon: {epsilon_mach}")

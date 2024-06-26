import numpy as np

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def initialize_harmony_memory(pop_size, dim, bounds):
    return np.random.uniform(bounds[0], bounds[1], (pop_size, dim))

def pitch_adjustment(parameter, lower_bound, upper_bound):
    # Adjust the pitch (parameter) within the specified bounds
    adjusted_parameter = parameter
    if np.random.rand() < 0.5:
        adjusted_parameter += np.random.uniform(-1, 1) * (upper_bound - lower_bound)
    return adjusted_parameter

def harmony_search(pop_size, dim, bounds, max_iterations, harmony_memory_rate=0.95, pitch_adjustment_rate=0.7):
    # Initialize harmony memory
    harmony_memory = initialize_harmony_memory(pop_size, dim, bounds)
    
    # Main loop
    for iteration in range(max_iterations):
        # Generate new harmonies
        new_harmonies = np.zeros_like(harmony_memory)
        for i in range(pop_size):
            for d in range(dim):
                if np.random.rand() < pitch_adjustment_rate:
                    new_harmonies[i, d] = pitch_adjustment(harmony_memory[i, d], bounds[0], bounds[1])
                else:
                    new_harmonies[i, d] = np.random.uniform(bounds[0], bounds[1])
        
        # Evaluate new harmonies
        new_fitness = np.array([objective_function(harmony) for harmony in new_harmonies])
        
        # Update harmony memory
        for i in range(pop_size):
            if new_fitness[i] < objective_function(harmony_memory[i]):
                if np.random.rand() < harmony_memory_rate:
                    harmony_memory[i] = new_harmonies[i]
        
        # Print best fitness value at each iteration
        best_fitness = np.min([objective_function(harmony) for harmony in harmony_memory])
        print(f"Iteration {iteration + 1}, Best Fitness: {best_fitness}")
    
    # Find the best harmony
    best_index = np.argmin([objective_function(harmony) for harmony in harmony_memory])
    best_solution = harmony_memory[best_index]
    best_fitness = objective_function(best_solution)
    
    return best_solution, best_fitness

# Parameters
pop_size = 50
dim = 10
bounds = (-5, 5)
max_iterations = 100
harmony_memory_rate = 0.95  # Memory consideration rate
pitch_adjustment_rate = 0.7  # Pitch adjustment rate

# Run Harmony Search
best_solution, best_fitness = harmony_search(pop_size, dim, bounds, max_iterations, harmony_memory_rate, pitch_adjustment_rate)

# Print results
print("\n--- Optimization Results ---")
print(f"Best solution found: {best_solution}")
print(f"Best fitness: {best_fitness}")
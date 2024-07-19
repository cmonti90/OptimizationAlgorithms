import numpy as np
import scipy

def levy_flight(beta=1.5):
    # Generate step from Levy flight distribution
    sigma_u = (scipy.special.gamma(1 + beta) * np.sin(np.pi * beta / 2) / (scipy.special.gamma((1 + beta) / 2) * beta * 2**((beta - 1) / 2)))**(1 / beta)
    sigma_v = 1
    u = np.random.normal(0, sigma_u, size=1)
    v = np.random.normal(0, sigma_v, size=1)
    step = u / np.power(np.abs(v), 1 / beta)
    return step

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def cuckoo_search(pop_size, dim, bounds, max_iterations):
    # Initialize population
    population = np.random.uniform(bounds[0], bounds[1], (pop_size, dim))
    
    # Main loop
    for iteration in range(max_iterations):
        # Generate new solutions (eggs) using Levy flights
        new_population = population.copy()
        for i in range(pop_size):
            step_size = levy_flight()
            new_solution = population[i] + step_size * (population[np.random.randint(pop_size)] - population[np.random.randint(pop_size)])
            new_solution = np.clip(new_solution, bounds[0], bounds[1])
            if objective_function(new_solution) < objective_function(population[i]):
                population[i] = new_solution
        
        # Abandon eggs with a probability (replace with new random solutions)
        for i in range(pop_size):
            if np.random.rand() < 0.2:  # Abandonment rate (adjustable)
                population[i] = np.random.uniform(bounds[0], bounds[1], dim)
        
        # Print best fitness value at each iteration
        best_fitness = np.min([objective_function(individual) for individual in population])
        print(f"Iteration {iteration + 1}, Best Fitness: {best_fitness}")
    
    # Find the best individual
    best_index = np.argmin([objective_function(individual) for individual in population])
    best_solution = population[best_index]
    best_fitness = objective_function(best_solution)
    
    return best_solution, best_fitness

# Parameters
pop_size = 50
dim = 10
bounds = (-5, 5)
max_iterations = 1000

# Run Cuckoo Search
best_solution, best_fitness = cuckoo_search(pop_size, dim, bounds, max_iterations)

# Print results
print("\n--- Optimization Results ---")
print(f"Best solution found: {best_solution}")
print(f"Best fitness: {best_fitness}")
import numpy as np

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def differential_evolution(pop_size, dim, bounds, max_iterations, F=0.5, CR=0.7):
    # Initialize population
    population = np.random.uniform(bounds[0], bounds[1], (pop_size, dim))
    
    # Main loop
    for iteration in range(max_iterations):
        for i in range(pop_size):
            # Select three unique individuals
            candidates = np.random.choice(pop_size, 3, replace=False)
            x1, x2, x3 = population[candidates]
            
            # Mutation
            mutant_vector = x1 + F * (x2 - x3)
            
            # Crossover
            crossover_mask = np.random.rand(dim) < CR
            trial_vector = np.where(crossover_mask, mutant_vector, population[i])
            
            # Selection
            if objective_function(trial_vector) < objective_function(population[i]):
                population[i] = trial_vector
        
        # Optional: Print best fitness value at each iteration
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
max_iterations = 100
F = 0.5  # Mutation factor
CR = 0.7  # Crossover probability

# Run Differential Evolution
best_solution, best_fitness = differential_evolution(pop_size, dim, bounds, max_iterations, F, CR)

# Print results
print("\n--- Optimization Results ---")
print(f"Best solution found: {best_solution}")
print(f"Best fitness: {best_fitness}")
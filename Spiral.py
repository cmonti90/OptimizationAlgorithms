
import numpy as np

def objective_function(x):
    # Example objective function: Sphere function for higher dimensions
    return np.sum(x**2)

def spiral_optimization(num_candidates, num_iterations, r, theta, dim, bounds):
    # Initialize population
    population = np.random.uniform(bounds[0], bounds[1], (num_candidates, dim))
    best_candidate = None
    best_fitness = float('inf')

    for _ in range(num_iterations):
        # Evaluate fitness
        fitness = np.array([objective_function(candidate) for candidate in population])

        # Update center (best candidate)
        best_idx = np.argmin(fitness)
        center = population[best_idx]
        if fitness[best_idx] < best_fitness:
            best_fitness = fitness[best_idx]
            best_candidate = center

        # Spiral update
        for i in range(num_candidates):
            displacement = (population[i] - center)
            population[i] = center + displacement * r * np.cos(theta)
            population[i] = np.clip(population[i], bounds[0], bounds[1])  # Ensure within bounds

    return best_candidate

# Parameters
num_candidates = 30
num_iterations = 100
r = 0.9
theta = np.pi / 4
dim = 10  # Higher dimension
bounds = (-5, 5)

# Run spiral optimization
best_solution = spiral_optimization(num_candidates, num_iterations, r, theta, dim, bounds)
print("Best solution found:", best_solution)
print("Objective value:", objective_function(best_solution))
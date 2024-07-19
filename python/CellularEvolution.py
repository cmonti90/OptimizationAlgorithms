import numpy as np

def objective_function(x):
    # Example objective function: Sphere function for higher dimensions
    return np.sum(x**2)

def initialize_population(pop_size, dim, bounds):
    return np.random.uniform(bounds[0], bounds[1], (pop_size, dim))

def evaluate_population(population):
    return np.array([objective_function(ind) for ind in population])

def select_neighborhood(index, grid_size):
    neighbors = []
    x, y = index
    neighbors.append((x, (y+1) % grid_size[1]))
    neighbors.append((x, (y-1) % grid_size[1]))
    neighbors.append(((x+1) % grid_size[0], y))
    neighbors.append(((x-1) % grid_size[0], y))
    return neighbors

def select_parents(neighborhood, population, fitness):
    return [population[n] for n in neighborhood], [fitness[n] for n in neighborhood]

def apply_crossover_and_mutation(parents, bounds, mutation_rate=0.1):
    parent1, parent2 = parents
    crossover_point = np.random.randint(0, len(parent1))
    offspring = np.concatenate((parent1[:crossover_point], parent2[crossover_point:]))
    mutation = np.random.uniform(bounds[0], bounds[1], offspring.shape) * mutation_rate
    offspring += mutation
    return np.clip(offspring, bounds[0], bounds[1])

def cellular_evolutionary_algorithm(grid_size, dim, bounds, num_iterations, mutation_rate=0.1):
    population = initialize_population(grid_size[0] * grid_size[1], dim, bounds).reshape(grid_size[0], grid_size[1], dim)
    fitness = evaluate_population(population.reshape(-1, dim)).reshape(grid_size[0], grid_size[1])
    best_individual = None
    best_fitness = float('inf')

    for _ in range(num_iterations):
        for i in range(grid_size[0]):
            for j in range(grid_size[1]):
                neighborhood = select_neighborhood((i, j), grid_size)
                parents, parent_fitness = select_parents(neighborhood, population, fitness)
                offspring = apply_crossover_and_mutation(parents, bounds, mutation_rate)
                offspring_fitness = objective_function(offspring)
                if offspring_fitness < fitness[i, j]:
                    population[i, j] = offspring
                    fitness[i, j] = offspring_fitness
                if offspring_fitness < best_fitness:
                    best_fitness = offspring_fitness
                    best_individual = offspring

    return best_individual, best_fitness

# Parameters
grid_size = (10, 10)
dim = 5
bounds = (-5, 5)
num_iterations = 100

# Run the cellular evolutionary algorithm
best_solution, best_solution_fitness = cellular_evolutionary_algorithm(grid_size, dim, bounds, num_iterations)
print("Best solution found:", best_solution)
print("Objective value:", best_solution_fitness)
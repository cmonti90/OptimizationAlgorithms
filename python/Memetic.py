import numpy as np

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def initialize_population(pop_size, dim, bounds):
    return np.random.uniform(bounds[0], bounds[1], (pop_size, dim))

def evaluate_population(population):
    return np.array([objective_function(ind) for ind in population])

def local_search(individual, bounds, num_iterations=10, step_size=0.1):
    best_individual = individual.copy()
    best_fitness = objective_function(best_individual)
    
    for _ in range(num_iterations):
        neighbor = individual + np.random.uniform(-step_size, step_size, individual.shape)
        neighbor = np.clip(neighbor, bounds[0], bounds[1])
        fitness = objective_function(neighbor)
        if fitness < best_fitness:
            best_individual = neighbor
            best_fitness = fitness
            
    return best_individual

def apply_crossover_and_mutation(parents, bounds, mutation_rate=0.1):
    new_population = []
    pop_size, dim = parents.shape
    for _ in range(pop_size // 2):
        parent1, parent2 = parents[np.random.choice(pop_size, 2, replace=False)]
        crossover_point = np.random.randint(1, dim)
        offspring1 = np.concatenate((parent1[:crossover_point], parent2[crossover_point:]))
        offspring2 = np.concatenate((parent2[:crossover_point], parent1[crossover_point:]))
        new_population.extend([offspring1, offspring2])
    new_population = np.array(new_population)
    mutation = np.random.uniform(bounds[0], bounds[1], new_population.shape) * mutation_rate
    new_population += mutation
    new_population = np.clip(new_population, bounds[0], bounds[1])
    return new_population

def memetic_algorithm(pop_size, dim, bounds, num_iterations, mutation_rate=0.1, local_search_iterations=10):
    population = initialize_population(pop_size, dim, bounds)
    fitness = evaluate_population(population)
    
    for _ in range(num_iterations):
        # Apply local search to each individual
        for i in range(pop_size):
            print(f'i = {i}')
            population[i] = local_search(population[i], bounds, local_search_iterations)
        
        # Evaluate fitness after local search
        fitness = evaluate_population(population)
        
        # Selection
        selected_indices = np.argsort(fitness)[:pop_size // 2]
        parents = population[selected_indices]
        
        # Crossover and mutation
        population = apply_crossover_and_mutation(parents, bounds, mutation_rate)
        
        # Evaluate fitness of new population
        fitness = evaluate_population(population)
    
    best_index = np.argmin(fitness)
    return population[best_index], fitness[best_index]

# Parameters
pop_size = 30
dim = 5
bounds = (-5, 5)
num_iterations = 100

# Run the memetic algorithm
best_solution, best_solution_fitness = memetic_algorithm(pop_size, dim, bounds, num_iterations)
print("Best solution found:", best_solution)
print("Objective value:", best_solution_fitness)
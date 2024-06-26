import numpy as np

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def initialize_population(pop_size, dim, bounds):
    return np.random.uniform(bounds[0], bounds[1], (pop_size, dim))

def evaluate_population(population, num_evaluations=5):
    fitness = np.zeros(len(population))
    for i in range(len(population)):
        evaluations = [objective_function(population[i]) for _ in range(num_evaluations)]
        fitness[i] = np.mean(evaluations)  # Fitness averaging
    return fitness

def update_historical_fitness(current_fitness, historical_fitness, alpha=0.1):
    return alpha * current_fitness + (1 - alpha) * historical_fitness

def genetic_algorithm_with_effective_fitness(pop_size, dim, bounds, num_iterations, mutation_rate=0.1, num_evaluations=5):
    population = initialize_population(pop_size, dim, bounds)
    historical_fitness = np.zeros(pop_size)
    
    for _ in range(num_iterations):
        fitness = evaluate_population(population, num_evaluations)
        effective_fitness = update_historical_fitness(fitness, historical_fitness)
        historical_fitness = effective_fitness.copy()

        # Selection
        selected_indices = np.argsort(effective_fitness)[:pop_size // 2]
        parents = population[selected_indices]

        # Crossover and mutation
        new_population = []
        for _ in range(pop_size // 2):
            parent1, parent2 = parents[np.random.choice(len(parents), 2, replace=False)]
            crossover_point = np.random.randint(1, dim)
            offspring1 = np.concatenate((parent1[:crossover_point], parent2[crossover_point:]))
            offspring2 = np.concatenate((parent2[:crossover_point], parent1[crossover_point:]))
            new_population.extend([offspring1, offspring2])
        new_population = np.array(new_population)

        # Mutation
        mutation = np.random.uniform(bounds[0], bounds[1], new_population.shape) * mutation_rate
        new_population += mutation
        new_population = np.clip(new_population, bounds[0], bounds[1])

        population = new_population

    best_index = np.argmin(effective_fitness)
    return population[best_index], effective_fitness[best_index]

# Parameters
pop_size = 30
dim = 5
bounds = (-5, 5)
num_iterations = 100

# Run the genetic algorithm with effective fitness
best_solution, best_solution_fitness = genetic_algorithm_with_effective_fitness(pop_size, dim, bounds, num_iterations)
print("Best solution found:", best_solution)
print("Objective value:", best_solution_fitness)
import numpy as np

def objective_function(x):
    # Example objective function: Multi-modal function
    return -np.sum(np.sin(5 * x)**2, axis=1)
def initialize_population(pop_size, dim, bounds):
    return np.random.uniform(bounds[0], bounds[1], (pop_size, dim))

def evaluate_population(population):
    return objective_function(population)

def fitness_sharing(fitness, population, sigma, alpha=1):
    shared_fitness = np.zeros_like(fitness)

    for i in range(len(population)):
        sh = 0

        for j in range(len(population)):
            d = np.linalg.norm(population[i] - population[j])

            if d < sigma:
                sh += 1 - (d / sigma)**alpha
                shared_fitness[i] = fitness[i] / sh if sh > 0 else fitness[i]

    return shared_fitness

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

def genetic_algorithm_with_fitness_sharing(pop_size, dim, bounds, num_iterations, mutation_rate=0.1, sigma=0.5):
    population = initialize_population(pop_size, dim, bounds)
    fitness = evaluate_population(population)
    
    for _ in range(num_iterations):
        shared_fitness = fitness_sharing(fitness, population, sigma)

        # Selection
        selected_indices = np.argsort(shared_fitness)[-pop_size//2:]
        parents = population[selected_indices]

        # Crossover and mutation
        population = apply_crossover_and_mutation(parents, bounds, mutation_rate)

        # Evaluate fitness
        fitness = evaluate_population(population)
        best_index = np.argmax(fitness)

    return population[best_index], fitness[best_index]

# Parameters
pop_size = 100
dim = 1
bounds = (-2, 2)
num_iterations = 100
# Run the genetic algorithm with fitness sharing
best_solution, best_solution_fitness = genetic_algorithm_with_fitness_sharing(pop_size, dim, bounds, num_iterations)
print("Best solution found:", best_solution)
print("Objective value:", best_solution_fitness)
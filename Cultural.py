import numpy as np

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def initialize_population(pop_size, dim, bounds):
    return np.random.uniform(bounds[0], bounds[1], (pop_size, dim))

def evaluate_population(population):
    return np.array([objective_function(ind) for ind in population])

def update_belief_space(belief_space, population, fitness, acceptance_ratio=0.2):
    # Update situational knowledge with the best solutions
    sorted_indices = np.argsort(fitness)
    num_accepted = int(acceptance_ratio * len(population))
    for i in sorted_indices[:num_accepted]:
        if fitness[i] < belief_space['best_fitness']:
            belief_space['best_individual'] = population[i]
            belief_space['best_fitness'] = fitness[i]
    # Update normative knowledge with bounds based on the best solutions
    for i in sorted_indices[:num_accepted]:
        belief_space['normative_knowledge']['min'] = np.minimum(belief_space['normative_knowledge']['min'], population[i])
        belief_space['normative_knowledge']['max'] = np.maximum(belief_space['normative_knowledge']['max'], population[i])

def influence_population(belief_space, population, influence_ratio=0.5):
    # Modify a portion of the population based on the belief space knowledge
    num_influenced = int(influence_ratio * len(population))
    for i in np.random.choice(len(population), num_influenced, replace=False):
        if np.random.rand() < 0.5:
            population[i] = belief_space['best_individual']
        else:
            population[i] = np.random.uniform(belief_space['normative_knowledge']['min'], belief_space['normative_knowledge']['max'])

def apply_crossover_and_mutation(population, bounds, mutation_rate=0.1):
    new_population = []
    pop_size, dim = population.shape
    for _ in range(pop_size // 2):
        parents = population[np.random.choice(pop_size, 2, replace=False)]
        crossover_point = np.random.randint(1, dim)
        offspring1 = np.concatenate((parents[0, :crossover_point], parents[1, crossover_point:]))
        offspring2 = np.concatenate((parents[1, :crossover_point], parents[0, crossover_point:]))
        new_population.extend([offspring1, offspring2])
    new_population = np.array(new_population)
    mutation = np.random.uniform(bounds[0], bounds[1], new_population.shape) * mutation_rate
    new_population += mutation
    new_population = np.clip(new_population, bounds[0], bounds[1])
    return new_population

def cultural_algorithm(pop_size, dim, bounds, num_iterations, mutation_rate=0.1):
    population = initialize_population(pop_size, dim, bounds)
    fitness = evaluate_population(population)
    
    belief_space = {
        'situational_knowledge': None,
        'normative_knowledge': {
            'min': np.full(dim, np.inf),
            'max': np.full(dim, -np.inf)
        },
        'best_individual': None,
        'best_fitness': float('inf')
    }
    
    for _ in range(num_iterations):
        update_belief_space(belief_space, population, fitness)
        influence_population(belief_space, population)
        population = apply_crossover_and_mutation(population, bounds, mutation_rate)
        fitness = evaluate_population(population)
    
    return belief_space['best_individual'], belief_space['best_fitness']

# Parameters
pop_size = 30
dim = 5
bounds = (-5, 5)
num_iterations = 100

# Run the cultural algorithm
best_solution, best_solution_fitness = cultural_algorithm(pop_size, dim, bounds, num_iterations)
print("Best solution found:", best_solution)
print("Objective value:", best_solution_fitness)
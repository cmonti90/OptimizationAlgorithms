import numpy as np

class Particle:
    def __init__(self, dim, bounds):
        self.position = np.random.uniform(bounds[0], bounds[1], dim)
        self.velocity = np.random.uniform(-1, 1, dim)
        self.best_position = self.position.copy()
        self.best_fitness = float('inf')

class Swarm:
    def __init__(self, pop_size, dim, bounds):
        self.particles = [Particle(dim, bounds) for _ in range(pop_size)]
        self.best_global_position = None
        self.best_global_fitness = float('inf')

def objective_function(x):
    # Example objective function: Sphere function
    return np.sum(x**2)

def update_particle_velocity_position(particle, swarm, global_best_position, w=0.5, c1=1, c2=1):
    inertia_weight = w
    cognitive_weight = c1
    social_weight = c2
    
    r1 = np.random.uniform(0, 1, len(particle.position))
    r2 = np.random.uniform(0, 1, len(particle.position))
    
    particle.velocity = (inertia_weight * particle.velocity +
                         cognitive_weight * r1 * (particle.best_position - particle.position) +
                         social_weight * r2 * (global_best_position - particle.position))
    
    particle.position += particle.velocity
    
    # Clamp position within bounds
    lower_bound, upper_bound = bounds
    particle.position = np.clip(particle.position, lower_bound, upper_bound)

def multi_swarm_pso(num_swarms, pop_size_per_swarm, num_iterations, dim, bounds):
    swarms = [Swarm(pop_size_per_swarm, dim, bounds) for _ in range(num_swarms)]
    
    for _ in range(num_iterations):
        for swarm in swarms:
            for particle in swarm.particles:
                fitness = objective_function(particle.position)
                
                if fitness < particle.best_fitness:
                    particle.best_position = particle.position
                    particle.best_fitness = fitness
                
                if fitness < swarm.best_global_fitness:
                    swarm.best_global_position = particle.position
                    swarm.best_global_fitness = fitness
            
            for particle in swarm.particles:
                update_particle_velocity_position(particle, swarm, swarm.best_global_position)
    
    # Find the best particle among all swarms
    best_global_particle = None
    best_global_fitness = float('inf')
    
    for swarm in swarms:
        for particle in swarm.particles:
            fitness = objective_function(particle.position)
            if fitness < best_global_fitness:
                best_global_fitness = fitness
                best_global_particle = particle
    
    return best_global_particle.position, best_global_fitness
#include "GeneticClustering.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>
#include <iostream>
#include <chrono>

template <typename T>
GeneticClustering<T>::GeneticClustering(size_t pop_size, size_t generations, 
                                      double cross_rate, double mut_rate, size_t clusters)
    : population_size(pop_size), max_generations(generations),
      crossover_rate(cross_rate), mutation_rate(mut_rate), k(clusters),
      max_no_improvement(20), diversity_threshold(0.01), target_fitness(0.95) {
    auto seed = static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()
    );
    rng.seed(seed);


    if constexpr (std::is_integral_v<T>) {
        mutation_op = new IntegerMutation<T>(mutation_rate, 3);
    } else {
        mutation_op = new GaussianMutation<T>(mutation_rate, 0.1);
    } 
}

template <typename T>
void GeneticClustering<T>::set_stop_conditions(size_t max_gen, size_t max_no_imp, 
                                             double diversity_thresh, double target_fit) {
    max_generations = max_gen;
    max_no_improvement = max_no_imp;
    diversity_threshold = diversity_thresh;
    target_fitness = target_fit;
}

template <typename T>
void GeneticClustering<T>::fit(const std::vector<Point<T>>& input_data) {
    data = input_data;
    population.resize(population_size);
    current_generation = 0;
    
    // Population initialize
    for (auto& individual : population) {
        individual.initialize(k, data, rng);
        individual.fitness = compute_fitness(individual);
    }

    size_t no_improvement_count = 0;
    double best_fitness_prev = -std::numeric_limits<double>::infinity();
    
    // Main evolution loop
    for (current_generation = 0; current_generation < max_generations; ++current_generation) {
        std::vector<Individual<T>> new_population;
        
        // Elitism - save the best individual
        auto best_it = std::max_element(population.begin(), population.end(),
            [](const Individual<T>& a, const Individual<T>& b) {
                return a.fitness < b.fitness;
            });
        new_population.push_back(*best_it);

        // Generation of a new generation
        while (new_population.size() < population_size) {
            // Selection
            Individual<T> parent1 = tournament_selection(3);
            Individual<T> parent2 = tournament_selection(3);
            
            // Crossover
            Individual<T> child1, child2;
            if (std::uniform_real_distribution<double>(0.0, 1.0)(rng) < crossover_rate) {
                crossover(parent1, parent2, child1, child2);
            } else {
                child1 = parent1;
                child2 = parent2;
            }
            
            // Mutation
            mutate(child1);
            mutate(child2);
            
            // Fitness assessment
            child1.fitness = compute_fitness(child1);
            child2.fitness = compute_fitness(child2);
            
            // Adding to a new population
            if (new_population.size() < population_size) new_population.push_back(child1);
            if (new_population.size() < population_size) new_population.push_back(child2);
        }
        
        population = new_population;
        
        // Checking the improvement
        double best_current = best_it->fitness;
        if (best_current <= best_fitness_prev + 1e-6) {
            no_improvement_count++;
        } else {
            no_improvement_count = 0;
        }
        best_fitness_prev = best_current;
        
        // Logging
        if (current_generation % 10 == 0) {
            double avg_fitness = std::accumulate(population.begin(), population.end(), 0.0,
                [](double sum, const Individual<T>& ind) { return sum + ind.fitness; }) / static_cast<double>(population_size);
            
            std::cout << "Generation " << current_generation 
                      << ", Best fitness: " << best_current
                      << ", Avg fitness: " << avg_fitness
                      << ", Diversity: " << calculate_diversity() << std::endl;
        }
        
        // Checking the stopping criteria
        if (should_stop(current_generation, no_improvement_count, best_current)) {
            std::cout << "Early stopping at generation " << current_generation << std::endl;
            break;
        }
    }
}

template <typename T>
bool GeneticClustering<T>::should_stop(size_t generation, size_t no_improvement_count, 
                                     double current_best_fitness) const {
    // 1. Maximum generations reached
    if (generation >= max_generations && max_generations != 0) return true;
    
    // 2. No improvements for max_no_improvement generations
    if (no_improvement_count >= max_no_improvement && max_no_improvement != 0) return true;
    
    // 3. The population has converged (low diversity)
    if (is_converged() && diversity_threshold != 0) return true;
    
    // 4. Target fitness achieved
    if (current_best_fitness >= target_fitness && target_fitness != 0) return true;
    
    return false;
}

template <typename T>
bool GeneticClustering<T>::is_converged() const {
    return calculate_diversity() < diversity_threshold;
}

template <typename T>
double GeneticClustering<T>::calculate_diversity() const {
    EuclideanDistance<T> metric;
    double total_distance = 0.0;
    size_t count = 0;
    
    for (size_t i = 0; i < population.size(); ++i) {
        for (size_t j = i + 1; j < population.size(); ++j) {
            for (size_t c = 0; c < k; ++c) {
                total_distance += metric.compute(
                    population[i].centroids[c],
                    population[j].centroids[c]
                );
                count++;
            }
        }
    }
    
    return count > 0 ? total_distance / (double)count : 0.0;
}

template <typename T>
Individual<T> GeneticClustering<T>::get_best_solution() const {
    return *std::max_element(population.begin(), population.end(),
        [](const Individual<T>& a, const Individual<T>& b) {
            return a.fitness < b.fitness;
        });
}

template <typename T>
double GeneticClustering<T>::computeWCSS(const Individual<T>& individual) const {
    double total_error = 0.0;
    EuclideanDistance<T> metric;
    
    for (size_t i = 0; i < data.size(); ++i) {
        size_t cluster_idx = individual.labels[i];
        const Point<T>& centroid = individual.centroids[cluster_idx];
        total_error += metric.compute(data[i], centroid) * metric.compute(data[i], centroid);
    }
    
    return total_error;
}

template <typename T>
double GeneticClustering<T>::getBestWCSS() const {
    const Individual<T>& best = get_best_solution();
    return computeWCSS(best);
}

template <typename T>
double GeneticClustering<T>::compute_fitness(Individual<T>& individual) {
    individual.update_labels(data);
    EuclideanDistance<T> metric;
    double sum_distances = 0.0;
    
    for (size_t i = 0; i < data.size(); ++i) {
        size_t cluster = static_cast<size_t>(individual.labels[i]);
        sum_distances += metric.compute(data[i], individual.centroids[cluster]);
    }
    
    return 1.0 / (1.0 + sum_distances);
}

template <typename T>
Individual<T> GeneticClustering<T>::tournament_selection(size_t tournament_size) {
    std::uniform_int_distribution<size_t> dist(0, population_size - 1);
    Individual<T> best;
    best.fitness = -std::numeric_limits<double>::infinity();
    
    for (size_t i = 0; i < tournament_size; ++i) {
        size_t idx = dist(rng);
        if (population[idx].fitness > best.fitness) {
            best = population[idx];
        }
    }
    return best;
}

template <typename T>
void GeneticClustering<T>::crossover(Individual<T>& parent1, Individual<T>& parent2, 
                                   Individual<T>& child1, Individual<T>& child2) {
    std::uniform_int_distribution<size_t> dist(1, k - 1);
    size_t crossover_point = dist(rng);
    
    child1.centroids.clear();
    child2.centroids.clear();
    
    for (size_t i = 0; i < crossover_point; ++i) {
        child1.centroids.push_back(parent1.centroids[i]);
        child2.centroids.push_back(parent2.centroids[i]);
    }
    
    for (size_t i = crossover_point; i < k; ++i) {
        child1.centroids.push_back(parent2.centroids[i]);
        child2.centroids.push_back(parent1.centroids[i]);
    }
}

template <typename T>
void GeneticClustering<T>::mutate(Individual<T>& individual) {
    for (auto& centroid : individual.centroids) {
        mutation_op->mutate(centroid, rng);
    }
}
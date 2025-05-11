#pragma once
#include <vector>
#include <random>
#include <memory>
#include "Individual.h"
#include "Point.h"
#include "MutationOperator.h"

// Genetic Algorithm for Clustering Problems
// T - numeric type for point coordinates (e.g., double, float, int)
template <typename T>
class GeneticClustering {
private:
    // Core algorithm parameters
    size_t population_size;     // Number of individuals in each generation
    size_t max_generations;     // Maximum number of generations to run
    double crossover_rate;      // Probability of crossover between parents [0,1]
    double mutation_rate;       // Probability of mutating an individual [0,1]
    size_t k;                   // Number of clusters to create

    // Early stopping conditions
    size_t max_no_improvement;  // Max generations without fitness improvement
    double diversity_threshold; // Minimum population diversity threshold
    double target_fitness;      // Target fitness value for early stopping

    // Algorithm state
    std::vector<Point<T>> data; // Input data to be clustered
    std::vector<Individual<T>> population; // Current population of solutions
    std::mt19937 rng;           // Mersenne Twister random number generator

    MutationOperator<T>* mutation_op; // Pointer to mutation strategy

    // Internal helper methods
    double compute_fitness(Individual<T>& individual); // Evaluates solution quality
    Individual<T> tournament_selection(size_t tournament_size); // Selects parents via tournament
    void crossover(Individual<T>& parent1, Individual<T>& parent2, 
                  Individual<T>& child1, Individual<T>& child2); // Creates offspring
    void mutate(Individual<T>& individual); // Applies mutation to an individual
    bool is_converged() const; // Checks if population has converged
    bool should_stop(size_t generation, size_t no_improvement_count, 
                    double current_best_fitness) const; // Determines stopping condition
    double calculate_diversity() const; // Computes population diversity metric

public:
    // Constructor - initializes core parameters
    // Parameters:
    //   pop_size - number of individuals in population
    //   generations - maximum generations to run
    //   cross_rate - crossover probability [0,1]
    //   mut_rate - mutation probability [0,1]
    //   clusters - number of clusters (k)
    GeneticClustering(size_t pop_size, size_t generations, double cross_rate,
                     double mut_rate, size_t clusters);

    // Configures early stopping conditions
    // Parameters:
    //   max_gen - absolute maximum generations
    //   max_no_imp - generations without improvement
    //   diversity_thresh - minimum diversity threshold
    //   target_fit - fitness value to stop at
    void set_stop_conditions(size_t max_gen, size_t max_no_imp,
                           double diversity_thresh, double target_fit);

    // Main training method - runs clustering on input data
    // Parameters:
    //   input_data - dataset to cluster (vector of Points)
    void fit(const std::vector<Point<T>>& input_data);

    // Returns the best solution found
    Individual<T> get_best_solution() const;

    // Get WCSS
    double computeWCSS(const Individual<T>& individual) const;

    // Receving method WCSS the best solution
    double getBestWCSS() const;

    // Monitoring methods
    size_t get_current_generation() const { return current_generation; }
    double get_current_diversity() const { return calculate_diversity(); }

    // Destructor - cleans up mutation operator
    ~GeneticClustering() { delete mutation_op; }

private:
    size_t current_generation = 0; // Tracks current generation number
};
#include <vector>
#include <random>
#include <memory>
#include "Point.h"

// Abstract base class for mutation operators
// T - numeric type of the point coordinates being mutated
template <typename T>
class MutationOperator {
public:
    // Virtual destructor for proper polymorphic deletion
    virtual ~MutationOperator() = default;

    // Pure virtual mutation operation
    // point - reference to the point being mutated
    // rng - random number generator for probabilistic operations
    virtual void mutate(Point<T>& point, std::mt19937& rng) = 0;
};

// Gaussian mutation for real-valued numbers (float/double)
// Applies normally distributed noise to each coordinate
template <typename T>
class GaussianMutation : public MutationOperator<T> {
    double mutation_rate;  // Probability [0,1] that a coordinate gets mutated
    double sigma;          // Standard deviation of Gaussian noise
    
public:
    // Constructor
    // rate - mutation probability per coordinate (0.0 to 1.0)
    // std_dev - standard deviation of Gaussian noise
    GaussianMutation(double rate, double std_dev) 
        : mutation_rate(rate), sigma(std_dev) {}
    
    // Mutates each coordinate with probability mutation_rate
    // Adds Gaussian noise N(0, sigma) to selected coordinates
    void mutate(Point<T>& point, std::mt19937& rng) override;
};

// Uniform integer mutation for discrete values
// Applies uniformly distributed integer changes
template <typename T>
class IntegerMutation : public MutationOperator<T> {
    double mutation_rate;  // Probability [0,1] of mutating each coordinate
    int max_change;       // Maximum absolute change (+/- this value)
    
public:
    // Constructor
    // rate - mutation probability per coordinate (0.0 to 1.0)
    // max - maximum absolute change amount
    IntegerMutation(double rate, int max) 
        : mutation_rate(rate), max_change(max) {}
    
    // Mutates each coordinate with probability mutation_rate
    // Adds uniform random integer in [-max_change, +max_change]
    void mutate(Point<T>& point, std::mt19937& rng) override;
};
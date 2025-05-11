#pragma once  

#include <vector>  
#include "Point.h"  

// Abstract base class for distance metrics
template <typename T>
class DistanceMetric {
public:
    virtual ~DistanceMetric() = default;

    // Pure virtual function to compute distance between two points
    // Parameters:
    //   a - First point (const reference for efficiency)
    //   b - Second point (const reference for efficiency)
    // Returns:
    //   Distance between points as double
    virtual double compute(const Point<T>& a, const Point<T>& b) const = 0;
};

// Derived class implementing Euclidean distance metric
template <typename T>
class EuclideanDistance : public DistanceMetric<T> {
public:
    // Computes Euclidean distance between two points
    // Formula: sqrt(?(a_i - b_i)?)
    // Parameters:
    //   a - First point
    //   b - Second point
    // Returns:
    //   Euclidean distance between points
    // Throws:
    //   std::invalid_argument if points have different dimensions
    double compute(const Point<T>& a, const Point<T>& b) const override;
};
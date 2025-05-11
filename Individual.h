#pragma once
#include <vector>
#include <random>
#include "DistanceMetric.h"
#include "Point.h"

// A templated class representing an individual solution in a clustering algorithm
// T is the numeric type used for point coordinates (e.g., double, float)
template <typename T>
class Individual {
public:
    // Cluster centroids (representative points for each cluster)
    std::vector<Point<T>> centroids;

    // Cluster labels for each data point (indicates which cluster each point belongs to)
    // labels.size() should equal the number of data points
    std::vector<size_t> labels;

    // Fitness value evaluating the quality of this clustering solution
    // Higher fitness typically indicates better clustering
    double fitness;

    // Default constructor
    // Initializes with empty centroids, labels, and zero fitness
    Individual();

    // Initializes the individual with random centroids
    // Parameters:
    //   k - number of clusters to create
    //   data - reference to the dataset being clustered
    //   rng - random number generator for probabilistic operations
    void initialize(size_t k, const std::vector<Point<T>>& data, std::mt19937& rng);

    // Updates cluster labels for all data points based on current centroids
    // (Assigns each point to its nearest centroid)
    // Parameters:
    //   data - reference to the dataset being clustered
    void update_labels(const std::vector<Point<T>>& data);

    // Recalculates centroids based on current cluster assignments
    // (Computes mean of all points in each cluster)
    // Parameters:
    //   data - reference to the dataset being clustered
    void update_centroids(const std::vector<Point<T>>& data);
};
#include "Individual.h"
#include "DistanceMetric.h"
#include <limits>
#include <numeric>

template <typename T>
Individual<T>::Individual() : fitness(-std::numeric_limits<double>::infinity()) {}

template <typename T>
void Individual<T>::initialize(size_t k, const std::vector<Point<T>>& data, std::mt19937& rng) {
    std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
    centroids.clear();
    
    for (size_t i = 0; i < k; ++i) {
        centroids.push_back(data[dist(rng)]);
    }
    update_labels(data);
}

template <typename T>
void Individual<T>::update_labels(const std::vector<Point<T>>& data) {
    labels.resize(data.size());
    EuclideanDistance<T> metric;
    
    for (size_t i = 0; i < data.size(); ++i) {
        double min_dist = std::numeric_limits<double>::max();
        size_t best_cluster = 0;
        
        for (size_t j = 0; j < centroids.size(); ++j) {
            double dist = metric.compute(data[i], centroids[j]);
            if (dist < min_dist) {
                min_dist = dist;
                best_cluster = j;
            }
        }
        labels[i] = best_cluster;
    }
}

template <typename T>
void Individual<T>::update_centroids(const std::vector<Point<T>>& data) {
    std::vector<Point<T>> new_centroids(centroids.size(), std::vector<T>(data[0].coors.size(), 0));
    std::vector<int> counts(centroids.size(), 0);
    
    for (size_t i = 0; i < data.size(); ++i) {
        size_t cluster = static_cast<size_t>(labels[i]);
        for (size_t j = 0; j < data[i].coors.size(); ++j) {
            new_centroids[cluster].coors[j] += data[i].coors[j];
        }
        counts[cluster]++;
    }
    
    for (size_t i = 0; i < new_centroids.size(); ++i) {
        if (counts[i] > 0) {
            for (size_t j = 0; j < new_centroids[i].coors.size(); ++j) {
                new_centroids[i].coors[j] /= static_cast<T>(counts[i]);
            }
        }
    }
    centroids = new_centroids;
}


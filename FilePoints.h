#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Point.h"

// A templated class for handling file operations related to Point data
template <typename T>
class FilePoints {
    public:
        // Read a list of points from txt file
        static std::vector<Point<T>> readFromFile(std::string filename);

        // Write to a txt file
        static void writeToFile(const std::vector<Point<T>> &points, std::string &filename);

        // Write points along with their cluster labels and centroids to a file
        static void writeClustersWithLabels(const std::vector<Point<T>>& points, 
                                      const std::vector<size_t>& labels,
                                      const std::vector<Point<T>> &centroids,
                                      const size_t& num_clusters,
                                      const std::string& filename,
                                      const double WCSS);
};

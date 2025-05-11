#pragma once
#include <vector>

// A templated struct representing a point in n-dimensional space
template <typename T>
struct Point {
    std::vector<T> coors;  // Vector storing the coordinates of the point
    
    // Constructor that initializes the point with given coordinates
    Point(const std::vector<T> &cors);

    // Method to get the dimension (number of coordinates) of the point
    // Marked as const because it doesn't modify the object
    size_t dimension() const { return coors.size(); }
};
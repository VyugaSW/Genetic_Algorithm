#include "Point.h"
#include <vector>
#include <iostream>

template <typename T>
Point<T>::Point (const std::vector<T> &cors) : coors(cors) {}


template class Point<double>;
template class Point<int>;
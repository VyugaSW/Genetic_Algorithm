#include "DistanceMetric.h"
#include <cmath>

template <typename T>
double EuclideanDistance<T>::compute(const Point<T>& a, const Point<T>& b) const {
    double sum = 0.0;
    for (size_t i = 0; i < a.coors.size(); ++i) {
        sum += (a.coors[i] - b.coors[i]) * (a.coors[i] - b.coors[i]);
    }
    return std::sqrt(sum);
}

template class EuclideanDistance<double>;
template class EuclideanDistance<int>;
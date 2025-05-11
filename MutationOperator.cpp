#include "MutationOperator.h"

template <typename T>
void GaussianMutation<T>::mutate(Point<T>& point, std::mt19937& rng) {
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::normal_distribution<double> gauss(0.0, sigma);
    
    for (size_t i = 0; i < point.dimension(); ++i) {
        if (prob(rng) < mutation_rate) {
            point.coors[i] += static_cast<T>(gauss(rng));
        }
    }
}

template <typename T>
void IntegerMutation<T>::mutate(Point<T>& point, std::mt19937& rng) {
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> change(-max_change, max_change);
    
    for (size_t i = 0; i < point.dimension(); ++i) {
        if (prob(rng) < mutation_rate) {
            point.coors[i] += static_cast<T>(change(rng));
        }
    }
}

template class GaussianMutation<double>;
template class IntegerMutation<int>;



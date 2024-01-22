#ifndef UTILS_H
#define UTILS_H
/* 
 * Header to add miscellanius functions or classes
 * */

#include <random>

template <typename RealType>
class Random {
public:
    Random():gen{rd()}{};
    RealType Uniform(RealType a, RealType b) {
        std::uniform_real_distribution<RealType> dis(a, b);
        return dis(gen);
    }

    std::random_device rd;
    std::mt19937 gen; // Standard mersene twister engine seeded with rd
};

#endif

//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_TIMEIT_HPP
#define LAB06_TIMEIT_HPP
#include "Timer.hpp"


typedef int (*add_vec)(const double *, const double *, double *, const int, const int);


class Timeit {
public:
    template <typename T>
    float timeit(add_vec f, AddArgs addArgs);
};


#endif //LAB06_TIMEIT_HPP

//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_UTILITY_HPP
#define LAB06_UTILITY_HPP


class Utility {
    const double expected_result = 1.0f;
public:
    Utility() = default;
    int check_result(const double *c, size_t n);
    int init_vectors(double *h_a, double *h_b, size_t n);
};


#endif //LAB06_UTILITY_HPP

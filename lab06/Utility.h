//
// Created by xgg on 3 Jun 2022.
//

#ifndef LAB06_UTILITY_H
#define LAB06_UTILITY_H


class Utility {
    constexpr static const double expected_result = 1.0f;

    Utility();

public:
    static int check_result(const double *c, size_t n);
    int generate_vectors(double *a, double *b, size_t n);
};


#endif //LAB06_UTILITY_H

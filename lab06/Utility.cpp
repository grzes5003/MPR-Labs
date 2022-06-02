//
// Created by xgg on 3 Jun 2022.
//

#include "Utility.h"

Utility::Utility() {

}

int Utility::check_result(const double *c, size_t n) {
    // Sum up vector c and print result divided by n, this should equal 1 within error
    double sum = 0;
    for (auto i = 0; i < n; ++i)
        sum += c[i];
    if (sum / n == Utility::expected_result) {
        return 0;
    }
    return 1;
}

int Utility::generate_vectors(double *a, double *b, size_t n) {
    return 0;
}

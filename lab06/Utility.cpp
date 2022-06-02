//
// Created by xgg on 3 Jun 2022.
//

#include "Utility.h"
#include <cmath>

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

int Utility::init_vectors(double *h_a, double *h_b, size_t n) {
    int i;
    for (i = 0; i < n; i++) {
        h_a[i] = sin(i) * sin(i);
        h_b[i] = cos(i) * cos(i);
    }
    return 0;
}

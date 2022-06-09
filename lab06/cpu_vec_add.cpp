//
// Created by xgg on 2 Jun 2022.
//

namespace cpu {
    inline int vec_add(const double *h_a, const double *h_b, double *h_c, const int n, const int block_size) {
        for (auto idx = 0; idx < n; ++idx) {
            h_c[idx] = h_a[idx] + h_b[idx];
        }
        return 0;
    }
}
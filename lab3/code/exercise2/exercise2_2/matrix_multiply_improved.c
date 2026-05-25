void matrix_multiply(int *a, int *b, int *output, int i, int k, int j) {
    const int BLOCK = 8;

    for (int ii = 0; ii < i; ii += BLOCK) {
        for (int jj = 0; jj < j; jj += BLOCK) {
            for (int kk = 0; kk < k; kk += BLOCK) {
                for (int x = ii; x < ii + BLOCK && x < i; ++x) {
                    for (int y = jj; y < jj + BLOCK && y < j; ++y) {
                        int sum = 0;
                        for (int z = kk; z < kk + BLOCK && z < k; ++z) {
                            sum += a[x * k + z] * b[z * j + y];
                        }
                        output[x * j + y] += sum;
                    }
                }
            }
        }
    }
}

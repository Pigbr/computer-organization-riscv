void matrix_transpose(int n, int *dst, int *src) {
    const int B = 8;

    for (int i = 0; i < n; i += B) {
        for (int j = 0; j < n; j += B) {

            // 若可完整處理一塊 8×8，則手動展開迴圈
            if (i + B <= n && j + B <= n) {
                for (int r = 0; r < B; ++r) {
                    int r_src_base = (i + r);
                    int v0 = src[r_src_base + (j + 0) * n];
                    int v1 = src[r_src_base + (j + 1) * n];
                    int v2 = src[r_src_base + (j + 2) * n];
                    int v3 = src[r_src_base + (j + 3) * n];
                    int v4 = src[r_src_base + (j + 4) * n];
                    int v5 = src[r_src_base + (j + 5) * n];
                    int v6 = src[r_src_base + (j + 6) * n];
                    int v7 = src[r_src_base + (j + 7) * n];

                    int r_dst_base = r_src_base * n;
                    dst[(j + 0) + r_dst_base] = v0;
                    dst[(j + 1) + r_dst_base] = v1;
                    dst[(j + 2) + r_dst_base] = v2;
                    dst[(j + 3) + r_dst_base] = v3;
                    dst[(j + 4) + r_dst_base] = v4;
                    dst[(j + 5) + r_dst_base] = v5;
                    dst[(j + 6) + r_dst_base] = v6;
                    dst[(j + 7) + r_dst_base] = v7;
                }
            }
            // 邊界處理：不足 8×8 的區塊
            else {
                for (int r = i; r < i + B && r < n; ++r) {
                    for (int c = j; c < j + B && c < n; ++c) {
                        dst[c + r * n] = src[r + c * n];
                    }
                }
            }
        }
    }
}

# Computer Organization — RISC-V Programming & Cache Optimization

計算機組織課程作業集。內容圍繞 RISC-V ISA:從手寫 inline assembly、使用 RISC-V V (Vector) Extension 做向量化加速,到修改 Spike 模擬器實作 FIFO cache replacement policy 與 cache-aware 程式優化。

- Course: Computer Organization 2025
- Toolchain: `riscv64-unknown-linux-gnu-gcc`, `riscv-isa-sim` (Spike), `pk` (proxy kernel), `local-judge`

---

## Tech Stack

- RISC-V Assembly (RV64G + RVV / V extension)
- GCC Inline Assembly(`asm volatile` 配 extended operand constraints)
- Spike RISC-V ISA simulator(含修改其原始碼)
- C / C++(Spike 是 C++ 寫的)
- Performance modeling:cycle count、CPI、CPU-bound vs Memory-bound 分析

---

## Labs

### Lab 1 — RISC-V Inline Assembly Basics

用 inline assembly 實作三種演算法,熟悉 RISC-V ISA 與 C/Assembly 的介面:

| 題目 | 重點技術 |
|---|---|
| `array_sort.c` | Bubble sort,用 `lw / sw / blt` 配 GCC operand constraints |
| `array_search.c` | Linear search,實作分支與條件跳躍 |
| `linked_list_sort.c` | Linked list merge sort(分割 / 合併 / 走訪三個 block 都用 asm) |

詳細實作說明見 [`lab1/code/README.md`](./lab1/code/README.md)

### Lab 2 — FFT & Vector Extension (RVV)

實作 Cooley-Tukey radix-2 DIF FFT,並對比 V extension 帶來的加速。同時做 performance modeling:插入 instruction counter,計算總 cycle、CPU time、判斷 program 屬於 CPU-bound 或 Memory-bound。

- `complex_add.c / complex_sub.c / complex_mul.c` — float complex 運算
- `bit_reverse.c / log2.c / pi.c` — FFT bit-reverse、log2、Leibniz π 級數
- `arraymul_baseline.c` — 純量版本陣列乘加
- `arraymul_improved.c` — 用 `vsetvli` + `vle32.v / vfmul.vv / vfadd.vf` 做向量化,實測加速超過 2 倍
- `arraymul_float.c / arraymul_double.c` — 單/雙精度比較

詳細實作說明見 [`lab2/code/README.md`](./lab2/code/README.md)

### Lab 3 — Cache Simulator & Cache-aware Optimization

分兩部分。

(A) 改 Spike 原始碼,實作 FIFO cache replacement policy:

- 修改 `riscv/cachesim.h` 與 `riscv/cachesim.cc`
- 用 `std::deque` 記錄每個 set 內 cache line 的進入順序
- Set-associative (`cache_sim_t`) 與 fully-associative (`fa_cache_sim_t`) 兩種都實作

(B) 用 loop tiling 重寫 matrix transpose / multiply:

針對 cache 規格(4-way / 8-set / 32B line)挑 block size = 8 x 8,讓單一 cacheline 內的資料一次處理完,大幅降低 conflict miss。

- Miss rate:6.1% → 2.7%(transpose)、7.5% → 0.5%(multiply)
- Memory access overhead 改善約 1.7 倍與 3.5 倍

詳細實作說明見 [`lab3/code/README.md`](./lab3/code/README.md)

---

## Repo Layout

```
.
├── lab1/code/      # Inline assembly — sort / search / merge sort
├── lab2/code/      # FFT + RVV vectorization + perf modeling
└── lab3/code/      # FIFO cache + tiled matrix ops
```

---

## What I Learned

- 看懂 GCC extended inline assembly 的 operand constraints(`"+r"`, `"=f"`, clobber list)
- RISC-V vector programming(stripmining、`vsetvli`)
- 讀懂並修改 Spike 原始碼、重新編譯 toolchain
- Cache 行為對效能的實際影響,以及怎麼用 loop tiling / loop interchange 改善

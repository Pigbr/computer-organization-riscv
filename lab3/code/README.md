---
title: README

---

# Exercise 1
## step1
去~/riscv/riscv-isa-sim/riscv裡面的cachesim.h和cachesim.cc並修改。
![image](https://hackmd.io/_uploads/Hye8_xxmxx.png)
## step2
修改cachesim.h
因為有使用dequeue來實作FIFO cache
```cpp
#inculde <deque>
```
class cache_sim_t{}裡面多創建一個變數
```cpp
std::vector<std::deque<size_t>> fifo_queues;
```
修改cachesim.cpp
void cache_sim_t::init()初始化
```cpp
  fifo_queues.resize(sets);
	for (size_t i = 0; i < sets; ++i) {
	  for (size_t j = 0; j < ways; ++j) {
	    fifo_queues[i].push_back(j);
	  }
	}
// 依題目來說cache(4ways 8sets)會是
[[0, 1, 2, 3],
 [0, 1, 2, 3],
 [0, 1, 2, 3],
 [0, 1, 2, 3],
 [0, 1, 2, 3],
 [0, 1, 2, 3],
 [0, 1, 2, 3],
 [0, 1, 2, 3]]
```
uint64_t cache_sim_t::victimize(uint64_t addr)實作
```cpp
uint64_t cache_sim_t::victimize(uint64_t addr)
{
  size_t idx = (addr >> idx_shift) & (sets - 1);
  size_t way = fifo_queues[idx].front();   // 去對應的index尋找那一個最早進來
  fifo_queues[idx].pop_front();            // 移除它
  fifo_queues[idx].push_back(way);         // 加回隊尾 (因為新進資料是最晚進來)
// 0, 1, 2, 3
// 變成
// 1, 2, 3, 0
  uint64_t victim = tags[idx * ways + way]; //取出 victim 資料
  tags[idx * ways + way] = (addr >> idx_shift) | VALID; //將新進資料加上VALID bit放進去
  return victim;
}
```
uint64_t fa_cache_sim_t::victimize(uint64_t addr)
```cpp
uint64_t fa_cache_sim_t::victimize(uint64_t addr)
{
  uint64_t new_tag = (addr >> idx_shift);

  uint64_t old_tag = 0;
  if (tags.size() == ways)  //如果滿了就把最早進來的拿掉
  {
    uint64_t victim_tag = fifo_queue.front();
    fifo_queue.pop_front();
    old_tag = tags[victim_tag];
    tags.erase(victim_tag);
  }
 

  tags[new_tag] = new_tag | VALID;   //把新進資料加上VALID bits放入
  fifo_queue.push_back(new_tag);     //紀錄進入順序

  return old_tag;
}

```
## step3
重新編譯
```shell
cd ~/riscv/riscv-isa-sim/build
make clean
../configure --prefix=$RISCV
make
sudo make install
```
查看是否成功
```shell
cd /opt/riscv/bin
ls -l
```
![image](https://hackmd.io/_uploads/BysUVbgmle.png)
## step4
用第二題測試
![image](https://hackmd.io/_uploads/HJFsV-xmex.png)
變低了
![image](https://hackmd.io/_uploads/S1Lc4ZlXxe.png)
# Exercise2
## matrix_transpose_improved
```clike
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
```
## 轉置方式
![image](https://hackmd.io/_uploads/S16oH7bXee.png)
先把同一個cacheline的資料轉置，這樣能大幅減少confict miss的數量。
## block=8原因
題目cache 設計
 4 ways
 8 sets
 32-byte cacheline
 1 data 64bits
 
一個cacheline可以放4個數字且4ways，src 和 dst 各兩個 4x2=8
然後有8個sets，所以為8x8的block
## matrix_multiply_improved
```clike
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
```
## 乘法方式
![image](https://hackmd.io/_uploads/SJeUuQZQgl.png)
跟轉置概念很像就盡量同一個cacheline的資料先做，這樣能大幅減少confict miss的數量。
## block=8原因
因為題目沒講的很清楚，我不知道cache要放幾個MATRIX。
先假設只要放a和b兩個矩陣那就跟第一題一樣，但如果要加output我覺得block=4才對，  
但我試過很多種組合發現block=8最好。

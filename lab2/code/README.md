---
title: README
---

# HW2 心得

我都用 size=4。

## 1. Fast Fourier Transform Calculation

### complex_add.c

```
"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
"fadd.s %[C_Re], %[A_Re], %[B_Re] \n\t"
"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
"fadd.s %[C_Im], %[A_Im], %[B_Im] \n\t"
```

就實部相加放進 `result.Re`，虛部放進 `result.Im`，然後 `fadd_cnt + 2`。

### complex_sub.c

```
"addi %[fsub_cnt], %[fsub_cnt], 1\n\t"
"fsub.s %[C_Re], %[A_Re], %[B_Re] \n\t"
"addi %[fsub_cnt], %[fsub_cnt], 1\n\t"
"fsub.s %[C_Im], %[A_Im], %[B_Im] \n\t"
```

就實部相減放進 `result.Re`，虛部放進 `result.Im`，然後 `fsub_cnt + 2`。

### complex_mul.c

```
"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
"fmul.s f1, %[A_Re], %[B_Re] \n\t"


"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
"fmul.s f2, %[A_Im], %[B_Im] \n\t"


"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
"fmul.s f3, %[A_Re], %[B_Im] \n\t"


"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
"fmul.s f4, %[A_Im], %[B_Re] \n\t"

"addi %[fsub_cnt], %[fsub_cnt], 1\n\t"
"fsub.s %[C_Re], f1, f2 \n\t"


"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
"fadd.s %[C_Im], f3, f4 \n\t"
```

(a+bi)×(c+di)=(ac−bd)+(ad+bc)i
實部
f1 <- a x c
f2 <- b x d
虛部
f3 <- a x d
f4 <- b x c
最後放進 result

### log2.c

```
"add t0 ,x0,x0 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"

"addi %[add_cnt], %[add_cnt], 1\n\t"
"add t1, %[N], x0 \n\t"


"loop_log2: \n\t"

"addi %[others_cnt], %[others_cnt], 1\n\t"
"beqz t1, end \n\t"

"addi %[others_cnt], %[others_cnt], 1\n\t"
"srli t1, t1, 1 \n\t"

"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t0, t0, 1 \n\t"

"addi %[others_cnt], %[others_cnt], 1\n\t"
"j loop_log2 \n\t"

"end: \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi %[log], t0 , -1 \n\t"
```

```
ex          loop1         loop2       loop3     loop4      0000=0 跳出
t0=0          1             2           3         4         4-1=3   return3
t1=1000     0100          0010         0001      0000       log₂8 = 3
```

只能算取完 log₂ 整數。

### bit_reverse.c

```
"add t0,x0 ,x0 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"add t1 , %[m], x0 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"add t2, x0 , x0 \n\t"

"reverse_loop: \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"and t0, %[b], 1 \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"slli t2, t2, 1 \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"or t2, t2, t0 \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"srli %[b], %[b], 1 \n\t"
"addi t1, t1, -1 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"bnez t1, reverse_loop \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"add %[b], t2 , x0 \n\t"
```

```
ex               loop1     loop2    loop3     t1=0 跳出
b(0~7)=001        000       000      000
t0=0               1         0        0        reslut=100(4)
t1=3               2         1        0        return 4
t2=0              001       010      100
```

### pi.c

```
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t1, x0 , 1 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t2, x0 , 3 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t3, x0 , -1 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t4 , %[N] , -1 \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"fcvt.s.w f1, t1 \n\t"
"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
"fadd.s %[pi], %[pi], f1 \n\t"

"loop: \n\t"

"addi %[others_cnt], %[others_cnt], 1\n\t"
"fcvt.s.w f2, t2 \n\t"
"addi %[fdiv_cnt], %[fdiv_cnt], 1\n\t"
"fdiv.s f2, f1, f2 \n\t"

"addi %[sub_cnt], %[sub_cnt], 1\n\t"
"bltz t3, sub \n\t"
"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
"fadd.s %[pi], %[pi], f2 \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"j next \n\t"

"sub: \n\t"
"addi %[fsub_cnt], %[fsub_cnt], 1\n\t"
"fsub.s %[pi], %[pi], f2 \n\t"

"next: \n\t"
"addi %[sub_cnt], %[sub_cnt], 1\n\t"
"sub t3,x0 , t3 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t2, t2, 2 \n\t"
"addi %[add_cnt], %[add_cnt], 1\n\t"
"addi t4, t4, -1 \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
"bnez t4, loop \n\t"
```

```
ex             loop1             loop2           ............    跑1000次跳出
t1=1          f2=1/3             f2=1/5
f1=1.0        t3=-1,跳到sub       t3=1,add
t2=3          %[pi]=1-1/3        %[pi]=1-1/3+1/5
f2=3.0                                                        return3.140593
t3=-1         t2=t2+2            t2=t2+2
t4=999        t3=-t3             t3=-t3
%[pi]=1       t4=t4-1            t4=t4-1
```

### macro_define.h

```
// exerxise1
#define macro_fft_cycle_count fft_cycle_count = add_cnt * add_CPI +\
                                                sub_cnt * sub_CPI +\
                                                mul_cnt * mul_cnt +\
                                                div_cnt * div_CPI +\
                                                lw_cnt * lw_CPI +\
                                                sw_cnt * sw_CPI +\
                                                fadd_cnt * fadd_CPI +\
                                                fsub_cnt * fsub_CPI +\
                                                fmul_cnt * fmul_CPI +\
                                                fdiv_cnt * fdiv_CPI +\
                                                others_cnt * others_CPI;

#define macro_fft_cpu_time fft_cpu_time = fft_cycle_count * cycle_time;

#define macro_calc_fft_ratio fft_ratio = 1.0 - ((float)(lw_cnt * lw_CPI + sw_cnt * sw_CPI)) / fft_cycle_count;
```

這部分就把有用到的指令放進去,在計算是否為 cpu-bound I/O-bound。

## 2-1. Array Multiplication without V Extension

### arraymul_baseline.c

```
"arraymul_baseline_loop: \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"beq %[arr_size], x0, arraymul_baseline_end \n\t"

"flw f0, 0(%[h]) \n\t"
"addi %[flw_cnt], %[flw_cnt], 1 \n\t"
"flw f1, 0(%[x]) \n\t"
"addi %[flw_cnt], %[flw_cnt], 1 \n\t"
"fmul.s f0, f0, f1 \n\t"
"addi %[fmul_cnt], %[fmul_cnt], 1 \n\t"
"fadd.s f0, f0, %[id] \n\t"
"addi %[fadd_cnt], %[fadd_cnt], 1 \n\t"
"fsw f0, 0(%[y]) \n\t"
"addi %[fsw_cnt], %[fsw_cnt], 1 \n\t"

"addi %[h], %[h], 4 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[x], %[x], 4 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[y], %[y], 4 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[arr_size], %[arr_size], -1 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"

"addi %[others_cnt], %[others_cnt], 1 \n\t"
"j arraymul_baseline_loop \n\t"

"arraymul_baseline_end: \n\t"
```

```
ex
    loop1                       loop2                           跑完8次
%[arr_Size]=8                 %[arr_Size]=7
f0 = h[0]        下一個4bytes  f0 = h[1]          下一個4bytes
f1 = x[0]                     f1 = x[1]
y[0]=h[0]+x[0]+id             y[1]=h[1]+x[1]+id

```

### macro_define.h

```
// exercise2_1 && exercise3_1
#define macro_arraymul_baseline_cycle_count arraymul_baseline_cycle_count = add_cnt * add_CPI +\
                                                                            fadd_cnt * fadd_CPI +\
                                                                            fmul_cnt * fmul_CPI +\
                                                                            flw_cnt * flw_CPI +\
                                                                            fsw_cnt * fsw_CPI +\
                                                                            others_cnt * others_CPI;

#define macro_arraymul_baseline_cpu_time arraymul_baseline_cpu_time =arraymul_baseline_cycle_count * cycle_time;

#define macro_calc_arraymul_baseline_ratio arraymul_baseline_ratio = 1.0 - ((float)(flw_cnt * flw_CPI + fsw_cnt * fsw_CPI)) / arraymul_baseline_cycle_count;
```

這部分就把有用到的指令放進去,在計算是否為 cpu-bound I/O-bound。

## 2-2. Array Multiplication with V Extension

### arraymul_improved.c

```
"vsetvli t0, %[arr_size], e32, m1 \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"vle32.v v0, (%[h]) \n\t"
"addi %[lw_cnt], %[lw_cnt], 1 \n\t"
"vle32.v v1, (%[x]) \n\t"
"addi %[lw_cnt], %[lw_cnt], 1 \n\t"
"vfmul.vv v2, v0, v1 \n\t"
"addi %[fmul_cnt], %[fmul_cnt], 1 \n\t"
"vfadd.vf v2, v2, %[id] \n\t"
"addi %[fadd_cnt], %[fadd_cnt], 1 \n\t"
"vse32.v v2, (%[y]) \n\t"
"addi %[sw_cnt], %[sw_cnt], 1 \n\t"
```

先設定好元素長度和陣列長度，然後透過 vle 放進 v0,v2，再透過 vmul 相乘和 vadd 相加。

### macro_define.c

```
// exercise2_2
#define macro_arraymul_vector_cycle_count arraymul_vector_cycle_count = add_cnt * add_CPI +\
                                                                        sub_cnt * sub_CPI +\
                                                                        mul_cnt * mul_CPI +\
                                                                        div_cnt * div_CPI +\
                                                                        lw_cnt * lw_CPI +\
                                                                        sw_cnt * sw_CPI +\
                                                                        fadd_cnt * fadd_CPI +\
                                                                        fmul_cnt * fmul_CPI +\
                                                                        others_cnt * others_CPI;

#define macro_arraymul_vector_cpu_time arraymul_vector_cpu_time = arraymul_vector_cycle_count * cycle_time;
```

這部分就把有用到的指令放進去,在計算是否為 cpu-bound I/O-bound。

## 3-1. Single Floating-point Multiplication

### arraumul_float.c

```
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"add t0, x0, x0 \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"fmv.s f3, %[result] \n\t"

"arraymul_float_loop: \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"beq %[arr_size],t0, arraymul_float_end \n\t"

"flw f0, 0(%[h]) \n\t"
"addi %[flw_cnt], %[flw_cnt], 1 \n\t"
"flw f1, 0(%[x]) \n\t"
"addi %[flw_cnt], %[flw_cnt], 1 \n\t"
"fmul.s f2, f0, f1 \n\t"
"addi %[fmul_cnt], %[fmul_cnt], 1 \n\t"
"fmul.s f3, f3, f2 \n\t"
"addi %[fmul_cnt], %[fmul_cnt], 1 \n\t"

"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[h], %[h], 4 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[x], %[x], 4 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi t0, t0, 1 \n\t"

"addi %[others_cnt], %[others_cnt], 1 \n\t"
"j arraymul_float_loop \n\t"

"arraymul_float_end: \n\t"

"fmv.s %[result], f3 \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
```

先透過 flw 載入
就透過 fmul.s 相乘,result=result*h[i]*x[i]一直乘下去

### macro_define.h

```
// exercise2_1 && exercise3_1
#define macro_arraymul_baseline_cycle_count arraymul_baseline_cycle_count = add_cnt * add_CPI +\
                                                                            fadd_cnt * fadd_CPI +\
                                                                            fmul_cnt * fmul_CPI +\
                                                                            flw_cnt * flw_CPI +\
                                                                            fsw_cnt * fsw_CPI +\
                                                                            others_cnt * others_CPI;

#define macro_arraymul_baseline_cpu_time arraymul_baseline_cpu_time =arraymul_baseline_cycle_count * cycle_time;

#define macro_calc_arraymul_baseline_ratio arraymul_baseline_ratio = 1.0 - ((float)(flw_cnt * flw_CPI + fsw_cnt * fsw_CPI)) / arraymul_baseline_cycle_count;
```

這部分就把有用到的指令放進去,在計算是否為 cpu-bound I/O-bound。

## 3-2. Double Floating-point Multipication

### arraumul_double.c

```
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"add t0, x0, x0 \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"fmv.d f3, %[result] \n\t"


"arraymul_double_loop: \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"beq %[arr_size],t0, arraymul_double_end \n\t"

"fld f0, 0(%[h]) \n\t"
"addi %[dlw_cnt], %[dlw_cnt], 1 \n\t"
"fld f1, 0(%[x]) \n\t"
"addi %[dlw_cnt], %[dlw_cnt], 1 \n\t"
"fmul.d f2, f0, f1 \n\t"
"addi %[dmul_cnt], %[dmul_cnt], 1 \n\t"
"fmul.d f3, f3, f2 \n\t"
"addi %[dmul_cnt], %[dmul_cnt], 1 \n\t"

"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[h], %[h], 8 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi %[x], %[x], 8 \n\t"
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi t0, t0, 1 \n\t"

"addi %[others_cnt], %[others_cnt], 1 \n\t"
"j arraymul_double_loop \n\t"

"arraymul_double_end: \n\t"
"fmv.d %[result], f3 \n\t"
"addi %[others_cnt], %[others_cnt], 1 \n\t"
```

先透過 fld 載入
就透過 fmul.d 相乘,result=result*h[i]*x[i]一直乘下去

### macro_define.h

```
// exercise3_2
#define macro_calc_arraymul_double_ratio arraymul_baseline_ratio = 1.0 - ((float)(dlw_cnt * dlw_CPI + dsw_cnt * dsw_CPI)) / arraymul_double_cycle_count;

#define macro_arraymul_double_cycle_count arraymul_double_cycle_count = add_cnt * add_CPI +\
                                                                        dadd_cnt * dadd_CPI +\
                                                                        dmul_cnt * dmul_CPI +\
                                                                        dlw_cnt * dlw_CPI +\
                                                                        dsw_cnt * dsw_CPI +\
                                                                        others_cnt * others_CPI;

#define macro_arraymul_double_cpu_time arraymul_double_cpu_time = arraymul_double_cycle_count * cycle_time;
```

這部分就把有用到的指令放進去,在計算是否為 cpu-bound I/O-bound。

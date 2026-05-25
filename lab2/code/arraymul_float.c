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
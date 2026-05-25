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
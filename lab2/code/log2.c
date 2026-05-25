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
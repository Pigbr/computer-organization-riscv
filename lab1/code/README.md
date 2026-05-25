---
title: README

---

# HW1  作業心得
## array_sort.c
```
asm volatile(
        "lw t0, 0(%0)\n\t"
        "lw t1, 4(%0)\n\t"
        "blt t0, t1, 1f\n\t"
        "sw t0, 4(%0)\n\t"
        "sw t1, 0(%0)\n\t"
        "1:\n\t"
        :
        : "r" (p_a+j)
        : "t0","t1"
        );
```
先比大小如果比下一個元素大，將自己與下一個元素交換。
## array_search.c
reuslt = -1
```
 asm volatile(
        "1:\n\t"
        "addi %0, %0 ,1\n\t"
        "bge %0, %1, 2f\n\t"
        "lw t0, 0(%3)\n\t"
        "beq t0, %2 , 3f\n\t"
        "addi %3, %3, 4\n\t"
        "j 1b\n\t"
        "2:\n\t"
        "addi %0, x0,-1\n\t"
        "3:\n\t"    
        : "+r" (result)
        : "r" (arr_size), "r" (target), "r" (p_a)
        : "t0"
        );

    return result;
}
```
先設result=0在與arr_size比較
小於:矩陣還有元素lw出來比較，找到結束。
大於:return -1。
## linked_list_sort.c
### BLOCK A splitlist
```
asm volatile(
        "mv t0, %2\n\t" 
        "mv t1, %2\n\t" 
        "loop:\n\t"
        "ld t0, 8(t0)\n\t" 
        "beqz t0, end\n\t" 
        "ld t0, 8(t0)\n\t" 
        "beqz t0, end\n\t"
        "ld t1, 8(t1)\n\t" 
        "j loop\n\t" 
        "end:\n\t" 
        "mv t2, t1\n\t" 
        "ld t1, 8(t1)\n\t" 
        "sd zero , 8(t2)\n\t" 
        "mv %0, %2\n\t"
        "mv %1, t1\n\t" 
        :"=r" (*firstHalf), "=r" (*secondHalf)
        :"r" (head)
        :"t0", "t1","t2"
        );
```
使用快慢指標進行分割，fast與slow指到head，維持fast走兩步slow走一步等到fast->next與fast->next->next是null，將slow->next=null再回傳。
### BLOCK B mergesortedLists
```
 asm volatile(
        "ld t0, 0(%2)\n\t" 
        "ld t1, 0(%3)\n\t"
        "blt t1,t0 ,b_less_a\n\t" 
        "mv %0, %2\n\t" 
        "mv %1, %2\n\t" 
        "ld %2, 8(%2)\n\t" 
        "j merge_loop\n\t" 
        "b_less_a:\n\t"
        "mv %0, %3\n\t" 
        "mv %1, %3\n\t" 
        "ld %3, 8(%3)\n\t" 
        "merge_loop:\n\t"
        "beqz %2, a_null\n\t" 
        "beqz %3, b_null\n\t" 
        "ld t0, 0(%2)\n\t" 
        "ld t1, 0(%3)\n\t" 
        "blt t1, t0, b_less_a_2\n\t" 
        "sd %2, 8(%1)\n\t" 
        "mv %1, %2\n\t" 
        "ld %2, 8(%2)\n\t" 
        "j merge_loop\n\t" 
        "b_less_a_2:\n\t"
        "sd %3, 8(%1)\n\t" 
        "mv %1, %3\n\t" 
        "ld %3, 8(%3)\n\t" 
        "j merge_loop\n\t" 
        "a_null:\n\t"
        "sd %3, 8(%1)\n\t" 
        "j end_merge\n\t" 
        "b_null:\n\t"
        "sd %2, 8(%1)\n\t" 
        "end_merge:\n\t"
        :"=r" (result), "=r" (tail),"+r" (a), "+r" (b)
        :
        :"t0", "t1"
        );
```
step1
a->data 與b->data 比大小，小的位址給result,tail然後a or b指到下一個節點。
step2
判斷 a,b是否null?     yes to step4 ||  no to step3
a->data 與b->data 比大小 小的給tail->next然後a or b指到下一個節點。 to step2
step4
把剩下的放在tail->next 後面
### BLOCK C next->node
```
 asm volatile(
            "mv %[cur], %[next]\n\t"    
            : [cur] "=r" (cur)
            : [next] "r" (cur->next)
            );
```
將cur->next的內容給cur。
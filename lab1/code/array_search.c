#include <stdio.h>

int arraySearch(int *p_a, int arr_size, int target)
{
    int result = -1;

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

// Main function to test the implementation
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    int arr_size;
    fscanf(input, "%d", &arr_size);
    int arr[arr_size];

    // Read integers from input file into the array
    for (int i = 0; i < arr_size; i++) {
        int data;
        fscanf(input, "%d", &data);
        arr[i] = data;
    }
    int target;
    fscanf(input, "%d", &target);
    fclose(input);

    int *p_a = &arr[0];

    int index = arraySearch(p_a, arr_size, target);

    // Print the result
    printf("%d ", index);
    printf("\n");

    return 0;
}

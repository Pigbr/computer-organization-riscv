#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Split the linked list into two parts
void splitList(Node *head, Node **firstHalf, Node **secondHalf) {

    asm volatile(
        "mv t0, %2\n\t" // Move head to firstHalf
        "mv t1, %2\n\t" // Move head to secondHalf
        "loop:\n\t"
        "ld t0, 8(t0)\n\t" // Load the next node of head into t0
        "beqz t0, end\n\t" // If t0 is NULL, jump to label 1
        "ld t0, 8(t0)\n\t" // Load the next node of head into t1
        "beqz t0, end\n\t" // If t0 is NULL, jump to label 1
        "ld t1, 8(t1)\n\t" // Load the next node of head into t1
        "j loop\n\t" // Jump to loop
        "end:\n\t" // Label 1
        "mv t2, t1\n\t" // Move t1 to t2
        "ld t1, 8(t1)\n\t" // Load the next node of t2 into t1
        "sd zero , 8(t2)\n\t" // Set the next pointer of t2 to NULL
        "mv %0, %2\n\t" // Move t0 to firstHalf
        "mv %1, t1\n\t" // Move t2 to secondHalf
        :"=r" (*firstHalf), "=r" (*secondHalf)
        :"r" (head)
        :"t0", "t1","t2"
    );
}

// Merge two sorted linked lists
Node *mergeSortedLists(Node *a, Node *b) {
    Node *result = NULL;
    Node *tail = NULL;

    asm volatile(
        "ld t0, 0(%2)\n\t" 
        "ld t1, 0(%3)\n\t"
        "blt t1,t0 ,b_less_a\n\t" // If b < a, go to b_less_a
        "mv %0, %2\n\t" // result = a
        "mv %1, %2\n\t" // tail = a
        "ld %2, 8(%2)\n\t" // Move to the next node of a
        "j merge_loop\n\t" // Jump to merge_loop
        "b_less_a:\n\t"
        "mv %0, %3\n\t" // result = b
        "mv %1, %3\n\t" // tail = b
        "ld %3, 8(%3)\n\t" // Move to the next node of b
        "merge_loop:\n\t"
        "beqz %2, a_null\n\t" // If a is NULL, jump to end_merge
        "beqz %3, b_null\n\t" // If b is NULL, jump to end_merge
        "ld t0, 0(%2)\n\t" // Load a->data into t0
        "ld t1, 0(%3)\n\t" // Load b->data into t1
        "blt t1, t0, b_less_a_2\n\t" // If b < a, go to b_less_a
        "sd %2, 8(%1)\n\t" // tail->next = a
        "mv %1, %2\n\t" // tail = a
        "ld %2, 8(%2)\n\t" // Move to the next node of a
        "j merge_loop\n\t" // Jump to merge_loop
        "b_less_a_2:\n\t"
        "sd %3, 8(%1)\n\t" // tail->next = b
        "mv %1, %3\n\t" // tail = b
        "ld %3, 8(%3)\n\t" // Move to the next node of b
        "j merge_loop\n\t" // Jump to merge_loop
        "a_null:\n\t"
        "sd %3, 8(%1)\n\t" // tail->next = b
        "j end_merge\n\t" // Jump to end_merge
        "b_null:\n\t"
        "sd %2, 8(%1)\n\t" // tail->next = a
        "end_merge:\n\t"
        :"=r" (result), "=r" (tail),"+r" (a), "+r" (b)
        :
        :"t0", "t1"
        );

    return result;
}

// Merge Sort function for linked list
Node *mergeSort(Node *head) {
    if (!head || !head->next)
        return head; // Return directly if there is only one node

    Node *firstHalf, *secondHalf;
    splitList(head, &firstHalf, &secondHalf); // Split the list into two sublists
   
    firstHalf = mergeSort(firstHalf);   // Recursively sort the left half
    secondHalf = mergeSort(secondHalf); // Recursively sort the right half

    return mergeSortedLists(firstHalf, secondHalf); // Merge the sorted sublists
}

// Function to print the linked list
void printList(Node *head) {
    Node *cur = head;
    while (cur) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }

    int list_size;
    fscanf(input, "%d", &list_size);
    Node *head = (list_size > 0) ? (Node *)malloc(sizeof(Node)) : NULL;
    Node *cur = head;
    for (int i = 0; i < list_size; i++) {
        fscanf(input, "%d", &(cur->data));
        if (i + 1 < list_size)
            cur->next = (Node *)malloc(sizeof(Node));
        cur = cur->next;
    }
    fclose(input);

    // Linked list sort
    head = mergeSort(head);

    // Print the sorted linked list
    cur = head;
    while (cur) {
        printf("%d ", cur->data);
        asm volatile(
            // Move to the next node
            "mv %[cur], %[next]\n\t"              // Move cur to next node
            : [cur] "=r" (cur)
            : [next] "r" (cur->next)
        );
    }
    printf("\n");

    return 0;
}
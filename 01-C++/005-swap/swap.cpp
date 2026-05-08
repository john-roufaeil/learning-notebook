#include "swap.h"

void swap_temp(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_addition(int *a, int *b) {
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void swap_multiplication(int *a, int *b) {
    *a = *a * *b;
    *b = *a / *b;
    *a = *a / *b;
}

void swap_xor(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}
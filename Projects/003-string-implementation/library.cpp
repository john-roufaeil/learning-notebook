#include <iostream>
#include "library.h"

void print_divider() {
    std::cout << "----------------------------------------" << std::endl;
}

int str_len(char inputStr[]) {
    int length = 0;
    while (inputStr[length] != '\0') {
        length++;
    }
    return length;
}

int str_cmp(char str1[], char str2[]) {
    int str1_len = str_len(str1);
    int str2_len = str_len(str2);
    int str1_sum = 0;
    int str2_sum = 0;

    for (int i = 0; i < str1_len; i++) {
    	str1_sum += str1[i];
    }

    for (int i = 0; i < str2_len; i++) {
    	str2_sum += *(str2+i);
    }

    if (str1_len > str2_len) return  1;
    if (str2_len > str1_len) return -1;
    if (str1_sum > str2_sum) return  1;
    if (str2_sum > str1_sum) return -1;

    return 0;
}

int is_alpha(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) return 1;
    return 0;
}

int is_lower(char c) {
    if (c >= 97 && c <= 122) return 1;
    return 0;
}

void to_lowercase(char str[]) {
    int len = str_len(str);
    char c = '\0';
    for (int i = 0; i < len; i++)
    {
        c = *(str + i);
        if (is_alpha(c) && !is_lower(c))
            *(str+i) = *(str+i) + 32;
    }
}

void to_uppercase(char str[]) {
    int len = str_len(str);
    char c = '\0';
    for (int i = 0; i < len; i++)
    {
        c = *(str + i);
        if (is_alpha(c) && is_lower(c))
            *(str+i) = *(str+i) - 32;
    }
}

int str_concat(char str1[], char str2[], int str1Len) {
    int str1_len = str_len(str1);
    int str2_len = str_len(str2);

    if (str1_len + str2_len + 1 > str1Len) return -1;

    for (int i = 0; i < str2_len; i++) {
    	*(str1+i+str1_len) = *(str2+i);
    }
    str1[str1_len + str2_len] = '\0';
    return 0;
}

int str_copy(char str1[], char str2[]) {
    int str2_len = str_len(str2);
    if (str2_len > str_len(str1)) return -1;

    for (int i = 0; i < str2_len; i++) {
    	*(str1+i) = *(str2+i);
    }
    *(str1+str2_len) = '\0';
    return 0;
}

#include "library.h"
#include <iostream>
#include <map>
#include <cstdio>
#include <chrono>
#include <thread>

#ifdef _WIN32
#else
#endif

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

void to_lowercase(char str[]) {
}

void to_uppercase(char str[]) {
}

void str_concat(char str1[], char str2[], int str1Len) {
}

void str_copy(char str1[], char str2[]) {
}

int str_find_char(char str[], char ch) {    

    return -1;
}
int str_find_substr(char str[], char substr[]) {
    return -1;
}
void str_reverse(char str[]) {
}

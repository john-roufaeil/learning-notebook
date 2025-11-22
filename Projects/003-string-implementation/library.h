#ifndef LIBRARY_H
#define LIBRARY_H

void print_divider();
int str_len(char inputStr[]);
int str_cmp(char str1[], char str2[]);
int is_alpha(char c);
void to_lowercase(char str[]);
void to_uppercase(char str[]);
int str_concat(char str1[], char str2[], int str1Len);
int str_copy(char str1[], char str2[]);

#endif
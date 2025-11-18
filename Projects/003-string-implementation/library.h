#ifndef LIBRARY_H
#define LIBRARY_H

void print_divider();
int str_len(char inputStr[]);
int str_cmp(char str1[], char str2[]);
void to_lowercase(char str[]);
void to_uppercase(char str[]);
void str_concat(char str1[], char str2[], int str1Len);
void str_copy(char str1[], char str2[]);
int str_find_char(char str[], char ch);
int str_find_substr(char str[], char substr[]);
void str_reverse(char str[]);

#endif
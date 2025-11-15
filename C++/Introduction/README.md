# Introduction

## Data Types

Basic Data Types:
- int
- float
- double
- char
- bool
- void

Pointers
- int * ptr = new int;
- int * ptr = new int[10];
- int * ptr = &x;
- int x = *ptr;

Other topics
- If Conditions
- For and While Loop
- Arrays
    - int arr1 [10];
    - int * arr2 = new int[N];
- Structs
- Classes

### Size modifiers

Size modifiers are not types, just keywords to modify the size of types

- short int -> 2 bytes
- int -> 4 bytes
- long int -> 8 bytes
- long long int -> 8 bytes
- float -> 4 bytes
- double -> 8 bytes
- long double -> 16 bytes

Compiler errors:
- short/long float
- short double

### Sign modifieres - Signed and unsigned

Compiler errors:
- double
- float

```cpp
char x=-128
char y=-x //128
printf("%d", y) //-128
```

### Casting



Procedure paradigm
`standardtype.h`

not all # is preprocessor

```c++
#ifndef STANDARDTYPE
#define STANDARDTYPE
int x;
#endif
```

typedef, custom declarations, and prototypes in .h files

```c++
unsigned char i = 8;
for (i=8; i>=0; i--) {
    printf("Welcome \n");
}
```

## Passing Variables to Functions

### Passing by Value

Not recommended for large structures

```cpp
int func(int x) {
    x++;
    return x;
}

int x = 5;
cout << func(x); // printed 6, x is 5
```

### Passing by Reference

```cpp
int func(int & x) {
    x++;
    return x;
}
int x = 5;
cout << func(x); // printed 6, x is 6
```

### Passing by Pointer

```cpp
int func(int * ptr) {
    (*ptr)++;
    return *ptr;
}

int x = 5;
cout << func(&x); // printed 6, x is 6
```

### Passing by constant reference

```cpp
int func(const int & x) {
    return x+1;
}

int x = 5;
cout << func(x); // printed 6, x is 5
```

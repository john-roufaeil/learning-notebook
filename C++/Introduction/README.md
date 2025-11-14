# Introduction

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


#include <stdarg.h>
#include <stdio.h>

int add_variadic(const int total, ...) {
    va_list args;
    va_start(args, total);  

    int result = 0;
    for (int i = 0; i < total; i++){
        result += va_arg(args, int);
    }

    va_end(args);
    return result;
}

int add_array(int size, int* nbrArray) {
    int result = 0;
    for (int i = 0; i < size; i++){
        result += *(nbrArray);
        nbrArray++;
    }
    nbrArray -= (size);
    return result;
}
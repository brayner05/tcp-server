#include <stdio.h>

typedef struct {
    int x;
} MyType;

typedef struct {
    short y;
    int x;
} MyChildType;


void foo(MyType *mt) {
    printf("%d\n", mt->x);
}

int main(void) {
    MyChildType mct = {
        .y = 0x01,
        .x = ~0
    };
    foo(&mct);
}
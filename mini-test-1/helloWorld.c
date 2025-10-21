#include <stdio.h>

struct foo5 {
    char c;
    struct foo5_inner {
        char *p;
        short x;
    } inner;
};

int main(void) {
    int i[3] = {12,4,1}, j = 3, *p = i, *q = &j, *r;
    int A = 5**p / *q+7;
    printf("%d \n", *q+7);
    printf("A %d:\n", A);
    *(r=&j) += **&p;
    printf("r %d:\n", *r);
    printf("Size of foo5: %zu\n", sizeof(struct foo5));
    return 0;
}
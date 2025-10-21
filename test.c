#include <stdio.h>
#include <stdbool.h>
#define HELLO_CONSTANT "Hello World!\n"

// function
int isEven(int num) {
    if ((num % 2) == 0) {
        return true;
    }
    return false;
}

int main(void) {
    int m = 0;
    m = m * 10 + ('8' - '0');
    printf("%d: ", m);

}
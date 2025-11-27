#include <stdio.h>
#include <stdlib.h>

void allocateIntSimple() {
    int *p = malloc(sizeof(int)); // allocate one int on the heap
    if (!p) return NULL;
    *p = 42;
    printf("value of p: %d\n", *p);

    free(p); // free the heap allocation once we are done
    p = NULL; // avoid dangling pointer
}

int main() {
    int* intPointer;
    size_t size = 16;
    intPointer = (int*) malloc(size * sizeof(int)); //16 * 4 = 64
    if (intPointer == NULL) {
        perror("malloc");
        exit(0);
    }

    // initialize allocated block
    for (int i = 0; i < size; i++) {
        // intPointer[i] = i;
        *(intPointer + i) = i;
    }

    for (int i = 0; i < size; i++) {
        // printf("num: %d\n",intPointer[i]);
        printf("num: %d\n", *(intPointer + i));
    }

    if (intPointer) {
        free(intPointer);
    }


    // lets do this with char
    // my goal: allocate exactly the space that we need and put inputString -> charPointer
    // after this, we will print this out.

    // first allocate enough memory using malloc.
    char* inputString = "something";
    char* charPointer;

    // size_t inputStringSize = (9 + 1); // 9 characters, 1 for null terminator
    size_t inputStringSize = strlen(inputString) + 1;

    charPointer = (char*) malloc(inputStringSize * sizeof(char));

    for (char c = 0; c < inputStringSize; c++) {
        *(charPointer + c) = *(inputString + c);
    }

    for (char c = 0; c < inputStringSize; c++) {
        printf("char: %c\n", *(charPointer + c));
    }

    if (charPointer) {
        free(charPointer);
    }

    return 0;
}
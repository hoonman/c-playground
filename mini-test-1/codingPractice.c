// ICS 53 - C Coding Practice

// Assume string.h functions are not permitted unless explicitly specified.

// Write a program in C to compute the sum of elements in an array starting at an index to a second index using pointers only. Assume indexes are within the range [0,size-1]
// // sum of all elements array[begin_index] to array[end_index]
// int rangeSum(int array[], int begin_index, int end_index, int size)    
#include <stdio.h>
#include <ctype.h>
int rangeSum(int array[], int begin_index, int end_index, int size) {
    // initialize a pointer int* mover that will move thorugh the array one by one. we will also have an index variable that counts what index we are at
    int totalSum = 0;
    // for (int i = 0; i < size; i++) {
    //     if (i >= begin_index && i <= end_index) {
    //         totalSum += *(array + i);
    //     }
    // }

    // while loop logic
    int* mover = array;
    int i = 0;
    while (i < size) {
        if (i >= begin_index && i <= end_index) {
            totalSum += *mover; // you can do this or *(mover + i) and not move the mover !
        }
        i++;
        mover++;
    }
    return totalSum;
}
// Write a function, using only pointers, to check if the string end occurs at the end of string. Return 1 if yes, 0 if no.
// int strend(char* string, char* end)
int strend(char* string, char* end) {
    // declare char* mover and move it to the back ('\0')
    // declare strMover and endMover
    // declare sizes so we know when to stop
    char* strMover = string;
    char* endMover = end;
    int strSize = 0;
    int endSize = 0;

    int matchCounter = 0;

    while (*strMover != '\0') {
        strSize++;
        strMover++;
    }

    while (*endMover != '\0') {
        endSize++;
        endMover++;
    }
    // now that we have these, we can move them backwards 
    // if endSize is bigger, than we can exit
    if (endSize > strSize) {
        return 0;
    }

    while (endSize > 0) {
        if (*strMover == *endMover) {
            matchCounter++;
            strMover--;
            endMover--;
        } else {
            return 0; // if at any point it doesn;t match we can just return 0;
        }
        endSize--;
    }
    return 1;
}

// Write a function to reverse a string in place (in the same memory as given) using only pointers
// void reverseStr(char* string)
// h e l l o
// o e l l h
void reverseStr(char* string) {
    // modify the strin in place meaning we need to swap places 
    char* startPtr = string;
    char* endPtr = string;
    // char* tempPtr = string;
    char temp = 'a';
    int size = 0;
    int counter = 0;
    while (*endPtr != '\0') {
        size++;
        endPtr++;
    }
    endPtr--;

    while (counter <= (size / 2)) {
        temp = *startPtr;
        *startPtr = *endPtr;
        *endPtr = temp;
        
        startPtr++;
        endPtr--;
        counter++;
    }


}

// Write a function to count all non-overlapping occurrences of a substring in string s using pointers only.
// int countSubString(char* s, char* substring)
int countSubString(char* s, char* substring) {
    char* substrMover = substring;
    int totalCount = 0;
    if (*substrMover == ' ') {
        return 0;
    }
    while (*s != '\0') {
        while (*substrMover != '\0') {
            if (*s == *substrMover) {
                substrMover++;
                s++;
            } else {
                substrMover = substring;
                s++;
                break;
            }
        }
        if (*substrMover == '\0') {
            substrMover = substring;
            totalCount++;
        }
    }
    return totalCount;
}

// Ex: countSubString("aaaaaabaaccacd", "aa") returns 4

// Write a function to count all occurrences of each of the characters in the symbol string within string s using only pointers.
// int countSubString(char* s, char* symbol)
// int countSubString(char* s, char* symbol) {
//     int totalCount = 0;
//     char* symbolPtr = symbol;
//     while (*s != '\0') {
//         while (*symbolPtr != '\0') {
//             if (*s == *symbolPtr ) {
//                 totalCount++;
//             }
//             symbolPtr++;
//         }
//         symbolPtr = symbol;
//         s++;
//     }
//     return totalCount;
// }

// Ex: countSubString("aa!a!!bd$ccacd", "ab!") returns 8

// Write a case-sensitive string compare function which returns 1 if the strings fully match (length and all chars), 0 if the strings don't match but are of the same length, and -1 if the strings don't match and are different lengths
// int strcompare(char* s, char* t)

int strCompare(char* s, char* t) {
    char* sPtr = s;
    char* tPtr = t;
    int sPtrLength = 0;
    int tPtrLength = 0;

    while (*sPtr != '\0') {
        sPtr++;
        sPtrLength++;
    }

    while (*tPtr != '\0') {
        tPtr++;
        tPtrLength++;
    }

    sPtr = s;
    tPtr = t;

    if (sPtrLength != tPtrLength) {
        return -1;
    }

    while (*sPtr != '\0' && *tPtr != '\0') {
        if (*sPtr != *tPtr) {
            return 0;
        }
    }
    return 1;
}

// Write a function, using only pointers, to find the length of a string, where the end of the string is defined as either '\0' or any character in the delimiter string (which is null-terminated). 
// int strlenD(char* string, char* delimiter);
int strLenD(char* string, char* delimiter) {
    int length = 0;
    char* mover = string;
    while (*mover != '\n' && !isspace(*mover)) {
        mover++;
        length++;
    }
    return length;
}

// Write a string compare function that returns 1 if the strings match for n characters starting at offset m, 0 if the strings don't match. You must check if m is within the length of both s and t.
// int submatch(char* s, char* t, int n, int m)
int submatch(char* s, char* t, int n, int m) {
    // 1 if the strings match for n characters starting at offset m 
    // 0 if the strings don't match.
    // check if m is within the length of s and t

    // since this seems like a one pass check, we don't need a mover, no need for double loop
    // we do need to make sure that we don't overflow for one string.
    // assume offset applies for both strings ?

    // check length
    int sLength = 0;
    int tLength = 0;
    char* sMover = s;
    char* tMover = t;

    while (*s != '\0') {
        if (sLength < m) {
            sMover++;
        }
        sLength++;
    }
    while (*t != '\0') {
        if (tLength < m) {
            tMover++;
        }
        tLength++;
    }

    // length doens't matter. match vs datc 
    // check if legnth s has enough characters after m
    if (sLength < (m + n) || tLength < (m + n)) {
        return 0;
    }

    int counter = 0;
    while ((sMover != '\0' || tMover != '\0') && (*sMover == *tMover)) {
        if (counter == n) {
            return 1;
        }
        sMover++;
        tMover++;
        counter++;
    }
    return 0;
}

// Write a function that substitutes all the occurrences of the from character with the to character in str using pointers only.
// char *substitute(char *str, char from, char to);

// struct person {
//   char netid[15];
//   float gpa;
//   char* major;
//   int age;
// } class[50]; 

// //Assume class[50] is fully initialized.
// struct person *p = &class[0];

// Write a function that counts all the people in the class with the specified major m, using only pointers and your strcompare function
// int countMajor(struct person *p, char *m, int sizeofClass)


// struct person {
//   char netid[15];
//   float gpa;
//   char* major;
//   int age;
// } class[50]; 

// //Assume class[50] is fully initialized.
// struct person *p = &class[0];

// Write a function that counts all the people in the class with the netid starting with prefix using only pointers and your submatch function
// int countMajor(struct person *p, char *prefix, int sizeofClass)

// struct person {
//   char netid[15];
//   float gpa;
//   char* major;
//   int age;
// } class[50]; 

// //Assume class[50] is fully initialized.
// struct person *p = &class[0];

// Write a function that counts all the people in the class with a gpa greater than g, using only pointers 
// int countGPA(struct person *p, float g, int sizeofClass)

// typedef struct node{
//      char c;
//      struct node* next;
// } Node;

// Assume Node* Listhead points at a singly linked list of initialized nodes.  

// Write a function to count the number of entries in the list that contain the same c character as Node t, using pointers only
// int countMatchList(Node * Listhead, Node t)

// typedef struct node{
//      char c;
//      struct node* next;
//      struct node* prev;
// } Node;

// Assume Node* Listhead points at a linked list of initialized nodes.  

// Write a function to determine if the elements in the list form a palindrome, using pointers only. Return 1 if yes, 0 if no
// int palendromeList(Node * Listhead)

// Ex: [x,a,m,a,x] or [a,b,b,a]

// typedef struct node{
//      char* string;
//      struct node* next;
// } Node;

// Assume Node* Listhead points at a linked list of initialized nodes.  

// Write a function to print out strings in the list which have length equal to len, using only pointers and your strlen

// void printStringByLen(Node * Listhead, int len)

// Given a function with the following definition:
// void printThreeWords(char* sentence){
// 	char* firstWord = NULL, *secondWord = NULL, *lastWord = NULL;
// 	//your code here
// 	print(“%s,%s,%s\n”, firstWord, secondWord, lastWord);
// };

// Assume that sentence is a c string that was allocated using malloc in the calling function. Assume all words are separated by a single ' ' (space) character. 

// Write an implementation of printThreeWords that prints out each word separately using only the pointers given to you. Do not allocate more space.

// EX:
// char* sentence = malloc(sizeof(char) * 24);
// // code to place the sentence “Computer Science Rules!” into sentence
// printThreeWords(sentence); // prints "Computer,Science,Rules!"


int main() {
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    printf("The sum of arr is %d: \n", rangeSum(arr, 4, 8, 10));
    // Test cases for strend function
    printf("\n--- Testing strend function ---\n");
    
    // Basic cases
    printf("Test 1: \"hello world\" ends with \"world\" -> Expected: 1, Got: %d\n", 
           strend("hello world", "world"));
    
    printf("Test 2: \"hello world\" ends with \"hello\" -> Expected: 0, Got: %d\n", 
           strend("hello world", "hello"));
    
    // Edge cases
    printf("Test 3: \"hello\" ends with \"\" (empty string) -> Expected: 1, Got: %d\n", 
           strend("hello", ""));
    
    printf("Test 4: \"\" (empty string) ends with \"\" (empty string) -> Expected: 1, Got: %d\n", 
           strend("", ""));
    
    printf("Test 5: \"\" (empty string) ends with \"hello\" -> Expected: 0, Got: %d\n", 
           strend("", "hello"));
    
    printf("Test 6: \"short\" ends with \"too long ending\" -> Expected: 0, Got: %d\n", 
           strend("short", "too long ending"));
    
    // Partial matches
    printf("Test 7: \"hello world\" ends with \"lo worl\" -> Expected: 0, Got: %d\n", 
           strend("hello world", "lo worl"));
    
    printf("Test 8: \"abcabc\" ends with \"abc\" -> Expected: 1, Got: %d\n", 
           strend("abcabc", "abc"));
    
    printf("Test 9: \"abcabc\" ends with \"cab\" -> Expected: 0, Got: %d\n", 
           strend("abcabc", "cab"));
    
    // Same strings
    printf("Test 10: \"test\" ends with \"test\" -> Expected: 1, Got: %d\n", 
           strend("test", "test"));

    printf("\n--- Testing reverseStr function ---\n");

    // Test case 1: Basic string
    char str1[] = "hello";
    printf("Test 1 (before): \"%s\"\n", str1);
    reverseStr(str1);
    printf("Test 1 (after): \"%s\" -> Expected: \"olleh\"\n", str1);

    // Test case 2: Even length string
    char str2[] = "abcd";
    printf("Test 2 (before): \"%s\"\n", str2);
    reverseStr(str2);
    printf("Test 2 (after): \"%s\" -> Expected: \"dcba\"\n", str2);

    // Test case 3: Odd length string
    char str3[] = "12345";
    printf("Test 3 (before): \"%s\"\n", str3);
    reverseStr(str3);
    printf("Test 3 (after): \"%s\" -> Expected: \"54321\"\n", str3);

    // Test case 4: String with spaces
    char str4[] = "hello world";
    printf("Test 4 (before): \"%s\"\n", str4);
    reverseStr(str4);
    printf("Test 4 (after): \"%s\" -> Expected: \"dlrow olleh\"\n", str4);

    // Test case 5: Palindrome string
    char str5[] = "radar";
    printf("Test 5 (before): \"%s\"\n", str5);
    reverseStr(str5);
    printf("Test 5 (after): \"%s\" -> Expected: \"radar\"\n", str5);

    // Test case 6: Single character
    char str6[] = "a";
    printf("Test 6 (before): \"%s\"\n", str6);
    reverseStr(str6);
    printf("Test 6 (after): \"%s\" -> Expected: \"a\"\n", str6);

    // Test case 7: Empty string
    char str7[] = "";
    printf("Test 7 (before): \"%s\"\n", str7);
    reverseStr(str7);
    printf("Test 7 (after): \"%s\" -> Expected: \"\"\n", str7);
    
    // Test cases for countSubString function
    printf("\n--- Testing countSubString function ---\n");

    // Test case 1: Basic example from requirement
    printf("Test 1: Count \"aa\" in \"aaaaaabaaccacd\" -> Expected: 4, Got: %d\n",
        countSubString("aaaaaabaaccacd", "aa"));

    // Test case 2: No matches
    printf("Test 2: Count \"xyz\" in \"abcdef\" -> Expected: 0, Got: %d\n",
        countSubString("abcdef", "xyz"));

    // Test case 3: Multiple non-overlapping matches
    printf("Test 3: Count \"ab\" in \"ababab\" -> Expected: 3, Got: %d\n",
        countSubString("ababab", "ab"));

    // Test case 4: Partial matches
    printf("Test 4: Count \"abc\" in \"abxabcabc\" -> Expected: 2, Got: %d\n",
        countSubString("abxabcabc", "abc"));

    // Test case 5: Potential overlaps (non-overlapping rule)
    printf("Test 5: Count \"aa\" in \"aaa\" -> Expected: 1, Got: %d\n",
        countSubString("aaa", "aa"));

    // Test case 6: Empty main string
    printf("Test 6: Count \"abc\" in \"\" -> Expected: 0, Got: %d\n",
        countSubString("", "abc"));

    // Test case 7: Empty substring (edge case)
    printf("Test 7: Count \"\" in \"abcdef\" -> Expected: 0, Got: %d\n",
        countSubString("abcdef", ""));

    // Test case 8: Single character matches
    printf("Test 8: Count \"a\" in \"banana\" -> Expected: 3, Got: %d\n",
        countSubString("banana", "a"));

    // Test case 9: Longer substring
    printf("Test 9: Count \"hello\" in \"hello world hello\" -> Expected: 2, Got: %d\n",
        countSubString("hello world hello", "hello"));

    // Test case 10: Match at beginning and end
    printf("Test 10: Count \"end\" in \"endgameend\" -> Expected: 2, Got: %d\n",
        countSubString("endgameend", "end"));

    return 0;

}
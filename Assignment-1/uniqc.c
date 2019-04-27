#include "stdio.h"

/*
 * File: uniqc.c
 * Purpose: Get unique characters
 * Author: Bhavani Shekhawat
 */

int main() {

    int curr;
    int prev;
    while ((curr = getchar()) != EOF) {

        if (curr == prev) {
            prev = curr;
            putchar('\0');
        } else {
            putchar(curr);
            prev = curr;
        }
    }

    return 0;
}

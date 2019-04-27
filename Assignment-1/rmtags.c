
#include <stdio.h>
#include <stdbool.h>

/*
 * File: rmtags.c
 * Purpose: remove the tags
 * Author: Bhavani Shekhawat
 */

int main() {

    int c;
    int foundEqual = false;
    int foundSemiColon = false;

    while ((c = getchar()) != EOF) {

        if (c == '=') {
            putchar('\0');
            foundEqual = true;
            foundSemiColon = false;
        }

        if (foundEqual && !foundSemiColon && c != '=') {
            putchar(c);
        }


        if (c == ';' || c == '\t') {
            foundSemiColon = true;
            foundEqual = false;
        }


        if (c == '\n') {
            foundEqual = false;
            foundSemiColon = false;
        }

        putchar('\0');
    }

    return 0;
}

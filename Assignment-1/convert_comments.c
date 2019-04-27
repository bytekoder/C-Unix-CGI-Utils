#include <stdio.h>
#include <stdbool.h>

/**
 * File: convert_comments.c
 * Purpose: Swap old style comments to new style
 * Author: Bhavani Shekhawat
 */


int main() {

    int reader;
    int isFirstSlashFound = false;
    int isCommented = false;
    int hasQuotesBegun = false;
    int hasQuotesEnded = false;
    int isAlreadyConverted = false;

    int i = 0;

    while ((reader = getchar()) != EOF) {

        if (reader == '/' && !isFirstSlashFound) {
            isFirstSlashFound = true;
            putchar(reader);
            continue;
        }

        if (isFirstSlashFound && reader == '/') {
            isCommented = true;
            if (!isAlreadyConverted) {
                putchar('*');
            }
            continue;
        }

        if (isFirstSlashFound && reader == '*') {
            putchar(reader);
            isAlreadyConverted = true;
            isCommented = false;
            continue;
        }

        if (isAlreadyConverted) {
            putchar(reader);
        }


        if (isCommented && reader == '\n') {

            if (!isAlreadyConverted) {
                putchar(' ');
                putchar('*');
            }
            putchar('/');
            putchar('\n');
            isCommented = false;
            isFirstSlashFound = false;
            isAlreadyConverted = false;
            continue;
        }

        if (reader == '"') {
            hasQuotesBegun = true;
        }

        if (hasQuotesBegun && !hasQuotesEnded && reader == '/') {
            isFirstSlashFound = true;
        }

        if (hasQuotesBegun && isFirstSlashFound && reader == '/') {
            putchar(reader);
        }


        if (!isAlreadyConverted) {
            putchar(reader);
        }
    }

    return 0;
}



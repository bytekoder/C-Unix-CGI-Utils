#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include <string.h>

/*
 *  hello6.c
 *
 *	purpose: shows how to examine a string char by char
 */

#define    STRSIZE        100

void repeat_a_message(char [], int);

int get_a_positive_number();

int is_all_digits(char []);

int main() {
    int maxnum;            /* limit			*/
    char message[STRSIZE];    /* an array of chars		*/

    printf("Print what string...? ");
    fgets(message, STRSIZE, stdin);    /* read in a string		*/

    //maxnum = get_a_positive_number();
    //repeat_a_message( message, maxnum );

    maxnum = is_all_digits(message);

    if (maxnum == 0) {
        printf("Not all digits");
    } else {
        printf("All Digits");
    }

    return 0;
}

void repeat_a_message(char message[STRSIZE], int times)
/*
 * purpose: prints out the string in `message' for `times' iterations
 */
{
    int i;
    while (i != times) {
        printf("%d, %s", i, message);
        i++;
    }
}

int get_a_positive_number()
/*
 * purpose: demand a positive integer from the user
 * returns: a positive integer
 */
{
    int inputval = -1;
    char inputstr[STRSIZE];
    int is_all_digits(char []);

    while (inputval <= 0) {
        printf("Repeat it how many times? ");    /* prompt	*/
        fgets(inputstr, STRSIZE, stdin);    /* input	*/
        if (is_all_digits(inputstr) == 0)        /* digits?	*/
            printf("This is not a number: %s", inputstr);
        else {
            inputval = atoi(inputstr);
            if (inputval <= 0)
                printf("%d is not positive number\n", inputval);
        }
    }
    return inputval;    /* send value back to caller */
}

int is_all_digits(char str[STRSIZE])
/*
 * purpose: examine a string and see if all the chars are digits
 * returns: 1 if all chars before the newline are digits, 0 otherwise
 * bug?:    what if a string with no chars appears?
 */
{
    printf("%i", strlen(str));
    for (int i = 0; i < strlen(str) - 1; i++) {

        if (!isdigit((int) str[i]) && str[i] != '\n') {
            return 0;
        }
    }
    return 1;
}



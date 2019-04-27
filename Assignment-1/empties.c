#include    <stdio.h>

/*
 *	empties.c
 *		assume lines look like
 *			XX=stuff;YY=morestuff;..;ZZ=yetmorestuff
 *		print lines that have one or more empty fields
 *		compile with readln.c
 *		Returns 0 for no empties, 1 for found some
 */

#define    LINESIZE    512
#define    TRUE        1
#define    FALSE        0
#define    DELIM        ';'

int has_empty(char []);

int readln(char [], int, char);

int main() {
    char line[LINESIZE];        /* an array of characters */
    int rv = 0;            /* passed back to shell	  */

    while (readln(line, LINESIZE, '\n') != 0)
        if (has_empty(line) == TRUE) {
            puts(line);
            rv = 1;
        }
    return rv;
}

int has_empty(char string[])
/*
 *	looks through the string to see if any fields are empty.
 *	this means that the character after an = is a colon or a newline
 *	return TRUE if any empties are found, else returns FALSE
 */
{
    int i;        /* use this for indexing */

    for (i = 0; string[i] != '\0'; i++)
        if (string[i] == '=' && (string[i + 1] == DELIM || string[i + 1] == '\0'))
            return TRUE;
    return FALSE;
}


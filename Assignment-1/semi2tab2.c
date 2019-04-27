#include    <stdio.h>

/*
 * semi2tab2.c
 *   purpose: filter data replacing semicolons with tab chars
 *     input: text
 *    output: text with tabs in place of semicolons
 *    errors: no error conditions
 *     usage: semi2tab < input > output
 *     notes: version 2 uses the more compact C syntax
 */

int main() {
    int c;        // this is ok as a comment, too
    while ((c = getchar()) != EOF) {
        if (c == ';') {
            c = '\t';    /* replace		*/
        }
        putchar(c);        /* send to output	*/
    }
    return 0;
}

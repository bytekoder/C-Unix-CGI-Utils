/**
 * Author: Bhavani Shekhawat
 * The program accepts as input rows of data.
 * Each row contains a sequence of strings separated by one or more spaces or tabs.
 * The program writes as output a table starting tag, a sequence of table rows, and then a table closing tag.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SPACE_CHAR              ' '
#define TAB_CHAR                '\t'
#define NEWLINE_CHAR            '\n'
#define START_TABLE_TAG         "<table>"
#define END_TABLE_TAG           "<table/>"
#define START_ROW_TAG           "<tr>"
#define END_ROW_TAG             "<tr/>"
#define START_CELL_TAG        "<td>"
#define END_CELL_TAG          "<td/>"
#define MAX_LINE_SIZE           256
#define DEFAULT_INDENT          4


int check_empty_line(char line[]);

void write_contents(char line[]);

void begin_table_tag(int *hasProcessed);

void end_table_tag();

void begin_row_tag();

void end_row_tag();

void begin_cell_tag();

void end_cell_tag();

void add_indent(int spaces);

void cleanup(int *hasProcessed);

int main() {

    int *p; // a pointer that maintains the flag
    int hasProcessed = 0;
    p = &hasProcessed;
    int reader;
    char line[MAX_LINE_SIZE];

    // Loop until EOF is not found
    while ((reader = getchar()) != EOF) {

        // Avoid getchar eating the first char
        ungetc(reader, stdin);

        // Loop each line
        if (fgets(line, MAX_LINE_SIZE, stdin)) {

            // End the program if there is nothing to process
            if (check_empty_line(line) != 1) {
                printf("%s", "Empty line found");
                return 0;
            } else {
                begin_table_tag(p);
                begin_row_tag();
                write_contents(line);
                end_row_tag();
            }

        }
    }
    end_table_tag();     // Call this only once at the very end
    cleanup(p);         // Reset all the flags

    return 0;
}


/**
 * Checks for an empty line
 * @param line a line containing cells data
 * @return an {@code int} indicating true/false
 */
int check_empty_line(char line[]) {
    for (int i = 0; i < MAX_LINE_SIZE; i++) {
        if (line[i] != SPACE_CHAR || line[i] != TAB_CHAR) {
            return 1;
        }
    }
    return 0;
}


/**
 * Start table tags and mark them as processed
 * @param hasProcessed
 */
void begin_table_tag(int *hasProcessed) {

    // Was a curious move just to see how pointers work
    // Should have just declared a global bool variable
    if (*hasProcessed == 0) {
        printf("%s", START_TABLE_TAG);
        printf("%c", NEWLINE_CHAR);
        *hasProcessed = 1;
    }

}

/**
 * End table tags
 * @param isDataFound
 */
void end_table_tag() {

    printf("%s", END_TABLE_TAG);
    printf("%c", NEWLINE_CHAR);

}

/**
 * Write the row <td> tag
 */
void begin_row_tag() {
    add_indent(4);
    printf("%s", START_ROW_TAG);
    printf("%c", NEWLINE_CHAR);
}

/**
 * End the row <td/> tag
 */
void end_row_tag() {
    add_indent(DEFAULT_INDENT);
    printf("%s", END_ROW_TAG);
    printf("%c", NEWLINE_CHAR);
}

/**
 *  Write the cell <tr> tag
 */
void begin_cell_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", START_CELL_TAG);
}

/**
 * End the cell <tr/> tag
 */
void end_cell_tag() {
    printf("%s", END_CELL_TAG);
    printf("%c", NEWLINE_CHAR);
}

/**
 * Indent to four spaces (standard)
 */
void add_indent(int spaces) {
    for (int i = 0; i < spaces; i++) {
        printf("%c", ' ');
    }
}

/**
 * Writes the content in its cell
 * @param line
 */
void write_contents(char line[]) {
    int activateCellTag = true;
    int activateSpaceSkipper = true;
    for (int i = 0; i < strlen(line); i++) {
        if (i == 0 || activateCellTag) {
            if (activateSpaceSkipper) {

                begin_cell_tag();
            }
            activateCellTag = false;
        }

        // Skip the extra spaces. Just need a single space here.
        if ((line[i] != SPACE_CHAR) && (line[i] != TAB_CHAR) && (line[i] != NEWLINE_CHAR)
            && !activateSpaceSkipper) {
            begin_cell_tag();
            activateSpaceSkipper = true;
        }

        if ((line[i] != SPACE_CHAR) && (line[i] != TAB_CHAR) && (line[i] != NEWLINE_CHAR)) {
            printf("%c", line[i]);
        } else {
            // Check if this needs to be skipped
            if (activateSpaceSkipper) {

                end_cell_tag();
            }
            activateSpaceSkipper = false;
            activateCellTag = true;     // Needs to be activated only when space is encountered
        }

        // No need to iterate over if EOL found
        if (line[i] == NEWLINE_CHAR) {
            break;
        }

    }
}

/**
 * Clean the flags
 * @param p a flag that tells if the <table> tag was processed
 */
void cleanup(int *p) {
    *p = 0;
}


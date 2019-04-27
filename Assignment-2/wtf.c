/**
 * Author: Bhavani Shekhawat
 * Purpose: Does the same work as tt2ht2.c however, this also deals with (;) delimited data
 * and converts it to a table format. This is achieved through a conditional check of delimiter tag and then
 * storing that particular delimiter in a single space array
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE           256
#define MAX_SECTION_SIZE        4096
#define DEFAULT_INDENT          4
#define DELIMITER_TAG_LENGTH        13
#define NO_PROCESS_TAG_START               "<noprocess>"
#define NO_PROCESS_TAG_END                 "</noprocess>"
#define ATTRIBUTE_TAG_START             "<attributes>"
#define ATTRIBUTE_TAG_END               "</attributes>"
#define START_ROW_TAG           "<tr>"
#define END_ROW_TAG             "<tr/>"
#define START_CELL_TAG        "<td>"
#define END_CELL_TAG          "<td/>"
#define NEWLINE_CHAR            '\n'
#define SPACE_CHAR              ' '
#define TAB_CHAR                '\t'
#define TABLE_START             "<table"
#define TABLE_END          "</table>"
#define DELIMITER_TAG       "<delim value="
#define DEFAULT_DELIMITER       " \n\t\r"

enum Tag {
    NO_PROCESS_TAG,
    ATTRIBUTE_TAG,
    TABLE_DATA
} type;

static bool check_if_tag_started(char line[]);

static bool check_if_tag_ended(char line[]);

static bool mark_as_table_data();

static void skip_line(char line[]);

void process_html_data(char line[]);

void process_attribute_data(char line[]);

void process_plain_text(char line[]);

void clean_up_attributes(char line[][MAX_SECTION_SIZE]);

void begin_row_tag();

void end_row_tag();

void begin_cell_tag();

void end_cell_tag();

void add_indent(int spaces);

static void start_table_tag();

static void end_table_tag();

static void check_delimiters(char line[]);


int attributes_counter = -1;
int td_class_counter = 0;

bool hasSkipped = false;
bool isStartTagFound = false;
bool isEndTagFound = false;
bool isTableStartDone = false;
bool isTableEndDone = false;
bool isCompleted = false;
bool isDelimFound = false;
bool isDelimProcessed = false;

char attributes_array[MAX_LINE_SIZE][MAX_SECTION_SIZE];
char table_start_tag_array[MAX_LINE_SIZE];
char table_end_tag_array[MAX_LINE_SIZE];
char delim_tag[1];

int main() {

    int reader;
    char line[MAX_LINE_SIZE];

    while ((reader = getchar()) != EOF) {
        ungetc(reader, stdin);

        if (fgets(line, MAX_LINE_SIZE, stdin)) {

            // Check if the delimiter was processed or not
            if (!isDelimProcessed) {
                check_delimiters(line);
                if (isDelimFound && isDelimProcessed) {
                    skip_line(line);
                    continue;
                }
            }

            if (!isStartTagFound || isEndTagFound) {
                check_if_tag_started(line);
            }

            if (isStartTagFound) {

                check_if_tag_ended(line);
            }


            if ((isStartTagFound && !isEndTagFound) || type == TABLE_DATA) {
                switch (type) {
                    case NO_PROCESS_TAG:
                        if (!hasSkipped) {
                            skip_line(line);
                            continue;
                        }
                        process_html_data(line);
                        break;
                    case ATTRIBUTE_TAG:
                        if (!hasSkipped) {
                            clean_up_attributes(attributes_array);
                            skip_line(line);
                            continue;
                        }
                        process_attribute_data(line);
                        break;
                    case TABLE_DATA:
                        // treating delimiter as plain-text however still need to make a distinction between
                        // actual data vs. a delimiter
                        if ((isDelimFound && isDelimProcessed) || !isDelimFound) {

                            process_plain_text(line);
                        }
                        break;
                }
            }

        }

    }
    if (type == TABLE_DATA && !isTableEndDone) {
        end_table_tag();
    }

    return 0;
}

static bool check_if_tag_started(char line[]) {
    char *no_process_start_pos = strstr(line, NO_PROCESS_TAG_START);
    char *attribute_start_pos = strstr(line, ATTRIBUTE_TAG_START);


    if (no_process_start_pos) {
        isStartTagFound = true;
        isEndTagFound = false;
        type = NO_PROCESS_TAG;
        return true;
    } else if (attribute_start_pos) {
        isStartTagFound = true;
        isEndTagFound = false;
        type = ATTRIBUTE_TAG;
        return true;
    } else if (no_process_start_pos == NULL && attribute_start_pos == NULL && !isStartTagFound) {

        mark_as_table_data();
        return true;

    } else {
        return false;
    }

}

static bool check_if_tag_ended(char line[]) {
    char *no_process_end_pos = strstr(line, NO_PROCESS_TAG_END);
    char *attribute_end_pos = strstr(line, ATTRIBUTE_TAG_END);

    if (no_process_end_pos) {
        isEndTagFound = true;
        isStartTagFound = false;
        hasSkipped = false;
        type = NO_PROCESS_TAG;
        return true;
    } else if (attribute_end_pos) {
        isEndTagFound = true;
        isStartTagFound = false;
        hasSkipped = false;
        type = ATTRIBUTE_TAG;
        isEndTagFound = true;
        return true;
    } else {
        return false;
    }
}

static bool mark_as_table_data() {
    type = TABLE_DATA;
    return true;
}

/**
 * Check if the line has any <delimiter>. This assumes that <delimiter> is on its own line.
 * @param line
 */
void check_delimiters(char line[]) {
    char *delimiter_tag_pos = strstr(line, DELIMITER_TAG);
    if (delimiter_tag_pos) {
        isDelimFound = true;
        delim_tag[0] = delimiter_tag_pos[DELIMITER_TAG_LENGTH];
        isDelimProcessed = true;
    }
}

void process_html_data(char line[]) {
    char *table_start_tag_pos = strstr(line, TABLE_START);
    char *table_end_tag_pos = strstr(line, TABLE_END);

    if (table_start_tag_pos) {
        for (int i = 0; i < MAX_LINE_SIZE; i++) {
            if (line[i] != '\n') {

                table_start_tag_array[i] = line[i];
            } else {
                break;
            }
        }
    }

    if (table_end_tag_pos) {
        for (int i = 0; i < MAX_LINE_SIZE; i++) {
            if (line[i] != '\n') {

                table_end_tag_array[i] = line[i];
            } else {
                break;
            }
        }

    }

    if (!table_start_tag_pos && !table_end_tag_pos) {

        for (int i = 0; i < MAX_LINE_SIZE; i++) {
            if (line == table_start_tag_pos) {
                break;
            }
            if (line[i] == '\n' || line[i] == '\0') {
                break;
            } else {
                printf("%c", line[i]);
            }
        }
    }

    if (!table_end_tag_pos && !table_end_tag_pos) {

        printf("%c", '\n');
    }
}

void process_attribute_data(char line[]) {
    bool status = false;
    attributes_counter++;
    for (int i = attributes_counter; i < MAX_LINE_SIZE; i++) {
        if (status == true) {
            break;
        }
        for (int j = 0; (j < MAX_SECTION_SIZE) && !status; j++) {
            attributes_array[i][j] = line[j];

            // This doesn't really do anything. Was a failed attempt of locale conversion to UTF-8
            // Did try the method from locale.h, however that didn't work either.
            if (line[j] == '\'') {
                attributes_array[i][j] = '\'';
            }

            if (line[0] == '\n') {
                attributes_array[i][j + 1] = '\n';
                break;
            }

            if (line[j] == '\n') {
                status = true;
                break;
            }

        }
    }
}

void process_plain_text(char line[]) {
    const char *dst = "</td>";
    char *token = strtok(line, DEFAULT_DELIMITER);
    char override_delim[1];
    override_delim[0] = delim_tag[0];

    // Check if there was a delimiter passed or not
    if (delim_tag[0] != '\0') {
        token = strtok(line, override_delim);
    }

    if (!isTableStartDone) {
        start_table_tag();
    }
    begin_row_tag();
    while (token) {
        bool wasAttributed = false;
        add_indent(3 * DEFAULT_INDENT);
        if (attributes_counter > -1) {
            isCompleted = false;

            for (int i = td_class_counter; (i <= attributes_counter) && !isCompleted;) {
                printf("<td ");
                wasAttributed = true;
                for (int j = 0; j < MAX_LINE_SIZE; j++) {

                    if (attributes_array[i][j] == ' ') {
                        continue;
                    }
                    if (attributes_array[i][j] == '\n' || attributes_array[i][j] == '\0') {
                        isCompleted = true;
                        break;
                    }
                    printf("%c", attributes_array[i][j]);

                }
                printf(">");
                td_class_counter++;
            }
        }

        if (!wasAttributed) {
            printf("<td>");
        }

        printf("%s", token);
        puts(dst);

        // If delimiter available, else work with space
        if (delim_tag[0] != '\0') {

            token = strtok(NULL, ";");
        } else {
            token = strtok(NULL, " ");
        }

    }
    end_row_tag();
    td_class_counter = 0;
}

void start_table_tag() {
    for (unsigned int i = 0; i < sizeof(table_start_tag_array) / sizeof(table_start_tag_array[0]); i++) {
        printf("%c", table_start_tag_array[i]);
    }
    printf("%c", '\n');
    isTableStartDone = true;
}

void end_table_tag() {
    for (unsigned int i = 0; i < sizeof(table_end_tag_array) / sizeof(table_end_tag_array[0]); i++) {
        printf("%c", table_end_tag_array[i]);
    }
    printf("%c", '\n');
    isTableEndDone = true;
}


void begin_row_tag() {
    add_indent(2 * DEFAULT_INDENT);
    printf("%s", START_ROW_TAG);
    printf("%c", NEWLINE_CHAR);
}

void end_row_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", END_ROW_TAG);
    printf("%c", NEWLINE_CHAR);
}

void begin_cell_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", START_CELL_TAG);
}


void end_cell_tag() {
    add_indent(2 * DEFAULT_INDENT);
    printf("%s", END_CELL_TAG);
    printf("%c", NEWLINE_CHAR);
}

void add_indent(int spaces) {
    for (int i = 0; i < spaces; i++) {
        printf("%c", ' ');
    }
}

static void skip_line(char line[]) {
    for (int i = 0; i < MAX_LINE_SIZE; i++) {
        if (line[i] == '\n') {
            hasSkipped = true;
            break;
        }
    }
}


void clean_up_attributes(char line[][MAX_SECTION_SIZE]) {
    attributes_counter = -1;
    memset(line, 0, sizeof(line[0][0]) * MAX_LINE_SIZE * MAX_SECTION_SIZE);
}



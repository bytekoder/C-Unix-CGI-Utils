/**
 * Author: Bhavani Shekhawat
 * Purpose: Converts any data to <html> table format
 * Usage: <preprocess></preprocess> is treated as plain html, however the table classes are parsed
 * <attributes></attributes> define any table <td> classes and are applied to <td> in the mentioned order.
 * In case of '/n' between attributes would mean to skip the <td> at that index
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE           256
#define MAX_SECTION_SIZE        4096
#define DEFAULT_INDENT          4
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


int attributes_counter = -1;
int td_class_counter = 0;

bool hasSkipped = false;
bool isStartTagFound = false;
bool isEndTagFound = false;
bool isTableStartDone = false;
bool isTableEndDone = false;
bool isCompleted = false;

char attributes_array[MAX_LINE_SIZE][MAX_SECTION_SIZE];
char table_start_tag_array[MAX_LINE_SIZE];
char table_end_tag_array[MAX_LINE_SIZE];

int main() {

    int reader;
    char line[MAX_LINE_SIZE];

    while ((reader = getchar()) != EOF) {
        ungetc(reader, stdin);

        if (fgets(line, MAX_LINE_SIZE, stdin)) {

            if (!isStartTagFound || isEndTagFound) {
                check_if_tag_started(line);
            }

            if (isStartTagFound) {

                check_if_tag_ended(line);
            }


            // Check for the type of tag and set appropriate flags
            // No processing will be done as long as startTag is found and endTag is not found
            // Therefore, even if tags appear within tags, it should work fine
            if ((isStartTagFound && !isEndTagFound) || type == TABLE_DATA) {
                switch (type) {
                    case NO_PROCESS_TAG:
                        if (!hasSkipped) {      // Make sure only the tag was skipped. We don't want to skip contents.
                            skip_line(line);    // Skip the line since we don't want to display the tags
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
                        process_plain_text(line);
                        break;
                }
            }

        }

    }
    // Close the table tags after plain-text data has been transformed
    if (type == TABLE_DATA && !isTableEndDone) {
        end_table_tag();
    }

    return 0;
}

/**
 * Check if <noprocess> or <attribute> tags are started
 * @param line
 * @return flag indicating true or false
 */
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

/**
 * Check if the tags were ended
 * @param line
 * @return flag indicating true/false
 */
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

/**
 * Mark the content eligible for table conversion
 * @return true/false
 */
static bool mark_as_table_data() {
    type = TABLE_DATA;
    return true;
}

/**
 * Process the data from <noprocess> section
 * @param line
 */
void process_html_data(char line[]) {
    char *table_start_tag_pos = strstr(line, TABLE_START);  // Check if <noprocess> contains any of the table tags
    char *table_end_tag_pos = strstr(line, TABLE_END); // Check for </table> tags as well

    // Check if <table> was found
    if (table_start_tag_pos) {
        for (int i = 0; i < MAX_LINE_SIZE; i++) {
            if (line[i] != '\n') {

                table_start_tag_array[i] = line[i];
            } else {
                break;
            }
        }
    }

    // Check if </table> was found
    if (table_end_tag_pos) {
        for (int i = 0; i < MAX_LINE_SIZE; i++) {
            if (line[i] != '\n') {

                table_end_tag_array[i] = line[i];
            } else {
                break;
            }
        }

    }

    // Check if both were not found
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

/**
 * Parse the content under <attributes></attributes>
 * @param line
 */
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

            // Check if this is an empty line
            if (line[0] == '\n') {
                attributes_array[i][j + 1] = '\n';
                break;
            }

            // End encountered so break the loop
            if (line[j] == '\n') {
                status = true;
                break;
            }

        }
    }
}

/**
 * Process the plain text to table data
 * @param line
 */
void process_plain_text(char line[]) {
    const char *dst = "</td>";
    if (!isTableStartDone) {
        start_table_tag();
    }
    char *token = strtok(line, " \n\t\r");  // Tokenize on space/tab char
    begin_row_tag();
    while (token) {
        bool wasAttributed = false;
        add_indent(3 * DEFAULT_INDENT);
        if (attributes_counter > -1) {
            add_indent(4);
            isCompleted = false;

            // Loop through the stored class attributes
            for (int i = td_class_counter; (i <= attributes_counter) && !isCompleted;) {
                printf("<td ");
                wasAttributed = true;
                for (int j = 0; j < MAX_LINE_SIZE; j++) {

                    if (attributes_array[i][j] == ' ') {
                        continue;
                    }
                    // Go to the next one
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

        // If the element needs the table class to be applied
        if (!wasAttributed) {
            printf("<td>");
        }

        // NOTE: I am doing something stupid here. strtok() appends a \0 at the end and I tried several techniques to append
        //       a </td> but for some reason it isn't doing it.
        printf("%s", token);
        puts(dst);
        token = strtok(NULL, " ");
    }
    end_row_tag();
    td_class_counter = 0;   // Reset the counter so that it iterates to next row in the array
}

/**
 * Start table tags
 */
void start_table_tag() {
    for (int i = 0; i < sizeof(table_start_tag_array) / sizeof(table_start_tag_array[0]); i++) {
        printf("%c", table_start_tag_array[i]);
    }
    printf("%c", '\n');
    isTableStartDone = true;
}

/**
 * End table tags
 */
void end_table_tag() {
    for (int i = 0; i < sizeof(table_end_tag_array) / sizeof(table_end_tag_array[0]); i++) {
        printf("%c", table_end_tag_array[i]);
    }
    printf("%c", '\n');
    isTableEndDone = true;
}

/**
 * Begin <tr>
 */
void begin_row_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", START_ROW_TAG);
    printf("%c", NEWLINE_CHAR);
}

/**
 * End </tr>
 */
void end_row_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", END_ROW_TAG);
    printf("%c", NEWLINE_CHAR);
}

/**
 * Begin <td>
 */
void begin_cell_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", START_CELL_TAG);
}

/**
 * End </td>
 */
void end_cell_tag() {
    add_indent(2 * (DEFAULT_INDENT));
    printf("%s", END_CELL_TAG);
    printf("%c", NEWLINE_CHAR);
}

/**
 * Add indent
 * @param spaces refers to amount of indent wanted
 */
void add_indent(int spaces) {
    for (int i = 0; i < spaces; i++) {
        printf("%c", ' ');
    }
}

/**
 * Skip the line so that it doesn't get processed
 * @param line
 */
static void skip_line(char line[]) {
    for (int i = 0; i < MAX_LINE_SIZE; i++) {
        if (line[i] == '\n') {
            hasSkipped = true;
            break;
        }
    }
}

/**
 * Clean up attributes_array so that if there is another <attribute> found, it gets overridden
 * @param line
 */
void clean_up_attributes(char line[][MAX_SECTION_SIZE]) {
    attributes_counter = -1;
    memset(line, 0, sizeof(line[0][0]) * MAX_LINE_SIZE * MAX_SECTION_SIZE);
}
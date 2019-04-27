#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE           256
#define MAX_SECTION_SIZE        4096
#define NO_PROCESS_TAG_START               "<noprocess>"
#define NO_PROCESS_TAG_END                 "</noprocess>"
#define ATTRIBUTE_TAG_START             "<attributes>"
#define ATTRIBUTE_TAG_END               "</attributes>"

enum Status {
    PROCESS_STARTED,
    PROCESS_OPEN,
    PROCESS_CLOSED,
    ATTRIBUTES_IN_PROCESS,
    ATTRIBUTES_PROCESSING,
    ATTRIBUTES_PROCESSED,
    TABLE_PROCESSING,
    TABLE_PROCESSED,
    NO_STATUS
} stage;

static bool parse_noprocess(char line[], char const *term);

static bool parse_attributes(char line[], char const *term);

int parse_table(char line[]);

void write_to_stdout(char line[], char const *delimiter);

void write_to_arr(char line[], char const *delimiter);

void clean_up(char attrib[][MAX_SECTION_SIZE]);


int attributes_counter = -1;

char attributes_array[MAX_LINE_SIZE][MAX_SECTION_SIZE];

int main() {

    stage = NO_STATUS;
    int reader;
    char line[MAX_LINE_SIZE];

    while ((reader = getchar()) != EOF) {
        ungetc(reader, stdin);

        if (fgets(line, MAX_LINE_SIZE, stdin)) {


            if (stage == NO_STATUS) {
                parse_noprocess(line, NO_PROCESS_TAG_START);
            }

            if (stage == PROCESS_STARTED) {
                stage = PROCESS_OPEN;
                continue;
            }

            if (stage == PROCESS_OPEN) {
                write_to_stdout(line, NO_PROCESS_TAG_END);
            }

            if (stage == PROCESS_OPEN && reader == EOF) {
                stage = PROCESS_CLOSED;
            }

            if (stage == PROCESS_CLOSED) {
                parse_attributes(line, ATTRIBUTE_TAG_START);
                continue;
            }

            if (stage == ATTRIBUTES_IN_PROCESS) {
                stage = ATTRIBUTES_PROCESSING;
            }

            if (stage == ATTRIBUTES_PROCESSING) {
                write_to_arr(line, ATTRIBUTE_TAG_END);
            }

            if (stage == ATTRIBUTES_PROCESSED) {
                for (int i = 0; i < MAX_LINE_SIZE; i++) {
                    for (int j = 0; j < MAX_SECTION_SIZE; j++) {
                        if (attributes_array[i][j] == '\n') {
                            continue;
                        } else {
                            printf("%c", attributes_array[i][j]);
                        }
                    }

                }
                continue;
            }

        }
    }

    return 0;
}

static bool parse_noprocess(char *line, char const *term) {
    char *position = strstr(line, term);

    if (position) {
        stage = PROCESS_STARTED;
        unsigned long strip_pos = (position - line) + sizeof(term) + 3;
        for (unsigned long i = strip_pos; i < MAX_LINE_SIZE; i++) {
            if (line[i] == '\n' || line[i] == '\0') {
                break;
            } else {
                printf("%c", line[i]);
            }
        }
        return true;
    }

    return false;
}

static bool parse_attributes(char *line, char const *term) {
    char *position = strstr(line, term);
    if (position) {
        stage = ATTRIBUTES_IN_PROCESS;
        unsigned long strip_pos = (position - line) + sizeof(term) + 4;
        for (unsigned long i = strip_pos; i < MAX_LINE_SIZE; i++) {
            if (line[i] == '\n' || line[i] == '\0') {
                break;
            }
        }

        return true;
    }

    return false;
}

void write_to_stdout(char line[], char const *term) {
    char *position = strstr(line, term);
    if (!position && stage == PROCESS_OPEN) {
        printf("%s", line);
    } else {
        stage = PROCESS_CLOSED;
    }
}

void write_to_arr(char line[], char const *term) {
    char *position = strstr(line, term);
    int status = 0;
    attributes_counter++;
    if (!position) {
        stage = ATTRIBUTES_PROCESSING;
        for (int i = attributes_counter; i < MAX_LINE_SIZE; i++) {
            if (status == 1) {
                continue;
            }
            for (int j = 0; (j < MAX_SECTION_SIZE) && status != 1; j++) {
                attributes_array[i][j] = line[j];

                if (line[j] == '\n') {
                    status = 1;
                    continue;
                }

                if (line[j] == '\0') {
                    stage = ATTRIBUTES_PROCESSED;
                    status = 1;
                    break;
                }
            }
        }
    } else {
        stage = ATTRIBUTES_PROCESSED;
    }
}

void clean_up(char line[][MAX_SECTION_SIZE]) {
    memset(line, 0, sizeof(line[0][0]) * MAX_LINE_SIZE * MAX_SECTION_SIZE);
}



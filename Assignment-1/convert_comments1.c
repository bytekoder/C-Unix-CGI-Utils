#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

int main() {

    int reader = 0;
    int isCommentAnticipated = false;
    int isCommentFound = false;
    int hasCommentEnded = false;
    int isAlreadyCommented = false;
    char line[MAX_SIZE];
    char prev;

    //Loop until EOF
    while ((reader = getchar()) != EOF) {
        ungetc(reader, stdin);
        // Loop until EOL
        if (fgets(line, MAX_SIZE, stdin)) {

            for (int i = 0; i < MAX_SIZE; i++) {

                while (line[i] != '/' && !isCommentAnticipated && !isCommentFound) {
                    i++;
                }

                if (line[i] == '/' && !isCommentAnticipated) {
                    isCommentAnticipated = true;
                    prev = line[i];
                    continue;
                }

                if (prev != line[i]){
                    isCommentAnticipated = false;
                    isAlreadyCommented= true;
                }

                if (prev == '/' && line[i] == '*'){
                    break;
                }


                if (line[i] == '/' && isCommentAnticipated && prev == '/') {
                    isCommentFound = true;
                    line[i] = '*';
                }

                // Means everything is a comment
                if (isCommentAnticipated && isCommentFound && line[i] == '\n') {
                    line[i] = ' ';
                    line[++i] = '*';
                    line[++i] = '/';
                    line[++i] = '\n';
                    hasCommentEnded = true;
                }

            }

            if (hasCommentEnded || isAlreadyCommented) {

                printf("%s", line);
                isCommentAnticipated = false;
                isCommentFound = false;
                hasCommentEnded = false;
            }

        }
    }

    return 0;
}



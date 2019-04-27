//
// Created by Bhavani Shekhawat
//

//TR=002;dir=i;day=m-f;TI=05:30;stn=bridgewater;Line=middleborough

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100
#define FIRST_CHAR "T"

int main() {

    char line[MAX_SIZE];
    int shouldSkipLine = false;
    int isTagFound = false;
    int firstHr;
    int secondHr;
    int firstMin;
    int secondMin;
    char prev;
    char seperator;

    // Loop until EOF
    while ((getchar()) != EOF) {

        // Loop until EOL
        if (fgets(line, MAX_SIZE, stdin)) {

            // Loop for every line
            for (int i = 0; i < MAX_SIZE; i++) {

                // Keep looping until we dont sense the right tag
                while (line[i] != 'I' && prev != 'I') {
                    i++;
                }

                // Store the first char of the anticipated tag
                // Once matched go to the next char
                if (line[i] == 'I') {
                    prev = line[i];
                    continue;
                }

                // Match the combination of "I="
                // Once matched go to the next char
                if (prev == 'I' && line[i] == '=') {
                    isTagFound = true;
                    continue;
                }

                // Stores the time data
                if (isTagFound && line[i] != '\n') {

                    //Convert them to integers from char
                    firstHr = line[i] - '0';
                    secondHr = line[++i] - '0';
                    seperator = line[++i] - '0';
                    firstMin = line[++i] - '0';
                    secondMin = line[++i] - '0';

                }

                // Check for valid hours/min
                if ((firstHr > 2) || (firstHr >= 2 && secondHr > 3) || (firstMin > 5)) {
                    shouldSkipLine = true;
                }

                // Push the counter to the end. Since we already found the line...
                i = MAX_SIZE - 1;
            }


            // Print the line if the flag was true
            if (shouldSkipLine) {
                printf(FIRST_CHAR);
                printf("%s", line);
            }

            // Setting flags and variables to default
            isTagFound = false;
            shouldSkipLine = false;
            prev = '\0';

        }
    }

    return 0;

}
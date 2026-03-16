#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define MAX_LINE_LENGTH 1024

void find_labels(FILE* mpf) {
    char line[MAX_LINE_LENGTH];
    regex_t regex;
    int reti;

    // Compile regex:
    // ^[a-zA-Z_]      -> Starts with a letter or underscore
    // [a-zA-Z0-9_]* -> Followed by any number of alphanumeric/underscores
    // :               -> Followed by a colon
    reti = regcomp(&regex, "^([a-zA-Z_][a-zA-Z0-9_]*):", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    printf("Matches found:\n");
    while (fgets(line, sizeof(line), mpf)) {
        // Execute regex match
        reti = regexec(&regex, line, 0, NULL, 0);
        if (!reti) {
            printf("Found Label Line: %s", line);
        }
    }

    // Free memory allocated to the pattern buffer
    regfree(&regex);
}

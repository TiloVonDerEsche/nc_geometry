#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define MAX_LINE_LENGTH 1024

void find_labels(FILE* mpf) {
    char line[MAX_LINE_LENGTH];
    regex_t regex;
    int reti;

    reti = regcomp(&regex, "^([A-Z_][A-Za-z0-9_]*):", REG_EXTENDED);
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

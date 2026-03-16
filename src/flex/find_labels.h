#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_LENGTH 1024

void find_labels(strfloat_t* h, FILE* mpf) {
    char line[MAX_LINE_LENGTH];
    char clean_line[MAX_LINE_LENGTH];
    int reti;
    regex_t regex;
    regmatch_t pmatch[2];


    reti = regcomp(&regex, "([A-Z_][A-Za-z0-9_]*):", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    printf("Matches found:\n");
    size_t li = 1;
    long byte_offset = 0;
    while (fgets(line, sizeof(line), mpf)) {
        strncpy(clean_line, line, MAX_LINE_LENGTH);
        char *comment_ptr = strchr(clean_line, ';');
        if (comment_ptr) {
            *comment_ptr = '\0'; // Terminate string at the semicolon
        }

        // If the line contains a quote before a colon, it's likely a string
        char *quote_ptr = strchr(clean_line, '"');
        char *colon_ptr = strchr(clean_line, ':');


        // Execute regex match
        if (quote_ptr == NULL || (colon_ptr != NULL && colon_ptr < quote_ptr)) {
          reti = regexec(&regex, clean_line, 2, pmatch, 0);
          if (!reti) {
            // pmatch[0].rm_eo is the byte offset relative to the START of 'line'
            // where the match (the colon) ends.
            long byoffset_colon = byte_offset + pmatch[0].rm_eo;

            int start = pmatch[1].rm_so; // Start Offset
            int end = pmatch[1].rm_eo;   // End Offset
            int length = end - start;

            char label_id[MAX_LINE_LENGTH];
            strncpy(label_id, &clean_line[start], length);
            label_id[length] = '\0';

            //----Save byte_offset and label_line in hashmap
            printf("Line %zu | Offset %ld | ID: '%s' | Content: %s",
              li, byoffset_colon, label_id, line);
          }
        }

        // Update the global file offset for the next line
        // ftell is the most reliable way to handle varying line lengths
        byte_offset = ftell(mpf);
        li++;
    }

    // Free memory allocated to the pattern buffer
    regfree(&regex);
}

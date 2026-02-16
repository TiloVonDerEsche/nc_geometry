#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"  // You'll need the cJSON library[](https://github.com/DaveGamble/cJSON)

int is_empty_or_comment(const char *line) {
    // Trim leading/trailing whitespace
    while (*line == ' ' || *line == '\t' || *line == '\n' || *line == '\r') line++;
    if (*line == '\0') return 1;  // Empty
    if (*line == '#' || (*line == '/' && *(line + 1) == '/')) return 1;  // Comment
    return 0;
}

int main() {
    // Simulate .mpf file lines (replace with FILE *fp = fopen("yourfile.mpf", "r"); and fgets in loop)
    const char *lines[] = {
        "",                          // Line 1: empty (skipped)
        "# This is a comment",       // Line 2: comment (skipped)
        "attr1=val1; attr2=val2;",   // Line 3: multiple attrs
        "// Another comment",        // Line 4: comment (skipped)
        "name=Alice;",               // Line 5: one attr
        "",                          // Line 6: empty (skipped)
        "age=30; score=85.5;",       // Line 7: multiple attrs
        "just a line no attrs",      // Line 8: valid but none attrs (empty object)
        NULL                         // End
    };

    cJSON *root = cJSON_CreateObject();  // Root is a JSON object with line_num as string keys

    int line_num = 1;
    int i = 0;
    while (lines[i]) {
        char line_copy[1024];  // Copy for modification
        strcpy(line_copy, lines[i]);

        // Trim the line
        char *start = line_copy;
        while (*start == ' ' || *start == '\t') start++;
        size_t len = strlen(start);
        while (len > 0 && (start[len - 1] == ' ' || start[len - 1] == '\t' || start[len - 1] == '\n' || start[len - 1] == '\r')) {
            start[--len] = '\0';
        }

        if (is_empty_or_comment(start)) {
            // Skip: no entry for this line (causes index jumps)
        } else {
            // Create object for this line (even if empty)
            cJSON *obj = cJSON_CreateObject();

            // Pretend parsing: split by ';' for multiple attr=value pairs
            // (Adapt this to your actual .mpf line parsing logic)
            char *token = strtok(start, ";");
            while (token) {
                // Trim token
                char *tstart = token;
                while (*tstart == ' ' || *tstart == '\t') tstart++;

                // Find '=' and split into key/value
                char *eq = strchr(tstart, '=');
                if (eq) {
                    *eq = '\0';
                    char *key = tstart;
                    char *val = eq + 1;
                    while (*val == ' ' || *val == '\t') val++;
                    cJSON_AddStringToObject(obj, key, val);  // Add attr:value
                }
                // Else: ignore malformed, or handle "none" case (obj remains empty)

                token = strtok(NULL, ";");
            }

            // Add to root with line_num as key (string)
            char key[32];
            sprintf(key, "%d", line_num);
            cJSON_AddItemToObject(root, key, obj);
        }

        line_num++;
        i++;
    }

    // Write to JSON file
    char *json_str = cJSON_Print(root);
    FILE *f = fopen("dynamic_data.json", "w");
    if (f) {
        fprintf(f, "%s\n", json_str);
        fclose(f);
    }
    free(json_str);
    cJSON_Delete(root);

    printf("JSON file written. Example output:\n%s\n", json_str);  // For demo

    return 0;
}

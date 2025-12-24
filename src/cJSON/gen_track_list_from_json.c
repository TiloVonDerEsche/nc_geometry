#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"

typedef struct {
    double x, y, z;
} Point;

// Helper to safely get double values from the numeric strings in your JSON
double get_json_double(cJSON *item, const char *key, double current_val) {
    cJSON *attr = cJSON_GetObjectItemCaseSensitive(item, key);
    if (cJSON_IsString(attr) && (attr->valuestring != NULL)) {
        return atof(attr->valuestring);
    }
    return current_val; // Return existing value if key is missing
}

int main() {
    // 1. Load the file into memory
    printf("Opening hmhis.json in binary read mode...\n");
    FILE *f = fopen("hmhis.json", "rb");
    if (!f) return 1;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    fclose(f);
    data[len] = '\0';

    // 2. Parse JSON Array
    puts("Parsing JSON Array...");
    cJSON *root = cJSON_Parse(data);
    if (!root) {
        printf("Error parsing JSON\n");
        return 1;
    }

    puts("Opening tracks.csv in write mode...");
    FILE *csv = fopen("tracks.csv", "w");
    fprintf(csv, "track_index,Ax,Ay,Az,Bx,By,Bz\n");

    int track_index = 0;
    double cur_x = 0, cur_y = 0, cur_z = 0;
    double prev_x = 0, prev_y = 0, prev_z = 0;
    int cur_laser = 0, prev_laser = 0;
    Point A = {0, 0, 0};

    cJSON *line_item = NULL;
    cJSON_ArrayForEach(line_item, root) {
        // Update coordinates only if they exist in this line (persistence)
        cur_x = get_json_double(line_item, "X", cur_x);
        cur_y = get_json_double(line_item, "Y", cur_y);
        cur_z = get_json_double(line_item, "Z", cur_z);

        // Get current laser state
        cur_laser = (int)get_json_double(line_item, "laser", (double)cur_laser);

        printf("(%f, %f, %f), %d\n",cur_x,cur_y,cur_z,cur_laser);

        // Detect Point A: Switch from 0 to 1
        // We capture the values from the previous line
        if (prev_laser == 0 && cur_laser == 1) {
            puts("Point A saved!");
            A.x = prev_x;
            A.y = prev_y;
            A.z = prev_z;
        }

        // Detect Point B: Switch from 1 to 0
        // Capture previous values and write the completed track
        if (prev_laser == 1 && cur_laser == 0) {
            puts("Track saved to csv!");
            fprintf(csv, "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                    track_index++,
                    A.x, A.y, A.z,
                    prev_x, prev_y, prev_z);
        }

        // Move current to previous for the next iteration
        prev_x = cur_x;
        prev_y = cur_y;
        prev_z = cur_z;
        prev_laser = cur_laser;
    }

    // Cleanup
    cJSON_Delete(root);
    free(data);
    fclose(csv);
    printf("CSV generated: %d tracks found.\n", track_index);

    return 0;
}

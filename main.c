#include "helper_funcs.h"
#include "extract_points.c"
#include "track_collision.c"
#include <ctype.h>

typedef struct {
    size_t mpf_lines;
    size_t precision;
    size_t max_line_len;

    char file_name[256];
    char output_csv_name[256];

    float horizontal_radius;
    float vertical_radius;
} Config;

// Function to trim whitespace and quotes from a string
char* trim(char* str) {
    char* end;
    // Remove leading whitespace and quotes
    while (isspace((unsigned char)*str) || *str == '"') str++;
    if (*str == 0) return str;
    // Remove trailing whitespace, quotes, and semicolon
    end = str + strlen(str) - 1;
    while (end > str && (isspace((unsigned char)*end) || *end == '"' || *end == ';')) end--;
    *(end + 1) = '\0';
    return str;
}

// Function to parse the config file
int read_config(const char* filename, Config* config) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open config file %s\n", filename);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        // Skip empty lines or comments
        char* trimmed = trim(line);
        if (strlen(trimmed) == 0 || trimmed[0] == '/' || trimmed[0] == '#') {
            continue;
        }

        // Split line into key and value
        char* key = strtok(trimmed, "=");
        char* value = strtok(NULL, ";");
        if (!key || !value) {
            fprintf(stderr, "Error: Invalid line format: %s\n", line);
            continue;
        }

        key = trim(key);
        value = trim(value);

        // Assign values to config struct
        if (strcmp(key, "mpf_lines") == 0) {
            config->mpf_lines = atoi(value);
        } else if (strcmp(key, "precision") == 0) {
            config->precision = atoi(value);
        } else if (strcmp(key, "max_line_len") == 0) {
            config->max_line_len = atoi(value);
        } else if (strcmp(key, "file_name") == 0) {
            strncpy(config->file_name, value, sizeof(config->file_name) - 1);
            config->file_name[sizeof(config->file_name) - 1] = '\0';
        } else if (strcmp(key, "output_csv_name") == 0) {
            strncpy(config->output_csv_name, value, sizeof(config->output_csv_name) - 1);
            config->output_csv_name[sizeof(config->output_csv_name) - 1] = '\0';
        } else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        } else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        } else {
            fprintf(stderr, "Warning: Unknown key: %s\n", key);
        }
    }

    fclose(file);
    return 0;
}

int main() {
    // Initialize config struct with default values
    Config config = {0};

    // Read the config file
    if (read_config("config.txt", &config) != 0) {
      return 1;
    }

    // Print the values to verify
    printf("mpf_lines=%zu\n", config.mpf_lines);
    printf("precision=%zu\n", config.precision);
    printf("max_line_len=%zu\n", config.max_line_len);
    printf("file_name=%s\n", config.file_name);
    printf("output_csv_name=%s\n", config.output_csv_name);
    printf("horizontal_radius=%.2f\n", config.horizontal_radius);
    printf("vertical_radius=%.2f\n", config.vertical_radius);


    data_tuple* cords = calloc(config.mpf_lines,sizeof(data_tuple));
    if (cords == NULL) {
      fprintf(stderr, "Memory allocation of cords failed\n");
        exit(EXIT_FAILURE);
    }


    track* track_list = calloc(config.mpf_lines,sizeof(track));
    if (track_list == NULL) {
      fprintf(stderr, "Memory allocation of track list failed\n");
        exit(EXIT_FAILURE);
    }


    size_t track_list_len = -1;
    float laser_power = -1;
    float machine_speed = -1;

    read_mpf(
      config.file_name,
      config.mpf_lines,
      config.max_line_len,
      &cords, &track_list, &track_list_len, &laser_power, &machine_speed
    );

    printf("track_list_len=%lld\
            \rlaser_power=%f\
            \rmachine_speed=%f\n\r",
            track_list_len,laser_power,machine_speed);

    //Set horizontal and vertical radius
    init_track_list(
      config.horizontal_radius,
      config.vertical_radius,
      track_list_len, &track_list);

    track_collision(track_list_len, &track_list);

    write_tracks_to_csv(
      config.output_csv_name,
      track_list_len,&track_list);

    free(cords);
    free(track_list);
    return 0;
}

#include "helper_funcs.h"
#include "extract_points.c"
#include "track_collision.c"

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
    printf("vertical_radius=%.2f\n\n", config.vertical_radius);



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

    printf("\ntrack_list_len=%lld\n\
            \rlaser_power=%f\n\
            \rmachine_speed=%f\n\n\r",
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

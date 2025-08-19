#include "helper_funcs.h"
#include "file_funcs.h"
#include "track_collision.h"

int main() {
    // Initialize config struct with default values
    Config config = {0};

    // Read the config file
    if (read_config("config.txt", &config) != 0) {
      return 1;
    }
    puts("\nRead values from config.txt:");
    // Print the config.txt values to verify
    printf("mpf_lines=%zu\n", config.mpf_lines);
    printf("precision=%zu\n", config.precision);
    printf("max_line_len=%zu\n", config.max_line_len);

    printf("\nmpf_file=%s\n", config.mpf_file);
    printf("data_tuples_csv=%s\n", config.data_tuples_csv);
    printf("track_list_csv=%s\n", config.track_list_csv);

    printf("\nhorizontal_radius=%.2f\n", config.horizontal_radius);
    printf("vertical_radius=%.2f\n\n", config.vertical_radius);



    data_tuple* tuple_list = calloc(config.mpf_lines,sizeof(data_tuple));
    if (tuple_list == NULL) {
      fprintf(stderr, "Memory allocation of tuple_list failed\n");
        exit(EXIT_FAILURE);
    }


    track* track_list = calloc(config.mpf_lines,sizeof(track));
    if (track_list == NULL) {
      fprintf(stderr, "Memory allocation of track list failed\n");
        exit(EXIT_FAILURE);
    }


    size_t track_list_len = -1;
    read_mpf(
      config.mpf_file,
      config.mpf_lines,
      config.max_line_len,
      &tuple_list, &track_list, &track_list_len, &config
    );


    set_track_radius(
      config.horizontal_radius,
      config.vertical_radius,
      track_list_len, &track_list);

    printf("Using track_list with %lld tracks for collision...\n",track_list_len);
    track_collision(track_list_len, &track_list);

    printf("\nSaving track_list in %s...\n",config.track_list_csv);
    write_tracks_to_csv(
      config.track_list_csv,
      track_list_len,&track_list);

    free(tuple_list);
    free(track_list);
    return 0;
}

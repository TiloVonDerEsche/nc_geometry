#include "helper_funcs.h"
#include "file_funcs.h"
#include "track_collision.h"
#include "interpolation.h"

int main() {
    // Initialize config struct with default values
    Config config = {0};

    // Read the config file
    if (read_config("config.txt", &config) != 0) {
      return 1;
    }


    puts("\nRead values from config.txt:");
    // Print the config.txt values to verify
    printf("lines_to_read=%zu\n", config.lines_to_read);
    printf("max_line_len=%zu\n", config.max_line_len);

    printf("\nmpf_file=%s\n", config.mpf_file);
    printf("data_tuples_csv=%s\n", config.data_tuples_csv);
    printf("track_list_csv=%s\n", config.track_list_csv);
    printf("interpolation_csv=%s\n", config.intpol_csv);

    printf("\nhorizontal_radius=%.2f\n", config.horizontal_radius);
    printf("vertical_radius=%.2f\n", config.vertical_radius);

    printf("\nstep_distance=%f\n\n",config.step_dis);

    //count lines of mpf
    FILE* mpf = fopen(config.mpf_file, "r");
    if (mpf == NULL) {
        fprintf(stderr, "Error: Could not open %s (in read mode)!\n", config.mpf_file);
        return 1;
    }
    config.mpf_lines = count_lines(mpf);
    fclose(mpf);

    if (config.lines_to_read > config.mpf_lines) {
      printf("%s got only %lu lines, but lines_to_read was set to %lu"
      ,config.mpf_file,config.mpf_lines,config.lines_to_read);

      if(config.lines_to_read == -1) {printf(", which is equal to -1");}
      puts("!");

      printf("Resetting lines_to_read to %lu...\n\n",config.mpf_lines);

      config.lines_to_read=config.mpf_lines;
    }



    data_tuple* tuple_list = (data_tuple*)calloc(config.mpf_lines,sizeof(data_tuple));
    if (tuple_list == NULL) {
      fprintf(stderr, "Memory allocation of tuple_list failed\n");
        exit(EXIT_FAILURE);
    }


    track* track_list = (track*)calloc(config.mpf_lines,sizeof(track));
    if (track_list == NULL) {
      fprintf(stderr, "Memory allocation of track list failed\n");
        exit(EXIT_FAILURE);
    }


    size_t track_list_len = -1;
    read_mpf(&tuple_list, &track_list, &track_list_len, &config);


    set_track_radius(
      config.horizontal_radius,
      config.vertical_radius,
      track_list_len, &track_list);

    printf("Using track_list with %lu tracks for collision...\n",track_list_len);
    track_collision(track_list_len, &track_list);

    printf("\nSaving track_list in %s...\n",config.track_list_csv);
    write_tracks_to_csv(
      config.track_list_csv,
      track_list_len,&track_list);


    puts("\n\n");
    //------Interpolation------//
    linear_intpol(track_list_len, &track_list, &config);

    free(tuple_list);
    free(track_list);
    return 0;
}

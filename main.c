#include "helper_funcs.h"
#include "extract_points.c"
#include "track_collision.c"


int main() {
    //---------------Parameters set by user---------------//
    size_t mpf_lines = 2500;
    //size_t precision = 10; //1000 yields ridiculous amount of csv lines
    size_t max_line_len = 1000;
    char* file_name = ".\\data\\ElGeo_5_V2_1.mpf";
    char* output_csv_name = ".\\data\\track_list.csv";

    //radii of welding track
    float horizonal_radius = 0.3;
    float vertical_radius = 0.3;
    //----------------------------------------------------//

    data_tuple* cords = calloc(mpf_lines,sizeof(data_tuple));
    if (cords == NULL) {
      fprintf(stderr, "Memory allocation of cords failed\n");
        exit(EXIT_FAILURE);
    }


    track* track_list = calloc(mpf_lines,sizeof(track));
    if (track_list == NULL) {
      fprintf(stderr, "Memory allocation of track list failed\n");
        exit(EXIT_FAILURE);
    }


    size_t track_list_len = -1;
    float laser_power = -1;
    float machine_speed = -1;

    read_mpf(file_name, mpf_lines, max_line_len, &cords, &track_list, &track_list_len, &laser_power, &machine_speed);

    printf("track_list_len=%lld\
            \rlaser_power=%f\
            \rmachine_speed=%f\n\r",
            track_list_len,laser_power,machine_speed);

    //Set horizontal and vertical radius
    init_track_list(horizonal_radius, vertical_radius, track_list_len, &track_list);

    track_collision(track_list_len, &track_list);

    write_tracks_to_csv(output_csv_name,track_list_len,&track_list);

    free(cords);
    free(track_list);
    return 0;
}

#include "helper_funcs.h"
#include "extract_points.c"
#include "track_collision.c"


int main() {
    size_t mpf_lines = 2500;
    //size_t precision = 10; //1000 yields ridiculous amount of csv lines
    size_t max_line_len = 1000;
    char* file_name = ".\\data\\ElGeo_5_V2_1.mpf";

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
    //float machine_speed =
    read_mpf_and_create_point_cloud(file_name, mpf_lines, max_line_len, &cords, &track_list, &track_list_len);

    printf("%lld",track_list_len);

    //Set horizontal and vertical radius
    init_track_list(0.3, 0.3, track_list_len, &track_list);

    track_collision(track_list_len, &track_list);

    write_tracks_to_csv(track_list_len,&track_list);

    free(cords);
    free(track_list);
    return 0;
}

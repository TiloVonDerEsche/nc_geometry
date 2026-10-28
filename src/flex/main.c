#include <stdio.h>
#include "helper.h"

extern Config config;
extern ubyte debug_bm;
extern strfloat_t* h;

extern FILE* hmhis;
extern FILE* tl;

extern void label_finder(FILE* fp);

extern FILE* yyin;
extern int yyparse();

//Read params from config
int main(int argc, char *argv[])
{
  // Read the config file
  const char* config_fname = "interpreter.cconf";
  if (read_config(config_fname, &config) != 0) {
    return 1;
  }
  printf("\nRead values from config, %s:\n",config_fname);
  // Print the config.ctxt values to verify
  printf("mpf_file=%s\n", config.mpf_file);
  printf("track_list_csv=%s\n", config.track_list_csv);
  printf("ncc_points_csv=%s\n", config.ncc_points_csv);

  h = init_hashmap();
  debug_bm = config.debug_bitmap;

  /*if(config.hmhis_to_file) {
    hmhis = init_file(config.hmhis_json,"[");
    setvbuf(hmhis, NULL, _IONBF, 0);  // disable buffering
  }*/

  tl = init_file(config.track_list_csv,"Track_id,\
    T_start_x,T_start_y,T_start_z,\
    T_end_x,T_end_y,T_end_z,\
    B,C,\
    machine_speed,\
    laser_power, laser_status,\
    G_code");
  ncc_points = init_file(config.ncc_points_csv, "Track_id,Point_id,x,y,z,\
    laser_power,machine_speed");

  exec(config.mpf_file); //set entry point -> Main_Program_File
  yyparse();

  //printf("%lu tracks written to %s!\n",tid,config.track_list_csv);
  if(config.hmhis_to_file) {close_hmhis();}
  fclose(tl);
  fclose(ncc_points);
  return 0;
}

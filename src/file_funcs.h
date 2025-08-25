#include "./read_mpf_refactor.c"

void set_track_radius(float hradius, float vradius, size_t track_list_len, track** tl) {
  for (size_t i = 0; i < track_list_len; i++) {
    (*tl)[i].hradius = hradius;
    (*tl)[i].vradius = vradius;
  }
}

void write_tracks_to_csv(char* csv_path, size_t track_list_len, track** tl) {
  printf("Writing to csv with path: %s...\n",csv_path);
  FILE* file = fopen(csv_path, "w");
  if (file == NULL) {
      fprintf(stderr, "An error occured while trying to open %s (in write mode)!",csv_path);
      return;
  }

  fprintf(file,"Track_id, A.x, A.y, A.z, B.x, B.y, B.z, laser_power, machine_speed, coll_vec.x, coll_vec.y, coll_vec.z, hradius, vradius\n");

  for (size_t i = 0; i < track_list_len; i++) {
    fprintf(file,"%lu, %f,%f,%f, %f,%f,%f, %f,%f, %f,%f,%f, %f,%f\n",
           i,
           (*tl)[i].A.x, (*tl)[i].A.y, (*tl)[i].A.z,
           (*tl)[i].B.x, (*tl)[i].B.y, (*tl)[i].B.z,
           (*tl)[i].laser_power,(*tl)[i].machine_speed,
           (*tl)[i].coll_vec.x,(*tl)[i].coll_vec.y,(*tl)[i].coll_vec.z,
           (*tl)[i].hradius,(*tl)[i].vradius);
  }
}

void read_mpf (
  char mpf_path[], size_t mpf_lines, size_t max_line_len, uint8_t read_all,
  data_tuple** tuple_list, track** tl, size_t* tl_len,
  Config* config){



  printf("Opening: %s in read mode...\n",mpf_path);
  FILE* file = fopen(mpf_path, "r");
  if (file == NULL) {
      fprintf(stderr, "Error: Could not open %s (in read mode)!\n", mpf_path);
      return;
  }

  //open csv file for writing the data_tuples there
  FILE* csv_file = fopen(config->data_tuples_csv, "w"); // or "a" to append
  if (csv_file == NULL){
      fprintf(stderr, "An error occured, while trying to open %s (in write mode)!\n", config->data_tuples_csv);
      return;
  }

  fprintf(csv_file, "P.x,P.y,P.z, laser_on_off, laser_power, machine_speed\n");

  uint8_t laser_on_off = 0;
  float puis_laser = 0; //laser_power
  float vit_tir = 0; //machine_speed

  char format[] = " %*[^-0123456789]%lf";

  double read_val = 0;
  //init first point, because the successor needs its predeceding point to copy
  (*tuple_list)[0].P.x = 0;
  (*tuple_list)[0].P.y = 0;
  (*tuple_list)[0].P.z = 0;

  vec3D new_P = {0,0,0};

  //use last 3 bits as bool which dimension changed (X,Y,Z)
  uint8_t dim_changed = 0b00000000; //0b00000111

  const char* keywords[] = {
    "/LASER_ON",
    "/LASER_OFF",
    "PUIS_LASER",
    "VIT_TIR=",
    "X",
    "Y",
    "Z",
    "G"
  };

  printf("Reading: %s...\n",mpf_path);

  //////////////////////////////////////////////////////////////////////////////
  //----------------------NC Code Interpreter Reading Loop--------------------//
  //////////////////////////////////////////////////////////////////////////////
  size_t ti = 0; //track id
  size_t i = 0; //data_tuple id (for tuple_list)
  //size_t li = 0;
  //continue as long as the line contains something
  char line_buf[max_line_len];
  char buf[max_line_len];
  char read_value[max_line_len];
  char c = '\0';
  size_t bi = 0;
  size_t vi = 0;

  //fgets prints the new line in line_buf, and keeps a line index count
  //read mpf linewise
  while ((fgets(line_buf, max_line_len, file) != NULL) && (i < mpf_lines || read_all)) {
      //line_buf and line are currently always equal
      printf("line_buf=%s",line_buf);

      bi = 0;
      vi = 0;
      //read line charwise
      for (size_t li=0; li < max_line_len; li++){
        c = line_buf[li];

        printf("Checking %s as keyword...",buf);
        if (is_in_list(trim(buf), keywords, 7)) {
          puts("KEYWORD FOUND!");
        }


        if (is_part_of_num(c)) {
            //check if current buf is matching to a keyword

            //parse value
            read_value[vi] = c;
            vi++;

            //printf("read_value=%sEND\n",read_value);
        }
        else {
          buf[bi] = c;
          bi++;

          //clear buffer
          read_value[0] = '\0';
          vi = 0;
        }
        printf("%c\n",c);

        if (c == '\0' || c == ';' || c == ' ' || c == '\n') {
            //buf[0] = '\0';
            memset(buf, 0, max_line_len);
            bi = 0;
            if (c == '\0' || c == ';') {break;}
        }

      }



      // if (x_has_changed == 1 || y_has_changed == 1 || z_has_changed == 1){
      //
      //     //"X10.0" in the NC means copy values of the old point and
      //     //change X to 10.0
      //     copy_data_tuple(i,tuple_list);
      //
      //     //then only change the parameters that have changed
      //     if (x_has_changed) (*tuple_list)[i].P.x = new_P.x;
      //     if (y_has_changed) (*tuple_list)[i].P.y = new_P.y;
      //     if (z_has_changed) (*tuple_list)[i].P.z = new_P.z;
      //line
      //     (*tuple_list)[i].laser = laser_on_off;
      //     (*tuple_list)[i].laser_power = puis_laser;
      //     (*tuple_list)[i].machine_speed = vit_tir;
      //
      //     fprintf(csv_file, "(%f, %f, %f), %s, %f, %f\n",
      //     (*tuple_list)[i].P.x, (*tuple_list)[i].P.y, (*tuple_list)[i].P.z,
      //     (*tuple_list)[i].laser ? "on" : "off", (*tuple_list)[i].laser_power,
      //     (*tuple_list)[i].machine_speed);
      //
      //     i += 1;
      // }
  }

  //save the length of tl (track_list) arr in tl_len
  (*tl_len) = ti;

  //close the mpf file
  fclose(file);
  fclose(csv_file);
  printf("Points from %s, were saved in %s!\n",mpf_path,config->data_tuples_csv);
}




int read_config(const char* filename, Config* config) {
    printf("Opening %s...\n",filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open config file %s\n", filename);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

        //ignore comments and empty lines
        if (strlen(trimmed) == 0 || trimmed[0] == '/' || trimmed[0] == '#') {
            continue;
        }
        char* key;
        char* value;
        parse_line(trimmed, &key, &value);
        if (!key || !value) {
            fprintf(stderr, "Error: Invalid line format: %s\n", line);
            continue;
        }

        if (strcmp(key, "mpf_lines") == 0) {
            config->mpf_lines = (size_t)atoi(value);
        }
        else if (strcmp(key, "precision") == 0) {
            config->precision = (size_t)atoi(value);
        }
        else if (strcmp(key, "max_line_len") == 0) {
            config->max_line_len = (size_t)atoi(value);
        }
        else if (strcmp(key, "mpf_file") == 0) {
            strncpy(config->mpf_file, value, sizeof(config->mpf_file) - 1);
            config->mpf_file[sizeof(config->mpf_file) - 1] = '\0';
        }
        else if (strcmp(key, "data_tuples_csv") == 0) {
            strncpy(config->data_tuples_csv, value, sizeof(config->data_tuples_csv) - 1);
            config->data_tuples_csv[sizeof(config->data_tuples_csv) - 1] = '\0';
        }
        else if (strcmp(key, "track_list_csv") == 0) {
            strncpy(config->track_list_csv, value, sizeof(config->track_list_csv) - 1);
            config->track_list_csv[sizeof(config->track_list_csv) - 1] = '\0';
        }
        else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        }
        else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        }
        else if (strcmp(key, "tracks_to_plot") == 0) {
          //Do nothing; Used by t_vis_color.c
        }
        else {
            fprintf(stderr, "Warning: Unknown key: %s in config file: %s!\n", key,filename);
        }
    }

    fclose(file);
    return 0;
}

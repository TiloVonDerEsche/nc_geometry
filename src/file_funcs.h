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

  fprintf(file,
  "track_id,"
  "track_start_x, track_start_y, track_start_z,"
  "track_end_x, track_end_y, track_end_z,"
  "laser_power_w, machine_speed_mm_per_min,"
  "coll_vec_x, coll_vec_y, coll_vec_z,"
  "track_hradius, track_vradius,"
  "track_direction_deg1, track_direction_deg2\n");

  for (size_t i = 0; i < track_list_len; i++) {
    fprintf(file,"%lu, %f,%f,%f, %f,%f,%f, %f,%f, %f,%f,%f, %f,%f\n",
           (i+1),
           (*tl)[i].SP.x, (*tl)[i].SP.y, (*tl)[i].SP.z,
           (*tl)[i].EP.x, (*tl)[i].EP.y, (*tl)[i].EP.z,
           (*tl)[i].laser_power,(*tl)[i].machine_speed,
           (*tl)[i].coll_vec.x,(*tl)[i].coll_vec.y,(*tl)[i].coll_vec.z,
           (*tl)[i].hradius,(*tl)[i].vradius);
  }
}

void read_mpf (uint8_t read_all,
  data_tuple** dtuple_list, track** track_list, size_t* tl_len,
  Config* config){
  //set config for mpf_lines, mpf_file path, data_tuples_csv


  printf("Opening: %s in read mode...\n",config->mpf_file);
  FILE* mpf = fopen(config->mpf_file, "r");
  if (mpf == NULL) {
      fprintf(stderr, "Error: Could not open %s (in read mode)!\n", config->mpf_file);
      return;
  }

  //open csv file for writing the data_tuples there
  FILE* dtuple_csv = fopen(config->data_tuples_csv, "w"); // or "a" to append
  if (dtuple_csv == NULL){
      fprintf(stderr, "An error occured, while trying to open %s (in write mode)!\n", config->data_tuples_csv);
      return;
  }

  fprintf(dtuple_csv, "di, ti, g, p_x,p_y,p_z, laser_on_off, laser_power_w, machine_speed_mm_per_min\n");

  //init first point, because the successor needs its predeceding point to copy
  (*dtuple_list)[0].P.x = 0;
  (*dtuple_list)[0].P.y = 0;
  (*dtuple_list)[0].P.z = 0;


  const size_t keywords_len = 8;
  const char* keywords[] = {
    "G",
    "X",
    "Y",
    "Z",
    "/LASER_ON",
    "/LASER_OFF",
    "PUIS_LASER "
    //"VIT_TIR="
  };

  printf("Reading %ld lines from %s...\n",config->mpf_lines,config->mpf_file);

  //////////////////////////////////////////////////////////////////////////////
  //----------------------NC Code Interpreter Reading Loop--------------------//
  //////////////////////////////////////////////////////////////////////////////

  //str_float hashmap for variables
  puts("Initializing str_float_map...");
  strfloat_t* h = strfloat_init();  // Create hashmap

  int absent;
  int keypos = -1;

  //hashmap indices
  khint_t kl; //index left token
  khint_t kr; //index right token

  size_t ti = 0; //track id
  size_t di = 0; //data_tuple id
  size_t bi = 0;
  size_t vi = 0;

  uint8_t feat_change = 0; //8bit bool for feature changes
  //uint8_t read_num_mode = 0;
  uint8_t laser_on_off = 0;

  char line_buf[config->max_line_len];
  char token_buf[config->max_line_len];
  char read_num_buf[config->max_line_len];
  char keyword_buf[config->max_line_len];
  char value_buf[config->max_line_len];

  char* key;
  char* value;
  char* trimmed;
  char* char_ptr;

  float machine_speed = -1;
  float puis_laser = 0; //laser_power
  float vit_tir = 0; //machine_speed

  while ((fgets(line_buf, config->max_line_len, mpf) != NULL) && (di < config->mpf_lines || read_all)) {
      feat_change = 0;
      char_ptr = line_buf; //start of the line


      while (*char_ptr != '\0' && *char_ptr != ';') {

          skip_spaces(&char_ptr);

          if (*char_ptr == '\0' || *char_ptr == ';') break;


          int i = 0;
          while (isalnum((unsigned char)*char_ptr) || *char_ptr == '/' || *char_ptr == '_') {
              keyword_buf[i++] = *char_ptr++;}
          keyword_buf[i] = '\0';

          if (i == 0) { // No keyword found, maybe just a number or symbol
              char_ptr++; continue;}

          skip_spaces(&char_ptr);

          // Check for delimiter
          if (*char_ptr == '=') {
              // It's a variable assignment
              char_ptr++; // Move past '='

              //read value on right side of '='
              while
              (is_part_of_num((unsigned char)*char_ptr) ||
                isalnum((unsigned char)*char_ptr) ||
                *char_ptr == '_') {
                  value_buf[i++] = *char_ptr++;}
              value_buf[i] = '\0';


              //check if value is a literal or another var
              if(is_valid_literal(value_buf) {
                kh_val(h, kl) = atof(value);
              } else if( is_valid_varname(value_buf) ) {
                kr = strfloat_get(h, value);

                if (kr < kh_end(h)) {
                    kh_val(h, kl) = kh_val(h, kr);}
              } else {
                fprintf(stderr,
                  "Line %lu:"
                  "Token on the right of '=' is neither a valid literal or varname!",
                  li);
              }




                //lookup value of var

              }

              //handle hashmap logic ...
          } else if (is_part_of_num(*char_ptr)) {
              // It's a command with a number
              // ... read number into value_buf ...
              // call set_key_value(keyword_buf, value_buf, ...)
          } else {
              // It's a standalone command like /LASER_ON
              // call set_key_value(keyword_buf, "", ...)
          }

      }

      // ... handle feat_change logic after this line ...
  }

  //save the length of tl (track_list) arr in tl_len
  (*tl_len) = ti;

  //close the mpf file
  fclose(mpf);
  fclose(dtuple_csv);
  printf("Points from %s, were saved in %s!\n",config->mpf_file,config->data_tuples_csv);


  //-----str_float variable hashmap

  //Print all variables from hashmap
  puts("-----");
  kh_foreach(h, k) {
      if (k < kh_end(h)) {  // Check if found (k != end iterator)
          printf("var %s=%f\n", kh_key(h, k), kh_val(h, k));
      }
  }
  puts("-----");

  //Cleanup str_float variable hashmap
  kh_foreach(h, k) {
      free((char*)kh_key(h, k));  // Free duplicated strings
  }
  strfloat_destroy(h);
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
            fprintf(stderr,
              "Error: Invalid line format: %s!\
              \r\nLine should contain <key>'='<value>;",line);
            continue;
        }

        if (strcmp(key, "mpf_lines") == 0) {
            config->mpf_lines = (size_t)atoi(value);
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
        else if (strcmp(key, "interpolation_csv") == 0) {
            strncpy(config->intpol_csv, value, sizeof(config->intpol_csv) - 1);
            config->intpol_csv[sizeof(config->intpol_csv) - 1] = '\0';
        }
        else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        }
        else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        }
        else if (strcmp(key, "step_distance") == 0) {
            config->step_dis = atof(value);
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

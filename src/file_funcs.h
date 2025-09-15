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

  uint8_t laser_on_off = 0;
  float puis_laser = 0; //laser_power
  float vit_tir = 0; //machine_speed

  char format[] = " %*[^-0123456789]%lf";

  double read_val = 0;
  //init first point, because the successor needs its predeceding point to copy
  (*dtuple_list)[0].P.x = 0;
  (*dtuple_list)[0].P.y = 0;
  (*dtuple_list)[0].P.z = 0;

  //vec3D new_P = {0,0,0};

  //8 bools: using only 7 bit for feature changes
  uint8_t feat_change = 0;


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
  strfloat_t *h = strfloat_init();  // Create empty map
  int absent;
   //khint_t is a portable unsigned int iterator, like size_t?
  khint_t k;
  khint_t k2;

  char* key;
  char* value;

  char* trimmed;
  //-------
  float machine_speed = -1;






  size_t ti = 0; //track id
  size_t di = 0; //data_tuple id (for dtuple_list)
  //size_t li = 0;


  char line_buf[config->max_line_len];
  //saves the chars of individual tokens / keywords while reading a line
  // gets cleared after reading ' ', '\n' etc
  char token_buf[config->max_line_len];

  char read_num_buf[config->max_line_len];
  char c = '\0';
  size_t bi = 0;
  size_t vi = 0;
  uint8_t read_num_mode = 0;
  int keypos = -1;

  //fgets prints the new line in line_buf, and keeps a line index count
  //continue as long as the line contains something
  //read mpf linewise
  while ((fgets(line_buf, config->max_line_len, mpf) != NULL) && (di < config->mpf_lines || read_all)) {
      //reset feat_change every line
      feat_change = 0; //TEST
      //printf("lbuf=%s",line_buf);

      //use str_float hashmap for variables
      //-------------------//
      //Does line contain '=' (might not be optimal solution)
      if (strchr(line_buf, '=') != NULL) { //does not work if '=' is in a comment
        //assign a variable mode
        trimmed = trim(line_buf); //remove whitespaces before, after & around '='
        parse_line(trimmed, &key, &value);



        //--------
        k = strfloat_put(h, trimmed, &absent);  // Key is a const char* pointer


        if (absent) {
            kh_key(h, k) = strdup(key);}

        //check if right token is a num or another var
        if (is_part_of_num(value[0])) {
            kh_val(h, k) = atof(value);}
        else { //lookup the value of the that var on the right of '='
            k2 = strfloat_get(h, value); // get iterator k
            if (k2 < kh_end(h)) {
                kh_val(h, k) = kh_val(h, k2);
            }
            //this works recursivly already?!
        }
      }
      //-------------------//
      else {
        bi = 0;
        vi = 0;
        //read line charwise
        for (size_t li=0; li < config->max_line_len; li++){
          c = line_buf[li];

          if (read_num_mode && is_part_of_num(c)) {
              //printf("IF li=%lu\n\n",li);
              //printf("read_num_mode on! Reading number for keyword %s...\n",token_buf);
              //check if current buf is matching to a keyword
              //parse value
              read_num_buf[vi] = c;
              vi++;

              //printf("read_num_buf=%s\n",read_num_buf);
          }
          else { // !(read_num_mode && is_part_of_num(c))
            //!read_num_mode || !is_part_of_num(c)
            //printf("ELSE li=%lu\n\n",li);


            //should be wrong for laser commands,
            //since they don't have a number afterwards
            //if (read_num_buf[0] != 0) {
              //printf("read_num_buf=%s\n",read_num_buf);

            //printf("keypos=%d, (keypos >= 0)=%s\n",keypos,(keypos >= 0) ? "true" : "false");
            if (keypos >= 0) {
              printf("Saving value %s for key %s with pos=%d...\n",
                            read_num_buf, token_buf, keypos);

              if (vi < config->max_line_len) {
                read_num_buf[vi] = '\0';}
              else {
                fprintf(stderr, "vi >= config->max_line_len\n"
                                "Meaning read_num_buf was overflown!\n"
                                "Exiting the program...\n");
                exit(-1);
              }

              set_key_value(keypos,read_num_buf,
                            di, &ti, dtuple_list, track_list,
                            &feat_change);

              keypos = -1;
              read_num_mode = 0;
              memset(read_num_buf, 0, config->max_line_len);
              vi = 0;
            }

            //TEST
            //don't clear read_num_buf btw keyword and number
            //clear if not inbetween token and buf is filled and not in num
            // if (c != ' ' && read_num_buf[0] != 0) {
            //   //printf("Clearing num_buf=%s...\n\n",read_num_buf);
            //   read_num_mode = 0;
            //   memset(read_num_buf, 0, config->max_line_len);
            //   vi = 0;
            // }


            //append char to token
            token_buf[bi] = c;
            bi++;
            token_buf[bi] = '\0';

            //printf("%c\n",c);

            //printf("Checking %s as keyword...\n",token_buf);
            keypos = is_in_list(token_buf, keywords, keywords_len);
            if (keypos != -1 && !read_num_mode) {
              //printf("KEYWORD %s with pos=%d FOUND!\n",token_buf,keypos);
              read_num_mode = 1;
            }


            if (c == '\0' || c == ';' || c == ' ' || c == '\n') {
                //token_buf[0] = '\0';
                memset(token_buf, 0, config->max_line_len);
                bi = 0;
                if (c == '\0' || c == ';') {break;}
            }
          }
        }
      }

      //See what bits of feat_change are still zero
      //Give the according dims the value of the last point






      if (feat_change > 0) {
        //puts("feat_change YES!");
        //feat_change=0b0%u%u%u%u%u%u%u
        printf("\ndi=%lu, ti=%lu, g=%u, p=(%f,%f,%f), laser=%s, laser_power=%f, machine_speed=%f\n",
        di, ti,
        (*dtuple_list)[di].G,
        (*dtuple_list)[di].P.x,(*dtuple_list)[di].P.y,(*dtuple_list)[di].P.z,
        (*dtuple_list)[di].laser ? "on" : "off",
        (*dtuple_list)[di].laser_power,
        (*dtuple_list)[di].machine_speed);

        // printf("%s%s%s%s%s%s%s",
        //         (feat_change & 1) ?  "G changed!\n" : "G did not changed!\n",
        //         (feat_change & 2) ?  "X changed!\n" : "X did not change!\n",
        //         (feat_change & 4) ?  "Y changed!\n" : "Y did not change!\n",
        //         (feat_change & 8) ?  "Z changed!\n" : "Z did not change!\n",
        //         (feat_change & 16) ? "laser changed!\n" : "laser did not change!\n",
        //         (feat_change & 32) ? "laser_power changed!\n" : "laser_power did not change!\n" ,
        //         (feat_change & 64) ? "machine_speed changed!\n" : "machine_speed did not change!\n"
        //       );



        //ensure persistence
        //copy previous value, if value has not been set in read line
        //G
        if (di > 0) {
          if ((feat_change & 1) == 0) {
            //(*dtuple_list)[di].P.x = (*dtuple_list)[di-1].P.x;
            //puts("G did not change!");
          }
          //X
          if ((feat_change & 2) == 0) {
            (*dtuple_list)[di].P.x = (*dtuple_list)[di-1].P.x;
            //puts("X did not change!");
          }
          //Y
          if ((feat_change & 4) == 0) {
            (*dtuple_list)[di].P.y = (*dtuple_list)[di-1].P.y;
            //puts("Y did not change!");
          }
          //Z
          if ((feat_change & 8) == 0) {
            (*dtuple_list)[di].P.z = (*dtuple_list)[di-1].P.z;
            //puts("Z did not change!");
          }
          //laser_on_off
          if ((feat_change & 16) == 0) {
            (*dtuple_list)[di].laser = (*dtuple_list)[di-1].laser;
            //puts("laser_on_off did not change!");
          }
          //laser_power
          if ((feat_change & 32) == 0) {
            (*dtuple_list)[di].laser_power = (*dtuple_list)[di-1].laser_power;

            //TEST
            if(ti > 0) {
            (*track_list)[ti].laser_power = (*track_list)[ti-1].laser_power;}

            //puts("laser_power did not change!");
          }
        }
        //machine_speed
        if ((feat_change & 64) == 0) {
          //(*dtuple_list)[di].P.x = (*dtuple_list)[di-1].P.x;
          puts("machine_speed did not change!");
        }

        // if (feat_change > 127) {
        //   fprintf(stderr,"Error: feat_change overflow! feat_change=%u",feat_change);
        // }

        //set rest of the features of data_tuple


        // X || Y || Z changed
        if ((feat_change & 14) > 0) {
          //create new point / entry in data_tuple csv and list

          //copy var value (hashmap) of machine_speed to dtuple struct obj
          // k2 = strfloat_get(h, "VIT_TIR"); //get index k of VIT_TIR var
          // if (k2 < kh_end(h)) {
          //     machine_speed = kh_val(h, k2);
          //     printf("Setting machine_speed=%f",machine_speed);
          //
          //     (*dtuple_list)[di].machine_speed = kh_val(h, k2);
          // } //TEST
          // else {
          //   printf("VIT_TIR key not in hashmap!");
          // }


          //printf("Writing new point to csv=%s...\n",config->data_tuples_csv);
          //write point to data_tuples csv
          fprintf(dtuple_csv,
          "%lu, %lu, %u, %f,%f,%f, %s, %f, %f\n",
          (di+1), (ti+1),
          (*dtuple_list)[di].G,
          (*dtuple_list)[di].P.x,(*dtuple_list)[di].P.y,(*dtuple_list)[di].P.z,
          (*dtuple_list)[di].laser ? "on" : "off",
          (*dtuple_list)[di].laser_power,
          (*dtuple_list)[di].machine_speed);



          feat_change = 0;

          puts("");
          di++;
        }

      }
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

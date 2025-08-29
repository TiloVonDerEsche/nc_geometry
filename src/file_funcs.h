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

void read_mpf (uint8_t read_all,
  data_tuple** dtuple_list, track** tl, size_t* tl_len,
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

  fprintf(dtuple_csv, "G, p_x,p_y,p_z, laser_on_off, laser_power_w, machine_speed_mm_per_min\n");

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

  uint8_t dim_changed = 0b000; //bool x y z


  const size_t keywords_len = 8;
  const char* keywords[] = {
    "/LASER_ON",
    "/LASER_OFF",
    "PUIS_LASER ",
    "VIT_TIR=",
    "X",
    "Y",
    "Z",
    "G"
  };

  printf("Reading %ld lines from %s...\n",config->mpf_lines,config->mpf_file);

  //////////////////////////////////////////////////////////////////////////////
  //----------------------NC Code Interpreter Reading Loop--------------------//
  //////////////////////////////////////////////////////////////////////////////

  //str_float hashmap for variables
  puts("Initializing str_float_map...");
  strfloat_t *h = strfloat_init();  // Create empty map
  int absent;
  khint_t k; //portable unsigned int iterator, like size_t?
  khint_t k2;

  char* key;
  char* value;

  char* trimmed;
  //-------






  size_t ti = 0; //track id
  size_t di = 0; //data_tuple id (for dtuple_list)
  //size_t li = 0;
  //continue as long as the line contains something
  char line_buf[config->max_line_len];
  char buf[config->max_line_len];
  char read_value[config->max_line_len];
  char c = '\0';
  size_t bi = 0;
  size_t vi = 0;
  uint8_t read_number = 0;
  int keypos = -1;
  //fgets prints the new line in line_buf, and keeps a line index count
  //read mpf linewise
  while ((fgets(line_buf, config->max_line_len, mpf) != NULL) && (di < config->mpf_lines || read_all)) {

      //line_buf and line are currently always equal
      printf("buf=%s",line_buf);



      //use str_float hashmap for variables
      //-------------------//
      //Does line contain '=' (might not be optimal solution)
      if (strchr(line_buf, '=') != NULL) { //does not work if '=' is in a comment
        //assign a variable mode
        trimmed = trim(line_buf); //remove whitespaces before, after & around '='
        parse_line(trimmed, &key, &value);



        //--------
        k = strfloat_put(h, line_buf, &absent);  // Key is a const char* pointer


        if (absent) {
            kh_key(h, k) = strdup(line_buf);}

        if (is_part_of_num(value[0])) {
            kh_val(h, k) = atof(value);}
        else { //if 'value' is another variable
            //lookup value for key on the right of '='
            //key on the left of '=' gets its value instead
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

          if (read_number && is_part_of_num(c)) {
              //check if current buf is matching to a keyword
              //parse value
              read_value[vi] = c;
              vi++;

              //printf("read_value=%sE\n",read_value);
          }
          else {

            //save value
            if (read_value[0] != 0) {
              //printf("read_value=%s\n",read_value);

              if (keypos >= 0) {
                set_key_value(keypos,read_value,
                              di, ti, dtuple_list, tl,
                              &dim_changed);
              }
            }
            if (c != ' ') {
              read_number = 0;
              memset(read_value, 0, config->max_line_len);
              vi = 0;
            }


            //inc buf
            buf[bi] = c;
            bi++;
          }
          //printf("%c\n",c);

          //printf("Checking %s as keyword...",buf);
          keypos = is_in_list(buf, keywords, keywords_len);
          if (keypos != -1) {
            //printf("KEYWORD %d FOUND!",keypos);
            read_number = 1;
          }


          if (c == '\0' || c == ';' || c == ' ' || c == '\n') {
              //buf[0] = '\0';
              memset(buf, 0, config->max_line_len);
              bi = 0;
              if (c == '\0' || c == ';') {break;}
          }

        }
      }

      //See what bits of dim_changed are still zero
      //Give the according dims the value of the last point





      //dim_changed seems to be redundant for some reason
      //(X,Y,Z) seem to be correctly set by set_key_value fn
      if (dim_changed > 0) {
        //puts("dim_changed YES!");
        //dim_changed=0b0%u%u%u%u%u%u%u
        printf("G=%u, p_x=%f, laser=%s, laser_power=%f, machine_speed=%f\n",
        (*dtuple_list)[di].G,
        (*dtuple_list)[di].P.x,
        (*dtuple_list)[di].laser ? "on" : "off",
        (*dtuple_list)[di].laser_power,
        (*dtuple_list)[di].machine_speed);

        printf("%s%s%s%s%s%s%s",
                (dim_changed & 64) ? "machine_speed changed!\n" : "machine_speed did not change!\n",
                (dim_changed & 32) ? "laser_power changed!\n" : "laser_power did not change!\n" ,
                (dim_changed & 16) ? "laser changed!\n" : "laser did not change!\n",
                (dim_changed & 8) ? "G changed!\n" : "G did not changed!\n",
                (dim_changed & 4) ?  "X changed!\n" : "X did not change!\n",
                (dim_changed & 2) ?  "Y changed!\n" : "Y did not change!\n",
                (dim_changed & 1) ?  "Z changed!\n" : "Z did not change!\n");

        if (dim_changed < 7) {
          //check every dim
          // puts("dim_changed < 7 YES!");
          //X
          if ((dim_changed & 4) == 0) {
            //(*dtuple_list)[di].P.x = (*dtuple_list)[di-1].P.x;
            puts("X did not change!");
          }
          //Y
          if ((dim_changed & 2) == 0) {
            puts("Y did not change!");
            //(*dtuple_list)[di].P.y = (*dtuple_list)[di-1].P.y;
          }
          //Z
          if ((dim_changed & 1) == 0) {
            puts("Z did not change!");
          //   (*dtuple_list)[di].P.z = (*dtuple_list)[di-1].P.z;
          }
          //
          //}
        }
        else if (dim_changed > 127) {
          fprintf(stderr,"Error: dim_changed overflow! dim_changed=%u",dim_changed);
        }

        //set rest of the features of data_tuple



        //write point to data_tuples csv
        fprintf(dtuple_csv,
        "%u, %f,%f,%f, %s, %f, %f\n",
        (*dtuple_list)[di].G,
        (*dtuple_list)[di].P.x,(*dtuple_list)[di].P.y,(*dtuple_list)[di].P.z,
        (*dtuple_list)[di].laser ? "on" : "off",
        (*dtuple_list)[di].laser_power,
        (*dtuple_list)[di].machine_speed);

        dim_changed = 0;

        puts("");
        di++;
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

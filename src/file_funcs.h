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

size_t count_lines(FILE* fp) {
  char c;
  size_t lines = 0;
  while(!feof(fp))
  {
    c = fgetc(fp);
    if(c == '\n')
    {
      lines++;
    }
  }

  rewind(fp);

  return lines;
}


void read_mpf (
  data_tuple** dtuple_list, track** track_list, size_t* tl_len,
  Config* config){
  //set config for mpf_lines, mpf_file path

  printf("Opening: %s in read mode...\n",config->mpf_file);
  FILE* mpf = fopen(config->mpf_file, "r");
  if (mpf == NULL) {
      fprintf(stderr, "Error: Could not open %s (in read mode)!\n", config->mpf_file);
      return;
  }

  printf("Opening: hmhis.json in write mode...\n");
  FILE* hmhis = fopen("./data/hmhis.json", "w");
  if (hmhis == NULL) {
      fprintf(stderr, "Error: Could not open hmhis.csv (in write mode)!\n");
      return;
  }
  fprintf(hmhis,"["); //JSON literal object list paranthesis

  //read mpf into program str_arr, each entry is a line of the mpf
  char program[config->mpf_lines][config->max_line_len];
  char line_buf[config->max_line_len];

  size_t mi = 0;
  while ((fgets(line_buf, config->max_line_len, mpf) != NULL) && (mi < config->lines_to_read)) {
    strcpy(program[mi],line_buf);
    printf("Copied %s to program[%lu]\n",line_buf,mi);
    mi++;
  }
  fclose(mpf);

  printf("Reading %ld lines from %s...\n",config->mpf_lines,config->mpf_file);

  //////////////////////////////////////////////////////////////////////////////
  //----------------------NC Code Interpreter Reading Loop--------------------//
  //////////////////////////////////////////////////////////////////////////////

  int absent;

  //str_float hashmap for variables
  puts("Initializing str_float_map...");
  strfloat_t* h = strfloat_init();  // Create hashmap


  //hashmap indices
  khint_t k;
  khint_t kl; //index left token
  khint_t kr; //index right token

  size_t ti = 0; //track id / index
  size_t di = 0; //data_tuple id / index
  size_t li = 0; //line index
  size_t ki = 0; //keyword index

  uint8_t lpower_mode = 0; //bool for puis_laser value fetching

  char keyword_buf[config->max_line_len];
  char value_buf[config->max_line_len];

  char* key;
  char* value;
  char* char_ptr;


  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");//dup so that freeing loop has something to free

  k = strfloat_put(h, "laser_power", &absent);
  kh_key(h, k) = strdup("laser_power");

  //linewise loop
  for (size_t li = 0; li < config->lines_to_read; li++) {
      char_ptr = program[li]; //set to start of line

      printf("program[%lu]=%s",li,program[li]);
      printf("*char_ptr=%c\n",*char_ptr);

      k = strfloat_put(h, "line", &absent);
      kh_val(h, k) = li;

      //print hmhis every line, except for comment
      if (*char_ptr != '\0' && *char_ptr != ';' && *char_ptr != '\n') {
        print_hashmap(h,hmhis);}

      //single command char loop
      while (*char_ptr != '\0' && *char_ptr != ';' && *char_ptr != '\n') {
          printf("*char_ptr=%c\n",*char_ptr);

            ki = 0; //reset keyword_buf index
            //read one token-> either a cmd keyword or varname
            while (isalnum((char)*char_ptr) || *char_ptr == '/' || *char_ptr == '_') {
                keyword_buf[ki++] = *char_ptr++;}
            keyword_buf[ki] = '\0';
            printf("keyword_buf=%s\n",keyword_buf);

            if (ki == 0) { // No keyword found, maybe just a symbol
              char_ptr++; continue;
            }

            //-------------------variable assignment-------------------//
            if (*char_ptr == '=') {
                //add left token (varname) as a hashmap key
                printf("Adding keyword %s to hashmap...\n",keyword_buf);
                kl = strfloat_put(h, keyword_buf, &absent);

                if (absent) {
                  kh_key(h, kl) = strdup(keyword_buf);}
                //----

                char_ptr++; // Move past '='


                //read right token
                size_t i = 0;
                while
                (is_part_of_fnum((char)*char_ptr) ||
                        isalnum((char)*char_ptr) ||
                               *char_ptr == '_') {
                    value_buf[i++] = *char_ptr++;
                }
                value_buf[i] = '\0';

                puts("-----");
                printf("Read %s=%s\n",keyword_buf,value_buf);
                puts("-----");

                //check if value is a literal or another var
                if(is_valid_literal(value_buf)) {
                  kh_val(h, kl) = atof(value_buf);}
                else if(is_valid_varname(value_buf)) {
                  kr = strfloat_get(h, value_buf); //lookup value of right var

                  //give left var value of right var
                  if (kr < kh_end(h)) {
                      kh_val(h, kl) = kh_val(h, kr);}
                }
                else {
                  fprintf(stderr,
                    "Line %lu: "
                    "Token on the right of '=' is neither a valid literal or varname!",
                    (li+1));
                }

            }
            else if (*char_ptr == ':') { //LABEL for goto cmd

            }

            //-----------------------cmd with num or num literal------------------//
            else if (is_part_of_fnum((char)*(char_ptr-1))) { //cmd with num or num
              if (isalpha(keyword_buf[0]) || keyword_buf[0] == '_') { //cmd with num
                *char_ptr--;

                //puts("CMD WITH NUMBER");

                size_t cb_len = 20; //replace 20 w global constant (config.txt)
                char cmd_buf[cb_len] = {};

                size_t fnb_len = 20; //replace 20 w global constant (config.txt)
                char fnum_buf[fnb_len] = {};
                //key & value, seperated through letters to digits
                parse_cmd_w_fnum(&char_ptr, ki, &cb_len, &cmd_buf, &fnb_len, &fnum_buf);

                puts("\n------");
                printf("Read cmd=%s, with len=%lu\n",cmd_buf,cb_len);
                printf("Read num after cmd=%s, with len=%lu\n",fnum_buf,fnb_len);
                puts("------\n");


                //printf("Adding keyword %s to hashmap...\n",cmd_buf);
                kl = strfloat_put(h, cmd_buf, &absent);

                if (absent) {
                  kh_key(h, kl) = strdup(cmd_buf);
                }

                kh_val(h, kl) = atof(fnum_buf);
              }
              else if (is_part_of_fnum(keyword_buf[0])) {
                puts("Read a pure float num!");
                if (lpower_mode) {
                  k = strfloat_put(h, "laser_power", &absent);
                  kh_val(h, k) = atof(keyword_buf);

                  lpower_mode = 0;
                }
              }

            }
            //-----------------special '/' cmds----------------------//
            else if ((keyword_buf[0]) == '/') { //special '/' cmds

                printf("Read special cmd: keyword_buf=%s\n",keyword_buf);

                if(strcmp(&keyword_buf[1],"LASER_ON") == 0) {
                  //puts("Read LASER_ON cmd!");
                  k = strfloat_put(h, "laser", &absent);
                  kh_val(h, k) = 1;
                }
                else if (strcmp(&keyword_buf[1],"LASER_OFF") == 0) {
                  //puts("Read LASER_OFF cmd!");
                  k = strfloat_put(h, "laser", &absent);
                  kh_val(h, k) = 0;
                }
            }
            //----------------cmds where value is seperated with ' '----------//
            else if ((strcmp(keyword_buf,"PUIS_LASER") == 0)) {
              lpower_mode = 1; //read value of laser_power in next iter
              puts("PUIS_LASER read!");
            }
            //---------------------unknown cmd (format)---------------------//
            else {
              puts("\n--------------");
              fprintf(stderr,"Read Keyword:'%s' does not start with '/',\n"
                             "Is neither a cmd followed by a num,\n"
                             "Nor is a variable assignment!\n",keyword_buf);
              puts("--------------\n");
            }
      }
  }

  //save the length of tl (track_list) arr in tl_len
  (*tl_len) = ti;

  print_hashmap(h,hmhis);

  fseek(hmhis, -3, SEEK_CUR); //overwrite last redundant comma
  fprintf(hmhis,"]"); //JSON literal object list paranthesis

  fclose(hmhis);

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

        if (strcmp(key, "lines_to_read") == 0) {
            config->lines_to_read = (size_t)atoi(value);
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

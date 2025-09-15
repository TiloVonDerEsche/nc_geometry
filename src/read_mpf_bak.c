// fgets prints the new line in line_buf, and keeps a line index count
// continue as long as the line contains something
// read mpf linewise
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
      if (is_part_of_num(value[0])) { //literal
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



void init_track_list(float hradius, float vradius, size_t track_list_len, track** tl) {
  for (size_t i = 0; i < track_list_len; i++) {
    (*tl)[i].hradius = hradius;
    (*tl)[i].vradius = vradius;
  }
}

void write_tracks_to_csv(size_t track_list_len, track** tl) {
  FILE* file = fopen(".\\data\\track_list.csv", "w");
  if (file == NULL) {
      perror("An error occured, while trying to create / open and write to track_list.csv file!");
      return;
  }

  fprintf(file,"Track_id, A.x, A.y, A.z, B.x, B.y, B.z, hradius, vradius, coll_vec.x, coll_vec.y, coll_vec.z\n");

  for (size_t i = 0; i < track_list_len; i++) {
    fprintf(file,"%lld, %f,%f,%f, %f,%f,%f, %f,%f, %f,%f,%f\n",
           i,
           (*tl)[i].A.x, (*tl)[i].A.y, (*tl)[i].A.z,
           (*tl)[i].B.x, (*tl)[i].B.y, (*tl)[i].B.z,
           (*tl)[i].hradius,(*tl)[i].vradius,
           (*tl)[i].coll_vec.x,(*tl)[i].coll_vec.y,(*tl)[i].coll_vec.z);
  }
}

float read_mpf_and_create_point_cloud(char filePath[], size_t mpf_lines, size_t max_line_len, data_tuple** cords, track** tl, size_t* tl_len) {
  char line[max_line_len]; //line buffer, to read a line with max 1000 chars

  // open mpf file for reading
  FILE* file = fopen(filePath, "r");
  if (file == NULL) {
      perror("An error occured, while trying to open a file");
      return 1;
  }

  //open csv file for writing the data_tuples there
  FILE* csv_file = fopen(".\\data\\data_tuples.csv", "w"); // or "a" to append
  if (csv_file == NULL){
      perror("Error while creating csv data_tuples file!");
      return 1;
  }

  uint8_t laser_on_off = 0;
  float machine_speed = 0.0;


  char format[] = " %*[^-0123456789]%lf";

  double number = 0;

  (*cords)[0].P.x = 0;
  (*cords)[0].P.y = 0;
  (*cords)[0].P.z = 0;

  vec3D new_P = {0,0,0};

  uint8_t x_has_changed = 0;
  uint8_t y_has_changed = 0;
  uint8_t z_has_changed = 0;


  printf("Reading file with path: %s\n",filePath);


  //////////////////////////////////////////////////////////////////////////////
  //----------------------NC Code Interpreter Reading Loop--------------------//
  //////////////////////////////////////////////////////////////////////////////
  uint8_t fetch_next_point = 0;
  size_t ti = 0; //track id
  int i = 0;
  //continue as long as the line contains something
  while ((fgets(line, max_line_len, file) != NULL) && i < mpf_lines) {
      //print_buf(line,max_line_len);

      x_has_changed = 0;
      y_has_changed = 0;
      z_has_changed = 0;

      //read the on/off state of the laser
      if (strstr(line, "G00") != NULL) {laser_on_off = 0;}
      else if (strstr(line, "G01") != NULL) {laser_on_off = 1;}

      if(strstr(line, "LASER_ON") != NULL) {
        //get next point
        fetch_next_point = 1;
      }
      else if(strstr(line, "LASER_OFF") != NULL) {
        //get current point
        if (i > 1) {
          (*tl)[ti].B.x = (*cords)[i-1].P.x;
          (*tl)[ti].B.y = (*cords)[i-1].P.y;
          (*tl)[ti].B.z = (*cords)[i-1].P.z;
        }

        //increment cylinder index
        ti++;
      }

      //read the set machine speed
      if (strstr(line, "VIT_TIR=") != NULL) {
          //sscanf returns 1 when it successfully parses one item
          //based on the format specifier provided.
          if (sscanf(strstr(line, "VIT_TIR="), format, &number)) {
              machine_speed = number;
          }
      }

      //ignore comments
      if (strstr(line, ";") != NULL){continue;}

      if (strstr(line, " X") != NULL){
          if (sscanf(strstr(line, " X"), format, &number)){
              new_P.x = number;
              x_has_changed = 1;
          }
      }

      if (strstr(line, " Y") != NULL){
          if (sscanf(strstr(line, " Y"), format, &number)){
              new_P.y = number;
              y_has_changed = 1;
          }
      }

      if (strstr(line, " Z") != NULL) {
          if (sscanf(strstr(line, " Z"), format, &number)){
              new_P.z = number;
              z_has_changed = 1;
          }
      }

      if (x_has_changed == 1 || y_has_changed == 1 || z_has_changed == 1){

          //(X10.0 in the NC means take the old point and change X to 10.0)
          copy_data_tuple(i,cords);

          //then only change the parameters that have changed
          if (x_has_changed) (*cords)[i].P.x = new_P.x;
          if (y_has_changed) (*cords)[i].P.y = new_P.y;
          if (z_has_changed) (*cords)[i].P.z = new_P.z;

          (*cords)[i].laser = laser_on_off;

          if (fetch_next_point) { //get start of track
            (*tl)[ti].A.x = (*cords)[i].P.x;
            (*tl)[ti].A.y = (*cords)[i].P.y;
            (*tl)[ti].A.z = (*cords)[i].P.z;

            fetch_next_point = 0;
          }


          //printf("(%f, %f, %f), laser=%d\n", (*cords)[i].P.x, (*cords)[i].P.y, (*cords)[i].P.z, (*cords)[i].laser);
          fprintf(csv_file, "(%f, %f, %f), laser=%d\n", (*cords)[i].P.x, (*cords)[i].P.y, (*cords)[i].P.z, (*cords)[i].laser);

          i += 1;
      }
  }

  //save the length of tl (track_list) arr in tl_len
  (*tl_len) = ti;

  //close the mpf file
  fclose(file);
  fclose(csv_file);
  return machine_speed;
}

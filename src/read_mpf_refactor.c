
uint8_t read_line(char line[], size_t max_line_len, data_tuple** tuple_list, track** tl) {
  char format[] = " %*[^-0123456789]%lf";

  double read_val = 0;

  //init first point, because the successor needs its predeceding point to copy
  // vec3D new_P = {0,0,0};

  // uint8_t x_has_changed = 0;
  // uint8_t y_has_changed = 0;
  // uint8_t z_has_changed = 0;

  //use last 3 bits as bool which dimension changed (X,Y,Z)
  uint8_t dim_changed = 0b00000000; //0b00000111

  const char* keywords[] = {
    "LASER_ON",
    "LASER_OFF",
    "PUIS_LASER ",
    "VIT_TIR=",
    " X",
    " Y",
    " Z"
  };

  //read words char wise
  //check which word is matching the current sequence
  //if keyword sequence completed, listen for the number after the keyword

  //puts("Printing characters of lines in mpf...");
  for (size_t li; li < max_line_len; li++){
    printf("%c\n",line[li]);
  }
    // if(strstr(line, "LASER_ON") != NULL) {
    //   laser_on_off = 1;
    //   //get point where head is currently (Point before LASER_ON)
    //   (*tl)[ti].A.x = (*tuple_list)[i-1].P.x;
    //   (*tl)[ti].A.y = (*tuple_list)[i-1].P.y;
    //   (*tl)[ti].A.z = (*tuple_list)[i-1].P.z;
    //
    //   (*tl)[ti].laser_power = puis_laser;
    //   (*tl)[ti].machine_speed = vit_tir;
    // }
    // //should we set laser_power & machine_speed at the start or end of track?
    // //they should not change during a track
    // else if(strstr(line, "LASER_OFF") != NULL) {
    //   laser_on_off = 0;
    //   //get current point
    //   if (i > 1) {
    //     //i-1 because there is no point at i
    //     (*tl)[ti].B.x = (*tuple_list)[i-1].P.x;
    //     (*tl)[ti].B.y = (*tuple_list)[i-1].P.y;
    //     (*tl)[ti].B.z = (*tuple_list)[i-1].P.z;
    //   }
    //
    //   //increment track index
    //   ti++;
    // }
    //
    // //read laser power
    // if (strstr(line, "PUIS_LASER ") != NULL) {
    //     if (sscanf(strstr(line, "PUIS_LASER "), format, &read_val)) {
    //         puis_laser = read_val;
    //     }
    // }
    //
    // //read the set machine speed
    // if (strstr(line, "VIT_TIR=") != NULL) {
    //     //sscanf returns 1 when it successfully parses one item
    //     //based on the format specifier provided.
    //     if (sscanf(strstr(line, "VIT_TIR="), format, &read_val)) {
    //         vit_tir = read_val;
    //     }
    // }
    //
    //
    // if (strstr(line, " X") != NULL){
    //     if (sscanf(strstr(line, " X"), format, &read_val)){
    //         new_P.x = read_val;
    //         x_has_changed = 1;
    //     }
    // }
    //
    // if (strstr(line, " Y") != NULL){
    //     if (sscanf(strstr(line, " Y"), format, &read_val)){
    //         new_P.y = read_val;
    //         y_has_changed = 1;
    //     }
    // }
    //
    // if (strstr(line, " Z") != NULL) {
    //     if (sscanf(strstr(line, " Z"), format, &read_val)){
    //         new_P.z = read_val;
    //         z_has_changed = 1;
    //     }
    // }
    //
    //
    // //ignore comments
    // if (strstr(line, ";") != NULL){continue;}

    return dim_changed;
}

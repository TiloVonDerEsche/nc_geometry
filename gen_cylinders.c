#include "helper_funcs.h"



void write_cyls_to_csv(float radius, size_t cyls_len, cylinder** cyls) {
  FILE* file = fopen("cylinders.csv", "w");
  if (file == NULL) {
      perror("An error occured, while trying to open a file");
      return;
  }

  fprintf(file,"Cylinder_id,A,B,radius\n");

  for (size_t i = 0; i < cyls_len; i++) {
    fprintf(file,"%d,(%f,%f,%f), (%f,%f,%f), %f\n",
           i,
           (*cyls)[i].A.x, (*cyls)[i].A.y, (*cyls)[i].A.z,
           (*cyls)[i].B.x, (*cyls)[i].B.y, (*cyls)[i].B.z,
           radius);
  }
}

float read_mpf_and_create_point_cloud(char filePath[], size_t mpf_lines, size_t max_line_len, data_tuple** cords, cylinder** cyls, size_t* cyls_len) {
  char line[max_line_len]; //line buffer, to read a line with max 1000 chars

  // open mpf file for reading
  FILE* file = fopen(filePath, "r");
  if (file == NULL) {
      perror("An error occured, while trying to open a file");
      return 1;
  }

  //open csv file for writing the data_tuples there
  FILE* csv_file = fopen("data_tuples.csv", "w"); // or "a" to append
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
  size_t cyl_i = 0;
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
          (*cyls)[cyl_i].B.x = (*cords)[i-1].P.x;
          (*cyls)[cyl_i].B.y = (*cords)[i-1].P.y;
          (*cyls)[cyl_i].B.z = (*cords)[i-1].P.z;
        }

        //increment cylinder index
        cyl_i++;
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
            (*cyls)[cyl_i].A.x = (*cords)[i].P.x;
            (*cyls)[cyl_i].A.y = (*cords)[i].P.y;
            (*cyls)[cyl_i].A.z = (*cords)[i].P.z;

            fetch_next_point = 0;
          }


          //printf("(%f, %f, %f), laser=%d\n", (*cords)[i].P.x, (*cords)[i].P.y, (*cords)[i].P.z, (*cords)[i].laser);
          fprintf(csv_file, "(%f, %f, %f), laser=%d\n", (*cords)[i].P.x, (*cords)[i].P.y, (*cords)[i].P.z, (*cords)[i].laser);

          i += 1;
      }
  }

  //save the length of cyls arr in cyls_len
  (*cyls_len) = cyl_i;

  //close the mpf file
  fclose(file);
  fclose(csv_file);
  return machine_speed;
}


// int main() {
//     size_t mpf_lines = 2500;
//     size_t precision = 10; //1000 yields ridiculous amount of csv lines
//     size_t max_line_len = 1000;
//     char* file_name = "ElGeo_5_V2_1.mpf";
//
//     data_tuple* cords = calloc(mpf_lines,sizeof(data_tuple));
//     if (cords == NULL) {
//       fprintf(stderr, "Memory allocation of cords failed\n");
//         exit(EXIT_FAILURE);
//     }
//
//     cylinder* cyls = calloc(mpf_lines,sizeof(cylinder));
//     if (cyls == NULL) {
//       fprintf(stderr, "Memory allocation of cylinder list failed\n");
//         exit(EXIT_FAILURE);
//     }
//
//     //size_t* cyls_len = calloc(1,sizeof(size_t));
//     size_t cyls_len = -1;
//     float machine_speed = read_mpf_and_create_point_cloud(file_name, mpf_lines, max_line_len, &cords, &cyls, &cyls_len);
//
//     printf("%d",cyls_len);
//
//     write_cyls_to_csv(0.3,cyls_len,&cyls);
//
//
//
//
//     free(cords);
//     return 0;
// }

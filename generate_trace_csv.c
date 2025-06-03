#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct {
float x;
float y;
float z;
} vec3D;

typedef struct {
  vec3D A;
  vec3D B;
  float radius;
} cylinder;

typedef struct {
  vec3D P;
  uint8_t laser; //boolean -> laser on or off?
} data_tuple;


void copy_data_tuple(size_t i, data_tuple** arr) {
    if (i <= 0) { return; } // Avoid seg fault when i = 0
    (*arr)[i].P.x = (*arr)[i-1].P.x;
    (*arr)[i].P.y = (*arr)[i-1].P.y;
    (*arr)[i].P.z = (*arr)[i-1].P.z;
    (*arr)[i].laser = (*arr)[i-1].laser;
}


void print_buf(char* buf, size_t buf_len) {
  for (size_t i = 0; i < buf_len && buf[i] != '\0'; i++) {
    printf("%c",buf[i]);
  }
}

float dot_product(vec3D s, vec3D t) {
  return (s.x * t.x) + (s.y * t.y) + (s.z * t.z);
}

float vec_len(vec3D v) {
  return sqrt(dot_product(v,v));
}

float angle_btw_vecs(vec3D v1, vec3D v2) {
  return acos(dot_product(v1,v2) / (vec_len(v1) * vec_len(v2)))* (180 / M_PI);
}

float read_mpf_and_create_point_cloud(char filePath[], size_t mpf_lines, size_t max_line_len, data_tuple** cords) {
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


          //printf("(%f, %f, %f), laser=%d\n", (*cords)[i].P.x, (*cords)[i].P.y, (*cords)[i].P.z, (*cords)[i].laser);
          fprintf(csv_file, "(%f, %f, %f), laser=%d\n", (*cords)[i].P.x, (*cords)[i].P.y, (*cords)[i].P.z, (*cords)[i].laser);

          i += 1;
      }
  }

  //close the mpf file
  fclose(file);
  fclose(csv_file);
  return machine_speed;
}


int simulate_machine(float machine_speed, size_t precision, size_t mpf_lines, data_tuple** cords) {

  ///////////////////////////////////////////////////////
  //----------Get the length of cords array------------//
  ///////////////////////////////////////////////////////
  puts("Counting amount of tuples in cords array...");

  //we iterate through the the cords array
  //and break when we read at least 5 entries which contain (0,0,0,0)
  size_t zero_counter = 0;
  size_t i;
  for(i = 0; i < mpf_lines; i++)
  {

      if ((*cords)[i].P.x == 0 && (*cords)[i].P.y == 0 && (*cords)[i].P.z == 0 && (*cords)[i].laser == 0)
      {
          zero_counter++;
      }
      else
      {
          zero_counter = 0;
      }

      if (zero_counter > 4)
      {
          break;
      }

      // printf("P%d(%f, %f, %f)\n",j, cords[j * 4], cords[j * 4 + 1], cords[j * 4 + 2]);
  }
  size_t cords_length = i - 4;
  ///////////////////////////////////////////////////////

  //printf("%d",cords_length);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //----------Simulate the machine and note the position at every timestep of 0.002 seconds and write to the csv------------//
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  puts("Simulating the machine...");

  vec3D A = {0,0,0}; //previous point
  vec3D B = {0,0,0}; //current point
  vec3D C = {0,0,0}; //next point

  vec3D AB = {0,0,0}; //vec A to B
  vec3D BC = {0,0,0}; //vec B to C

  float AB_len = 1;
  float BC_len = 1;

  float dot_product = 0;
  float angle_AB_BC = 0;

  float time = 0;
  float temp_time = 0;

  //determine when the machine slows down
  float angle_threshold = 75.0;

  //~boolean variable, that is 1 when the angle_last_curr exceeds the angle_threshold and otherwise 0
  uint8_t critical_area = 0;


  //Prepare the output csv
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  FILE* csvFile = fopen("trace_output.csv", "w"); // or "a" to append

  if (csvFile == NULL)
  {
      perror("Error opening file");
      return 1;
  }

  // Write the header row of the csv
  fprintf(csvFile, "Time in seconds,"
                    "Laser On/Off,"
                    "angle_AB_BC,"
                    "A,"
                    "B,"
                    "C,"
                    "AB,"
                    "AB_len,"
                    "BC,"
                    "BC_len,"
                    "dotp,"
                    "(AB_len * BC_len),"
                    "\n");
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  for(int i = 0; i < cords_length - 1; i ++)
  {
    //set A,B,C to the next triple of points in cords
     if (i > 0) {
          A.x = (*cords)[(i-1)].P.x;
          A.y = (*cords)[(i-1)].P.y;
          A.z = (*cords)[(i-1)].P.z;
     }

      B.x = (*cords)[i].P.x;
      B.y = (*cords)[i].P.y;
      B.z = (*cords)[i].P.z;

      C.x = (*cords)[i+1].P.x;
      C.y = (*cords)[i+1].P.y;
      C.z = (*cords)[i+1].P.z;


      //redefine vectors AB, BC
      AB.x = B.x - A.x;
      AB.y = B.y - A.y;
      AB.z = B.z - A.z;

      BC.x = C.x - B.x;
      BC.y = C.y - B.y;
      BC.z = C.z - B.z;


      angle_AB_BC = angle_btw_vecs(AB,BC);

      if (angle_AB_BC > angle_threshold) {critical_area = 1;}
        else {critical_area = 0;}

      uint8_t laser_on_off = (*cords)[i].laser;


      //printf("Going from (%f,%f,%f) to (%f,%f,%f)\n",B.x,B.y,B.z,C.x,C.y,C.z);
      //While Point C isn't reached go towards it.
      while((round(B.x) != round(C.x)) ||
            (round(B.y) != round(C.y)) ||
            (round(B.z) != round(C.z)))
      {
          //printf("B(%f,%f,%f), C(%f,%f,%f)\n",round(B.x),round(B.y),round(B.z), round(C.x),round(C.y),round(C.z));

          //Go 1/precision step on the vector BC to C
          //should I change precision proportionally depending on the
          //ratio of the component compared to the vec length?
          if(round(B.x) != round(C.x)) {B.x += BC.x / precision;}
          if(round(B.y) != round(C.y)) {B.y += BC.y / precision;}
          if(round(B.z) != round(C.z)) {B.z += BC.z / precision;}

          //time counts the overall passed time.
          time += (BC_len / machine_speed) * 60; //in seconds (t = s/v)

          //temp_time counts to 0.002 and then gets reset to 0, so that an entry is being made every 0.002 seconds.
          temp_time += (BC_len / machine_speed) * 60;

          if (temp_time > 0.002)
          {
              // printf("%f",temp_time);
              //write a row to the csv
              fprintf(csvFile, "%f," //time
                                "laser?=%d,"//laser

                                "A(%f,%f,%f)," //A
                                "B(%f,%f,%f),"
                                "C(%f,%f,%f),"

                                "AB(%f,%f,%f),"
                                "%f,"

                                "BC(%f,%f,%f),"
                                "%f,"

                                // "dotP= %f,"
                                // "lenP= %f,"
                                "angle= %f,"
                                "critical?=%d\n",

                                time,
                                laser_on_off,
                                A.x,A.y,A.z,
                                B.x,B.y,B.z,
                                C.x,C.y,C.z,
                                AB.x,AB.y,AB.z,
                                AB_len,
                                BC.x,BC.y,BC.z,
                                BC_len,
                                // dot_product,
                                // (length_of_curr_vector*length_of_last_vector),
                                angle_AB_BC,
                                critical_area);

              //reset temp_time so it can count to 0.002 again
              temp_time = 0;
          }
      }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  fclose(csvFile);
}

int main() {
    size_t mpf_lines = 250000;
    size_t precision = 10; //1000 yields ridiculous amount of csv lines
    size_t max_line_len = 1000;
    char* file_name = "ElGeo_5_V2_1.mpf";

    data_tuple* cords = calloc(mpf_lines,sizeof(data_tuple));
    if (cords == NULL) {
      fprintf(stderr, "Memory allocation of cords failed\n");
        exit(EXIT_FAILURE);
    }

    cylinder* cyls = calloc(mpf_lines,sizeof(cylinder));
    if (cyls == NULL) {
      fprintf(stderr, "Memory allocation of cylinder list failed\n");
        exit(EXIT_FAILURE);
    }

    float machine_speed = read_mpf_and_create_point_cloud(file_name, mpf_lines, max_line_len, &cords);
    simulate_machine(machine_speed,precision,mpf_lines,&cords);




    free(cords);
    return 0;
}

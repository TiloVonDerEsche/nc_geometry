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

float dot_product(vec3D A, vec3D B) {
  return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}

float vec_len(vec3D A) {
  return sqrt(dot_product(A,A));
}

int read_mpf_and_create_point_cloud(char filePath[], size_t mpf_lines, size_t max_line_len, data_tuple** cords) {
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
  return 0;
}


int simulate_machine(size_t mpf_lines, data_tuple** cords) {

  ///////////////////////////////////////////////////////
  //----------Get the length of cords array------------//
  ///////////////////////////////////////////////////////
  //we iterate through the the cords array
  //and break when we read at least 5 entries which contain (0,0,0,0)
  unsigned int j;
  for(j = 0; j < mpf_lines; j++)
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
  cords_length = j - 4;
  ///////////////////////////////////////////////////////

  //printf("%d",cords_length);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //----------Simulate the machine and note the position at every timestep of 0.002 seconds and write to the csv------------//
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Point -1
  // double xn1 = 0;
  // double yn1 = 0;
  // double zn1 = 0;
  vec3D A = {0,0,0};

  //Point 0
  // double x0 = 0;
  // double y0 = 0;
  // double z0 = 0;
  vec3D B = {0,0,0};

  //Point 1
  // double x1 = 0;
  // double y1 = 0;
  // double z1 = 0;
  vec3D C = {0,0,0};


  //last vector (Vector from P-1 to P0)
  // double lvx = 0;
  // double lvy = 0;
  // double lvz = 0;
  vec3D AB = {0,0,0};

  //current vector (Vector from P0 to P1)
  // double vx = 0;
  // double vy = 0;
  // double vz = 0;
  vec3D BC = {0,0,0};

  double AB_len = 1;

  double BC_len = 1;

  //dot_product between last and current vector is used to determine their respective angle
  double dot_product = 0;

  double angle_AB_BC = 0;


  double time = 0;
  double temp_time = 0;

  //for some reason does the precision dictate the number of lines in the csv
  double precision = 100000;

  //determine when the machine slows down
  double angle_threshold = 75.0;

  //~boolean variable, that is 1 when the angle_last_curr exceeds the angle_threshold and otherwise 0
  unsigned int critical_area = 0;


  //Prepare the output csv
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  FILE* csvFile = fopen("trace_output.csv", "w"); // or "a" to append

  if (csvFile == NULL)
  {
      perror("Error opening file");
      return 1;
  }

  // Write the header row of the csv
  fprintf(csvFile, "Time in seconds, Laser On/Off, Angle Between Last and Current Vector, P-1, P0, P1, last vector(P-1 to P0), length last vector, current vector(P0 to P1), length current vector, dot_product, (length_of_last * length_of_curr), angle between last and current vector \n");
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  for(int i = 0; i < cords_length - 1; i ++)
  {
     if (i > 0)
     {
          //point-1
          A.x = cords[(i-1)];
          A.y = cords[(i-1) * 4 + 1];
          A.z = cords[(i-1) * 4 + 2];
     } //xn1, yn1, zn1 = 0; for i = 0


      //point0
      B.x = cords[i];
      B.y = cords[i];
      B.z = cords[i];

      //point1
      C.x = cords[(i+1)];
      C.y = cords[(i+1)];
      C.z = cords[(i+1)];


      //last vector from point-1 to point0
      AB = B.x - A.x;
      AB = B.y - A.y;
      AB = B.z - A.z;

      //current vector from point0 to point1
      BC.x = C.x - B.x;
      BC.y = C.y - B.y;
      BC.z = C.z - B.z;

      AB_len = vec_len(AB);
      BC_len = vec_len(BC);

      //last_vector * current_vector
      dot_product = dot_product(AB,BC);

      //the angle between the last and current vector
      angle_AB_BC = acos(dot_product / (AB_len * BC_len))* (180 / M_PI); //convert the radiant result to degrees

      if (angle_AB_BC > angle_threshold)
      {
          critical_area = 1;
      }
      else
      {
          critical_area = 0;
      }

      laser_on_off = (*cords)[i].laser;


      // printf("%d.: lv(%f, %f, %f), v(%f, %f, %f)\n",i, lvx, lvy, lvz, vx, vy, vz);

      //While point_1 isn't reached go towards it.
      //TODO round the cords

      while((round(x0 * precision) / precision) != (round(x1 * precision) / precision) || (round(y0 * precision) / precision) != (round(y1 * precision) / precision) || (round(z0 * precision) / precision) != (round(z1 * precision) / precision))
      {



          //Go 1/1000 step on the vector to point1
          //TODO uncomment "/ precision"
          x0 += vx; //precision;
          y0 += vy; //precision;
          z0 += vz; //precision;

          // printf("vector(%f,%f,%f))\n",vx/1000,vy/1000,vz/1000);
          // printf("p0(%f,%f,%f),\np1(%f,%f,%f)\n",x0,y0,z0, x1,y1,z1);


          //time counts the overall passed time.
          time += length_of_curr_vector / machine_speed * 60; //in seconds (t = s/v)

          //temp_time counts to 0.002 and then gets reset to 0, so that an entry is being made every 0.002 seconds.
          temp_time += length_of_curr_vector / machine_speed * 60;

          if (temp_time > 0.002)
          {
              // printf("%f",temp_time);
              //write a row to the csv
              fprintf(csvFile, "time= %f, %d, P-1(%f,%f,%f), P0(%f,%f,%f), P1(%f,%f,%f), lv(%f,%f,%f), %f, v(%f,%f,%f), %f, dotP= %f, lenP= %f, angle= %f, critical?=%d\n",time,laser_on_off,xn1,yn1,zn1,x0,y0,z0,x1,y1,z1,lvx,lvy,lvz,length_of_last_vector,vx,vy,vz,length_of_curr_vector, dot_product, (length_of_curr_vector*length_of_last_vector),angle_last_curr,critical_area);

              //reset temp_time so it can count to 0.002 again
              temp_time = 0;
          }


          //printf("%s\n",(round(x0 * precision) / precision) != (round(x1 * precision) / precision) && (round(y0 * precision) / precision) != (round(y1 * precision) / precision) && (round(z0 * precision) / precision) != (round(z1 * precision) / precision) ? "true" : "false");
      }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  fclose(csvFile);
}

int main() {
    size_t mpf_lines = 2500;
    size_t max_line_len = 1000;
    char* file_name = "ElGeo_5_V2_1.mpf";

    data_tuple* cords = calloc(mpf_lines,sizeof(data_tuple));
    if (cords == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    read_mpf_and_create_point_cloud(file_name, mpf_lines, max_line_len, &cords);




    free(cords);


    return 0;
}

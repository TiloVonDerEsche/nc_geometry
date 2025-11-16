#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

#include "khashl_helper.h"


#define CMD_BUF_LEN 20
#define FNUM_BUF_LEN 20

typedef struct {
  size_t lines_to_read;
  size_t mpf_lines;
  size_t max_line_len;

  char mpf_file[256];
  char data_tuples_csv[256];
  char track_list_csv[256];
  char intpol_csv[256];

  float horizontal_radius;
  float vertical_radius;

  float step_dis;
} Config;

typedef struct {
  float x;
  float y;
  float z;
} vec3D;

typedef struct {
  vec3D normal;
  vec3D P;
} plane;

typedef struct {
  //set by interpolation.h
  vec3D P; //intpol point

  //copied from track
  float laser_power;
  float machine_speed;
  vec3D coll_vec;
  float hradius;
  float vradius;


  //set in interpolation fn
  vec3D dir_vec;

  //direction vec of track in polar coordinate form
  //direction angles
  float dir1;
  float dir2;

} intpol_tuple;


typedef struct {
  //set by read_mpf
  vec3D SP; //start point
  vec3D EP; //end point
  float laser_power;
  float machine_speed;
  //set by track_collision
  vec3D coll_vec;
  //set by set_track_radius
  float hradius;
  float vradius;
} track;

typedef struct {
  //set by read_mpf
  uint8_t G;
  vec3D P;
  uint8_t laser; //bool -> laser on/off
  float laser_power;
  float machine_speed;
} data_tuple;

//----------------------struct mutation fns---------------------------------//

void set_track_radius(float hradius, float vradius,
  size_t track_list_len, track** tl) {

  for (size_t i = 0; i < track_list_len; i++) {
    (*tl)[i].hradius = hradius;
    (*tl)[i].vradius = vradius;
  }
}

//---------vector math functions-----------//

float dot_product(vec3D s, vec3D t) {
  return (s.x * t.x) + (s.y * t.y) + (s.z * t.z);
}

vec3D cross_product(vec3D a, vec3D b) {
  return (vec3D) {
    a.y*b.z - a.z*b.y,
    a.z*b.x - a.x*b.z,
    a.x*b.y - a.y*b.x
  };
}

float vec_len(vec3D v) {
  return sqrt(dot_product(v,v));
}

float angle_btw_vecs(vec3D v1, vec3D v2) {
  return acos(dot_product(v1,v2) / (vec_len(v1) * vec_len(v2)))* (180 / M_PI);
}

vec3D connecting_vec(vec3D v1, vec3D v2) {
  return (vec3D) {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
}

vec3D vec_minus(vec3D v1, vec3D v2) {
  return (vec3D) {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3D vec_add(vec3D v1, vec3D v2) {
  return (vec3D) {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3D vec_add_scaled(vec3D s, float scalar, vec3D t) {
  return (vec3D) {s.x + scalar * t.x, s.y + scalar * t.y, s.z + scalar * t.z};
}

vec3D vec_scale(vec3D v, float s) {
  return (vec3D) {v.x * s, v.y * s, v.z * s};
}

vec3D norm_vec(vec3D v) {
  float v_len = vec_len(v);
  if (v_len == 0) {printf("Error: Trying to norm zero vec!\n");
    return (vec3D) {0,0,0};}
  return (vec3D) {v.x / v_len, v.y / v_len, v.z / v_len};
}

//https://studyflix.de/mathematik/abstand-punkt-gerade-2006
float distance_point_to_line(vec3D P, vec3D SP,vec3D dir_vec) {
  return vec_len(cross_product(vec_minus(P,SP), dir_vec)) / vec_len(dir_vec);
}

// Function to compute distance and foot point
// P: A point on Line 1
// v: Direction vector of Line 1
// Q: A point representing the second parallel line
// FQ: Connecting vec btw foot_point and Q, gets returned
vec3D lotfuss(vec3D P, vec3D v, vec3D Q, vec3D *foot_point, double *distance) {
  // Compute t = [(Q - P) . v] / (v . v)
  vec3D PQ = vec_minus(Q, P);
  double v_dot_v = dot_product(v, v);

  if (v_dot_v == 0) {
      //printf("Error: Direction vector cannot be zero vec.\n");
      *distance = 0;
      foot_point->x = foot_point->y = foot_point->z = 0;
      return (vec3D) {0,0,0};
  }

  double t = dot_product(PQ, v) / v_dot_v;

  // Foot point F = P + t * v
  *foot_point = vec_add_scaled(P, t, v);

  // Distance = ||Q - F||
  vec3D FQ = vec_minus(Q, *foot_point);
  *distance = vec_len(FQ);

  return FQ;
}

//-----------------struct list fns---------------//
void copy_data_tuple(size_t i, data_tuple** arr) {
  if (i <= 0) { return; } // Avoid seg fault when i = 0
  (*arr)[i].P.x = (*arr)[i-1].P.x;
  (*arr)[i].P.y = (*arr)[i-1].P.y;
  (*arr)[i].P.z = (*arr)[i-1].P.z;
  (*arr)[i].laser = (*arr)[i-1].laser;
}

//---------------------printing fns---------------------//
void print_buf(char* buf, size_t buf_len) {
  for (size_t i = 0; i < buf_len && buf[i] != '\0'; i++) {
    printf("%c",buf[i]);
  }
}

//---------------string boolean checking functions------------------//

uint8_t is_part_of_fnum(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }

    // Check for specific characters: '-', '.', '/', 'e', '^'
    if (c == '+' || c == '-' || c == '.' || c == 'e' || c == '^') {
        return 1;
    }

    return 0;
}


uint8_t is_valid_varname(const char* name) {
    uint8_t onechar = 0; //at least one char bool
    char c;
    for (int i = 0; name[i] != '\0'; i++) {
        c = name[i];
        if(!onechar && isalpha(c)) {onechar=1;}
        if ( !(isalnum(c) || c == '_') || c == ' ' || c == '\0' || c == '\n') {
            //invalid variable name
            return 0;
        }
    }
    //at least one char in varname
    if (onechar) {return 1;}
    return 0; //no chars in varname
}

uint8_t is_valid_literal(const char* lit) {
  char c;
  for (int i = 0; lit[i] != '\0'; i++) {
      c = lit[i];
      if ( !(is_part_of_fnum(c)) ) {
          //invalid variable name
          return 0;
      }
  }
  // All characters passed the check
  return 1;
}


//------------------------------misc----------------------------------//
int is_in_list(const char* str, const char** list, size_t list_size) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(str, list[i]) == 0) {
            return i;
        }
    }

    return -1;
}

//---------------str conversion functions------------------//

float str_to_float(const char* str) {
  char* endptr;  // To handle potential parsing errors

  float value = strtof(str, &endptr);

  if (str == endptr) {
      // No conversion occurred (invalid input)
      printf("Invalid float string.\n");
  }
  // else {
  //     printf("Converted float: %f\n", value);}

  return value;
}

uint8_t str_to_uint8(const char* str) {
  char* endptr;             // To handle parsing errors
  errno = 0;                // Reset errno before call

  unsigned long value = strtoul(str, &endptr, 10);  // Base 10 for decimal
  uint8_t result = 0;

  if (str == endptr) {
      // No conversion occurred (invalid input)
      printf("Invalid uint8_t string.\n");
  } else if (errno == ERANGE || value > UINT8_MAX) {
      printf("Value out of uint8_t range.\n");
  } else {
      result = (uint8_t)value;
      //printf("Converted uint8_t: %u\n", result);
  }

  return result;
}


//-----------string operation functions-----------------//



void flip(size_t i, size_t j, char str[]) {
  char temp = str[i];
  str[i] = str[j];
  str[j] = temp;
}

//mutates str
void flip_str(char str[], size_t str_len) {
  for (size_t i = 0; i < str_len/2; i++) {
    //printf("Flipping %c with %c...\n",str[i], str[str_len-i-1]);
    flip(i, str_len-i-1, str);
  }
}

//mutates str pointed to
void flip_str_ptr(char (*str)[20], size_t str_len) {
  for (size_t i = 0; i < str_len/2; i++) {
    //printf("Flipping %c with %c...\n",(*str)[i], (*str)[str_len-i-1]);
    flip(i, str_len-i-1, (*str));
  }
}


void skip_spaces(char* char_ptr) {
  while (isspace((char)*char_ptr)) { //TEST
      char_ptr++;}
}


// Function to trim leading and trailing whitespaces and quotes from a string
char* trim(char* str) {
    char* end;
    // Remove leading whitespace and quotes
    while (isspace((unsigned char)*str) || *str == '"') str++;
    if (*str == 0) return str;
    // Remove trailing whitespace, quotes, and semicolon
    end = str + strlen(str) - 1;
    while (str < end && (isspace((unsigned char)*end) || *end == '"' || *end == ';')) end--;
    *(end + 1) = '\0';
    return str;
}

void parse_line(char* line, char** key, char** value) {
    //using '=' as delimiter
    char* eq_pos = strchr(line, '=');
    if (eq_pos == NULL) {
        *key = NULL;
        *value = NULL;
        return;
    }
    *eq_pos = '\0';  // Split the string at '='
    *key = trim(line);
    //inc the char pointer to give trim the str which starts after '='
    *value = trim(eq_pos + 1);
}

//mutates str[]
// void parse_cmd_w_fnum(char str[], size_t str_len,
//                      char** alphas, size_t* alphas_len,
//                      char** f_chars, size_t* f_chars_len) {
//   size_t m = 0;
//   char* char_ptr = str; //set to start of str
//
//   while (isalpha(*char_ptr) && m < str_len) {
//     char_ptr++;m++;
//   }
//
//   char dest[20];
//   strcpy(dest, str); //avoids setting '\0' at i 0 for f_chars aswell
//
//   *alphas = dest;
//   *alphas_len = m;
//   (*alphas)[m] = '\0';
//
//   *f_chars = &(str[m]);
//   *f_chars_len = str_len - m;
// }



//-------------Example Usage---------------//
// char str[] = "AB123"; size_t str_len = 5;
//
// char A[20]; size_t A_len;
// char* F = str; size_t F_len;
//
//IMPORTANT!, so that we can set '\0' at A[m],
//without affecting F, which is using the str arr memory
// strcpy(A, str);
//
//parse_cmd_w_fnum(str_len,&A,&A_len,&F,&F_len);
//printf("str_len=%lu\nA=%s,%lu\nB=%s,%lu,\n",str_len,A,A_len,F,F_len);


// void parse_cmd_w_fnum(char** c,size_t str_len,
//                               size_t* A_len, char (*A)[20],
//                               size_t* F_len, char (*F)[20]) {
//   //printf("1.:\nstr_len=%lu,A=%s,%lu,F=%s,%lu,\n",str_len,*A,*A_len,*F,*F_len);
//   if (str_len < 2) {
//     fprintf(stderr,"Trying to call parse_cmd_w_fnum with str_len<2\n");
//     return;}
//
//   // if (*F == NULL) {
//   //   fprintf(stderr, "Passing of parameter 'F' failed, in parse_cmd_w_fnum fn!\n");
//   //   return;}
//
//   //printf("2.:\nstr_len=%lu,A=%s,%lu,F=%s,%lu,\n",str_len,*A,*A_len,*F,*F_len);
//
//   //total length of str, which is cmd_w_fnum f.e. "X1.2"-> 4
//   size_t tlen = 0;
//   size_t alen = 0; //len for cmd (alphas) "X" -> 1
//   size_t flen = 0; //len for num (float) "1.2" -> 3
//
//    //read cmd_w_fnum backwards
//    //read fnum
//    while (is_part_of_fnum(**c) && tlen < str_len) {
//      (*F)[flen] = **c;
//
//      //printf("*F=%s\n", *F); printf("**c=%c\n",**c);
//      (*c)--;flen++;
//      tlen++;}
//
//   //read cmd
//   while (isalpha(**c) && tlen < str_len) {
//     (*A)[alen] = **c;
//
//     //printf("*A=%s\n", *A); printf("**c=%c\n",**c);
//     (*c)--;alen++;
//     tlen++;}
//
//   flip_str_ptr(A,alen);
//   flip_str_ptr(F,flen);
//
//   (*A)[alen] = '\0';
//   *A_len = alen;
//
//   *F_len = flen;
//   (*F)[flen] = '\0';
//
//   //printf("3.:\nstr_len=%lu A=%s,%lu F=%s,%lu\n",str_len,*A,*A_len,*F,*F_len);
//
//   //set the char_ptr to the space behind read cmd_w_fnum
//   (*c)+=tlen+1;
// }

//split string across the switch from aplhas to a float num
void parse_cmd_w_fnum(char** c, size_t str_len, char (*A)[20], char (*F)[20]) {
  //printf("1.:\nstr_len=%lu,A=%s,%lu,F=%s,%lu,\n",str_len,*A,*A_len,*F,*F_len);
  if (str_len < 2) {
    fprintf(stderr,"Trying to call parse_cmd_w_fnum with str_len<2\n");
    return;}

  // if (*F == NULL) {
  //   fprintf(stderr, "Passing of parameter 'F' failed, in parse_cmd_w_fnum fn!\n");
  //   return;}

  //printf("2.:\nstr_len=%lu,A=%s,%lu,F=%s,%lu,\n",str_len,*A,*A_len,*F,*F_len);

  //total length of str, which is cmd_w_fnum f.e. "X1.2"-> 4
  size_t tlen = 0;
  size_t alen = 0; //len for cmd (alphas) "X" -> 1
  size_t flen = 0; //len for num (float) "1.2" -> 3

   //read cmd_w_fnum backwards
   //read fnum
   while (is_part_of_fnum(**c) && tlen < str_len) {
     (*F)[flen] = **c;

     //printf("*F=%s\n", *F); printf("**c=%c\n",**c);
     (*c)--;flen++;
     tlen++;}

  //read cmd
  while (isalpha(**c) && tlen < str_len) {
    (*A)[alen] = **c;

    //printf("*A=%s\n", *A); printf("**c=%c\n",**c);
    (*c)--;alen++;
    tlen++;}

  flip_str_ptr(A,alen);
  flip_str_ptr(F,flen);

  (*A)[alen] = '\0';
  (*F)[flen] = '\0';

  //printf("3.:\nstr_len=%lu A=%s,%lu F=%s,%lu\n",str_len,*A,*A_len,*F,*F_len);

  //set the char_ptr to the space behind read cmd_w_fnum
  (*c)+=tlen+1;
}


//ugly static mapping
//use hashmap instead
//smarter solution required
void set_key_value(
  int keypos, const char* read_value,
  size_t di, size_t* ti, data_tuple** tuple_list, track** track_list,
  uint8_t* feat_change) {

    printf("read_value=%s",read_value);

    float fval = 0.0;
    //uint8_t feat_change = 0b000; //bool x y z

    switch (keypos) {
        case 0:  //G (Operating Mode)
            //printf("Setting G=%u\n",str_to_uint8(read_value));
            (*tuple_list)[di].G = str_to_uint8(read_value);
            //(*feat_change) += 1;
            break;
        case 1: //3D Point from mpf file
            //printf("Setting p_x=%f\n",str_to_float(read_value));
            (*tuple_list)[di].P.x = str_to_float(read_value);
            //(*feat_change) += 2;
            break;
        case 2:
            //printf("Setting p_y=%f\n",str_to_float(read_value));
            (*tuple_list)[di].P.y = str_to_float(read_value);
            //(*feat_change) += 4;
            break;
        case 3:
            //printf("Setting p_z=%f\n",str_to_float(read_value));
            (*tuple_list)[di].P.z = str_to_float(read_value);
            (*feat_change) += 8;
            break;
        case 4://LASER_ON
            //track start P is point in front of /LASER_ON
            if(di > 0) {
              printf("Setting Track Start Point to (%f,%f,%f)",
                (*tuple_list)[di-1].P.x,
                (*tuple_list)[di-1].P.y,
                (*tuple_list)[di-1].P.z);

              (*track_list)[*ti].SP.x = (*tuple_list)[di-1].P.x;
              (*track_list)[*ti].SP.y = (*tuple_list)[di-1].P.y;
              (*track_list)[*ti].SP.z = (*tuple_list)[di-1].P.z;
            }


            printf("Setting laser=1\n");
            (*tuple_list)[di].laser = 1;
            //(*feat_change) += 16;
            break;
        case 5://LASER_OFF
            //point before /LASER_OFF is end point of track


            if (di > 0) {
              printf("Setting Track End Point to (%f,%f,%f)",
                (*tuple_list)[di-1].P.x,
                (*tuple_list)[di-1].P.y,
                (*tuple_list)[di-1].P.z);

              (*track_list)[*ti].EP.x = (*tuple_list)[di-1].P.x;
              (*track_list)[*ti].EP.y = (*tuple_list)[di-1].P.y;
              (*track_list)[*ti].EP.z = (*tuple_list)[di-1].P.z;
              (*ti)++;
            }

            printf("Setting laser=0\n");
            (*tuple_list)[di].laser = 0;
            //(*feat_change) += 16;
            break;

        case 6://PUIS_LASER
            printf("Setting laser_power=%f\n",str_to_float(read_value));
            fval = str_to_float(read_value);
            (*tuple_list)[di].laser_power = fval;
            (*track_list)[*ti].laser_power = fval;
            //(*feat_change) += 32;
            break;

        case 7://VIT_TIR
            printf("Setting machine_speed=%f\n",str_to_float(read_value));
            fval = str_to_float(read_value);   //sanity check ToDo revert!
            (*tuple_list)[di].machine_speed = str_to_float(read_value);
            (*track_list)[*ti].machine_speed = str_to_float(read_value); //gets set multiple times per same track
            //(*feat_change) += 64;
            break;
        default:
            fprintf(stderr,"Invalid keypos=%d",keypos);
            break;
    }
}

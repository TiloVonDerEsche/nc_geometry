#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

//used for variable str_float hashmap
#include "khashl.h"
KHASHL_MAP_INIT(KH_LOCAL,
  strfloat_t, strfloat,
  const char*, float,
  kh_hash_str, kh_eq_str)


typedef struct {
  size_t mpf_lines;
  size_t precision;
  size_t max_line_len;

  char mpf_file[256];
  char data_tuples_csv[256];
  char track_list_csv[256];

  float horizontal_radius;
  float vertical_radius;
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
  //set by read_mpf
  vec3D A;
  vec3D B;
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

vec3D vec_add_scaled(vec3D s, double scalar, vec3D t) {
  return (vec3D) {s.x + scalar * t.x, s.y + scalar * t.y, s.z + scalar * t.z};
}

vec3D norm_vec(vec3D v) {
  double v_len = vec_len(v);
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


// Function to trim leading and trailing whitespaces and quotes from a string
char* trim(char* str) {
    char* end;
    // Remove leading whitespace and quotes
    while (isspace((unsigned char)*str) || *str == '"') str++;
    if (*str == 0) return str;
    // Remove trailing whitespace, quotes, and semicolon
    end = str + strlen(str) - 1;
    while (end > str && (isspace((unsigned char)*end) || *end == '"' || *end == ';')) end--;
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

float str_to_float(const char* str) {
  char* endptr;  // To handle potential parsing errors

  float value = strtof(str, &endptr);

  if (str == endptr) {
      // No conversion occurred (invalid input)
      printf("Invalid float string.\n");
  } else {
      printf("Converted float: %f\n", value);
  }

  return value;
}

uint8_t str_to_uint8(const char* str) {
  char* endptr;             // To handle parsing errors
  errno = 0;                // Reset errno before call

  unsigned long value = strtoul(str, &endptr, 10);  // Base 10 for decimal

  if (str == endptr) {
      // No conversion occurred (invalid input)
      printf("Invalid uint8_t string.\n");
  } else if (errno == ERANGE || value > UINT8_MAX) {
      // Out of range for uint8_t (0-255)
      printf("Value out of uint8_t range.\n");
  } else {
      uint8_t result = (uint8_t)value;
      printf("Converted uint8_t: %u\n", result);
  }

  return 0;
}


uint8_t is_part_of_num(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }

    // Check for specific characters: '-', '.', '/', 'e', '^'
    if (c == '+' || c == '-' || c == '.' || c == 'e' || c == '^') {
        return 1;
    }

    return 0;
}


int is_in_list(const char* str, const char** list, size_t list_size) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(str, list[i]) == 0) {
            return i;
        }
    }

    return -1;
}

//ugly static mapping
//use hashmap instead
//smarter solution required
void set_key_value(
  int keypos, const char* read_value,
  int tui, int tri, data_tuple** tuple_list, track** track_list,
  uint8_t* dim_changed) {

    float fval = 0.0;
    //uint8_t dim_changed = 0b000; //bool x y z

    switch (keypos) {
        case 0: //LASER_ON
            (*tuple_list)[tui].laser = 1;
            break;
        case 1: //LASER_OFF
            (*tuple_list)[tui].laser = 0;
            break;
        case 2: //PUIS_LASER
            fval = str_to_float(read_value);
            (*tuple_list)[tui].laser_power = fval;
            (*track_list)[tri].laser_power = fval;
            break;
        case 3: //VIT_TIR
            fval = str_to_float(read_value);
            (*tuple_list)[tui].machine_speed = fval;
            (*track_list)[tri].machine_speed = fval;
            break;
        case 4: //3D Point from mpf file
            (*tuple_list)[tui].P.x = str_to_float(read_value);
            (*dim_changed) = 4;
            break;
        case 5:
            (*tuple_list)[tui].P.y = str_to_float(read_value);
            (*dim_changed) = 2;
            break;
        case 6:
            (*tuple_list)[tui].P.z = str_to_float(read_value);
            (*dim_changed) = 1;
            break;
        case 7: //G (Operating Mode)
            (*tuple_list)[tui].G = str_to_uint8(read_value);
            break;
        default:
            fprintf(stderr,"Invalid keypos=%d",keypos);
            break;
    }
}

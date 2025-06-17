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
  uint8_t left;
  uint8_t bot_left;
  uint8_t bottom;
  uint8_t bot_right;
  uint8_t right;
} coll_ma;
//collision_matrix

typedef struct {
  vec3D A;
  vec3D B;
  float hradius;
  float vradius;
  coll_ma M;
} track;

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

vec3D connecting_vec(vec3D v1, vec3D v2) {
  return (vec3D) {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
}

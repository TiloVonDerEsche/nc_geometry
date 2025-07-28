#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct {
    size_t mpf_lines;
    size_t precision;
    size_t max_line_len;

    char file_name[256];
    char output_csv_name[256];

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
  vec3D A;
  vec3D B;
  float hradius;
  float vradius;
  vec3D coll_vec;
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
        printf("Error: Direction vector cannot be zero vec.\n");
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

int read_config(const char* filename, Config* config) {
    FILE* file = fopen(filename, "r");
    if (!file) {
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
        char* key = strtok(trimmed, "= ");  // Include space in delimiter to handle spaces around '='
        char* value = strtok(NULL, ";");
        if (!key || !value) {
            fprintf(stderr, "Error: Invalid line format: %s\n", line);
            continue;
        }
        key = trim(key);
        value = trim(value);

        if (strcmp(key, "mpf_lines") == 0) {
            config->mpf_lines = (size_t)atoi(value);
        } else if (strcmp(key, "precision") == 0) {
            config->precision = (size_t)atoi(value);
        } else if (strcmp(key, "max_line_len") == 0) {
            config->max_line_len = (size_t)atoi(value);
        } else if (strcmp(key, "file_name") == 0) {
            strncpy(config->file_name, value, sizeof(config->file_name) - 1);
            config->file_name[sizeof(config->file_name) - 1] = '\0';
        } else if (strcmp(key, "output_csv_name") == 0) {
            strncpy(config->output_csv_name, value, sizeof(config->output_csv_name) - 1);
            config->output_csv_name[sizeof(config->output_csv_name) - 1] = '\0';
        } else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        } else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        } else {
            fprintf(stderr, "Warning: Unknown key: %s\n", key);
        }
    }

    fclose(file);
    return 0;
}

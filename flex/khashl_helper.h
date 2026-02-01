#include "khashl.h"
#include <ctype.h>

KHASHL_MAP_INIT(KH_LOCAL,
  strfloat_t, strfloat,
  const char*, float,
  kh_hash_str, kh_eq_str)


//print in JSON object syntax
void print_hashmap(strfloat_t* h, FILE* destination) {
  //Print all variables from hashmap
  khint_t k;
  fprintf(destination,"{");

  kh_foreach(h, k) {
      if (k < kh_end(h)) {  // Check if found (k != end iterator)
          fprintf(destination,"\"%s\":\"%f\",", kh_key(h, k), kh_val(h, k));
      }
  }

  //only works if destination is seekable, a pipe f.e. is not
  fseek(destination, -1, SEEK_CUR); //move to pos of last redundant comma
  fprintf(destination,"},\n"); //replace last comma
}

//=========================================================//
//---------------------Helper Functions--------------------//
//=========================================================//

typedef struct {
  float x;
  float y;
  float z;
} vec3D;

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

int read_config(const char* fpath, Config* config) {
    printf("Opening %s...\n",fpath);
    FILE* file = fopen(fpath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open config file %s\n", fpath);
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);

        //ignore comments and empty lines
        if (strlen(trimmed) == 0 || trimmed[0] == '/' || trimmed[0] == '#') {
            continue;
        }

        char* key;
        char* value;

        parse_line(trimmed, &key, &value);
        if (!key || !value) {
            fprintf(stderr,
              "Error: Invalid line format: %s!\
              \r\nLine should contain <key>'='<value>;",line);
            continue;
        }

        if (strcmp(key, "lines_to_read") == 0) {
            config->lines_to_read = (size_t)atoi(value);
        }
        else if (strcmp(key, "max_line_len") == 0) {
            config->max_line_len = (size_t)atoi(value);
        }
        else if (strcmp(key, "mpf_file") == 0) {
            strncpy(config->mpf_file, value, sizeof(config->mpf_file) - 1);
            config->mpf_file[sizeof(config->mpf_file) - 1] = '\0';
        }
        else if (strcmp(key, "data_tuples_csv") == 0) {
            strncpy(config->data_tuples_csv, value, sizeof(config->data_tuples_csv) - 1);
            config->data_tuples_csv[sizeof(config->data_tuples_csv) - 1] = '\0';
        }
        else if (strcmp(key, "track_list_csv") == 0) {
            strncpy(config->track_list_csv, value, sizeof(config->track_list_csv) - 1);
            config->track_list_csv[sizeof(config->track_list_csv) - 1] = '\0';
        }
        else if (strcmp(key, "interpolation_csv") == 0) {
            strncpy(config->intpol_csv, value, sizeof(config->intpol_csv) - 1);
            config->intpol_csv[sizeof(config->intpol_csv) - 1] = '\0';
        }
        else if (strcmp(key, "horizontal_radius") == 0) {
            config->horizontal_radius = atof(value);
        }
        else if (strcmp(key, "vertical_radius") == 0) {
            config->vertical_radius = atof(value);
        }
        else if (strcmp(key, "step_distance") == 0) {
            config->step_dis = atof(value);
        }
        else if (strcmp(key, "tracks_to_plot") == 0) {
          //Do nothing; Used by t_vis_color.c
        }
        else {
            fprintf(stderr, "Warning: Unknown key: %s in config file: %s!\n", key,fpath);
        }
    }

    fclose(file);
    return 0;
}










































//csv nonsense

//print one line of hmhis, which is fnums seperated by ','
// void print_values_to_hmhis(strfloat_t* h, FILE* hmhis) {
//   //Print all variables from hashmap
//   khint_t k;
//   kh_foreach(h, k) {
//       if (k < kh_end(h)) {  // Check if found (k != end iterator)
//           printf("Printing k=%d: value=%f\n", k, kh_val(h, k));
//           fprintf(hmhis,"%f,", kh_val(h, k));
//       }
//   }
//
//   fprintf(hmhis,"\n");
// }

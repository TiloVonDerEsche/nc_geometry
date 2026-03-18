#include "helper.h"

//---khashmap_helper-----//

strfloat_t* init_hashmap() {
  khint_t k;
  int absent;

  strfloat_t* h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 0;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");
  kh_val(h, k) = 0;

  return h;
}

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

FILE* init_file(char* f_path, char* f_header) {
  printf("Opening: %s in write mode...\n",f_path);
  FILE* fp = fopen(f_path, "w");
  if (fp == NULL) {
      fprintf(stderr, "Error: Could not open %s (in write mode)!\n",f_path);
      exit(-1);
  }

  fprintf(fp,"%s\n",f_header);

  return fp;
}
////////////////////////////////////////////////////////////




void set_var_incr(char* varname, float fnum) {
    if (incr_mode && is_coord(varname[0])) {
      set_var(varname, get_var_val(varname)+fnum);
    }
    else {
      set_var(varname,fnum);
    }
}

void set_var(char* varname, float fnum) {
  khint_t k;
  int absent;

  k = strfloat_put(h, varname, &absent);
  if (absent) {
    kh_key(h, k) = strdup(varname);}
  kh_val(h, k) = fnum;
  //printf("Set %s to %f\n", varname, fnum);
}

float get_var_val(char* varname) {
  if (!varname) return 0;

  khint_t k;
  int absent;
  k = strfloat_get(h, varname);
  if ( kh_exist(h, k) ) {
    return kh_val(h, k);
  }

  return 0;
}


//file functions

void close_hmhis() {
  //delete last redundant comma
  fseek(hmhis, -3, SEEK_CUR);
  fprintf(hmhis,"]");
  fclose(hmhis);
}



void write_track_line() {
  fprintf(tl,"%lu, %f, %f, %f, %f, %f, %f, %f, %f, 0, 0, 0, %f, %f\n",
  tid++, A.x, A.y, A.z, B.x, B.y, B.z,
  get_var_val("PUIS_LASER"), get_var_val("VIT_TIR"),
  //coll_vec,
  config.hrad, config.vrad);
}


//------Math Functions------//

vec3D rot_x(vec3D p, float t) {
   float rad = TO_RAD(t);
   float st = sin(rad);
   float ct = cos(rad);
   return (vec3D) {
   p.x,
   p.y * ct - p.z * st,
   p.y * st + p.z * ct
   };
}


vec3D rot_y(vec3D p, float t) {
   float rad = TO_RAD(t);
   float st = sin(rad);
   float ct = cos(rad);
   return (vec3D) {
   p.x * ct + p.z * st,
   p.y,
   -p.x * st + p.z * ct
   };
}


vec3D rot_z(vec3D p, float t) {
   float rad = TO_RAD(t);
   float st = sin(rad);
   float ct = cos(rad);
   return (vec3D) {
   p.x * ct - p.y * st,
   p.x * st + p.y * ct,
   p.z
   };
}

vec3D rot_xyz(vec3D p, vec3D abc) {
   return rot_x(rot_y(rot_z(p,abc.z),abc.y),abc.x);
}

vec3D rot_point() {
    vec3D p = {
          get_var_val("X"),
          get_var_val("Y"),
          get_var_val("Z")};
    vec3D abc = {
          get_var_val("A"),
          get_var_val("B"),
          get_var_val("C")};

    return rot_xyz(p,abc);
}

//------------String Functions--------------//
int is_coord(char c) {
  return (c == 'X' || c == 'Y' || c == 'Z' || c == 'A' || c == 'B' || c == 'C');
}

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

//------------File Functions-------------------//

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

        if (strcmp(key, "debug_prints") == 0) {
            config->debug_prints = atoi(value);
        }
        else if (strcmp(key, "hmhis_to_stdout") == 0) {
            config->hmhis_to_stdout = atoi(value);
        }
        else if (strcmp(key, "hmhis_to_file") == 0) {
            config->hmhis_to_file = atoi(value);
        }
        else if (strcmp(key, "tracks_def_by_laser") == 0) { 
            config->tracks_def_by_laser = atoi(value);
        }
         else if (strcmp(key, "track_mid_len") == 0) {
            config->track_mid_len = atoi(value);
        }

        else if (strcmp(key, "mpf_file") == 0) {
            strncpy(config->mpf_file, value, sizeof(config->mpf_file) - 1);
            config->mpf_file[sizeof(config->mpf_file) - 1] = '\0';
        }
        else if (strcmp(key, "track_list_csv") == 0) {
            strncpy(config->track_list_csv, value, sizeof(config->track_list_csv) - 1);
            config->track_list_csv[sizeof(config->track_list_csv) - 1] = '\0';
        }
        else if (strcmp(key, "hmhis_json") == 0) {
            strncpy(config->hmhis_json, value, sizeof(config->hmhis_json) - 1);
            config->hmhis_json[sizeof(config->hmhis_json) - 1] = '\0';
        }
        else if (strcmp(key, "tracks_to_plot") == 0) {
          //Do nothing; Used by t_vis_color.c
        }
        else if (strcmp(key, "interpolation_csv") == 0) {}
        else if (strcmp(key, "horizontal_radius") == 0) {
            config->hrad = atof(value);
        }
        else if (strcmp(key, "vertical_radius") == 0) {
            config->vrad = atof(value);
        }
        else if (strcmp(key, "step_distance") == 0) {}
        else if (strcmp(key, "track_accel_margin") == 0) {}
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

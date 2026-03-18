#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include "khashl.h"

//===========Structs===========//
typedef struct {
  float x;
  float y;
  float z;
} vec3D;

typedef struct {
  int debug_prints;
  int hmhis_to_stdout;
  int hmhis_to_file;
  int tracks_def_by_laser;
  int track_mid_len;

  char mpf_file[256];
  char track_list_csv[256];
  char hmhis_json[256];

  float hrad;
  float vrad;
} Config;

//===========Macros===========//

KHASHL_MAP_INIT(KH_LOCAL,
  strfloat_t, strfloat,
  const char*, float,
  kh_hash_str, kh_eq_str)


#define PI 3.14159265358979323846
#define TO_RAD(deg) ((deg) * PI / 180.0)

//===========Extern Vars===========//
extern long byte_counter;
extern int debug;
extern Config config;

extern strfloat_t* h;

extern size_t tid;

extern FILE* yyin;

extern FILE* tl;
extern FILE* hmhis;

extern vec3D A;
extern vec3D B;

extern int incr_mode;

//===========Functions===========//
strfloat_t* init_hashmap();

//print in JSON object syntax
void print_hashmap(strfloat_t* h, FILE* destination);

FILE* init_file(char* f_path, char* f_header);


void set_var_incr(char* varname, float fnum);

void set_var(char* varname, float fnum);

float get_var_val(char* varname);


//file functions

void close_hmhis();



void write_track_line();



//------Math Functions------//


vec3D rot_x(vec3D p, float t);


vec3D rot_y(vec3D p, float t);


vec3D rot_z(vec3D p, float t);

vec3D rot_xyz(vec3D p, vec3D abc);

//------------String Functions--------------//
int is_coord(char c);

char* trim(char* str);

void parse_line(char* line, char** key, char** value);

//------------File Functions-------------------//

int read_config(const char* fpath, Config* config);

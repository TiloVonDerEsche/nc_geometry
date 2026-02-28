%{
  #include <stdio.h>
  #include <math.h>

  #include "khashl_helper.h"

  int yylex (void);
  int yyerror(char* s);
  void jump(char*);

  vec3D rot_point();

  void set_var_incr(char*, float);
  void set_var(char*, float);
  float get_var_val(char*);
  strfloat_t* init_hashmap();

  strfloat_t* h;

  FILE* hmhis;
  FILE* tl;

  extern long byte_counter;

  char* pending_jump_label = NULL;
  int jump_requested = 0;
  int incr_mode = 0;
  int skip = 0;

  size_t tid = 1;
  vec3D A;
  vec3D B;

  Config config = {0};
  int debug;
%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token SEP NEWLINE SEMICOLON COMMENT OTHER
%token <char*> STRING
%token <int> INT
%token <float> FLOAT

%token IF ENDIF GOTO

%token <char*> CMD
%token <char*> VAR
%token <char*> SPECIAL_CMD
%token <char*> CUSTOM_VAR
%token CALL
%token <char*> MISC_ID
%token <char*> LABEL

%nterm <float> val
%nterm <float> fn
%nterm <float> arith_expr
%nterm <int> bool_expr

%left '+' '-'
%left '*' '/'

%%
prog:
  lines
;

lines:
  %empty
  | lines line NEWLINE
;

line:
  opt_seps
  | opt_seps opt_skip exprs opt_seps
                        {
                         if(config.hmhis_to_file) {print_hashmap(h, hmhis);}
                         if(config.hmhis_to_stdout) {print_hashmap(h, stdout);}

                         if (jump_requested) {
                              jump_requested = 0;
                              skip = 0;
                              char* target = pending_jump_label;
                              pending_jump_label = NULL;
                              jump(target);
                              free(target);
                         }
                        }
  | error
;

opt_skip:
    %empty
    | '/'
;

opt_seps:
  %empty
  | seps
;

seps:
  SEP
  | seps SEP
;

exprs:
  expr
  | exprs seps expr;
;

expr:
  IF SEP bool_expr {
      if (!$3) {
          skip++;
          if(debug) {printf("Skip=%d\n",skip);}
      }
  } if_body ENDIF {
      if (skip > 0) {
        skip--;
        if(debug) {printf("Skip=%d\n",skip);}
      }
    }
  | CMD arith_expr       {
                          if(!skip){
                            set_var($1,$2);

                            if($1[0] == 'G') {
                              switch((int)$2) {
                                case 90: incr_mode=0;break;
                                case 91: incr_mode=1;break;
                                default: break;
                              }
                            }
                          }
                         }
  | assignment
  | LABEL                {
                          if(!skip){
                            if (get_var_val($1) == 0) { //define once
                              set_var($1, (float)byte_counter);

                              char line_label[256];
                              snprintf(line_label, sizeof(line_label), "%s_line", $1);
                              set_var(line_label, get_var_val("line"));
                            }
                          }
                         }
  | GOTO SEP MISC_ID   {if(!skip){
                          pending_jump_label = strdup($3);
                          jump_requested = 1;

                          // Increment skip so the parser ignores everything
                          // until it reaches the end of the current IF or line
                          skip = 1;
                          }
                        }
  | SPECIAL_CMD          {
                          if(strcmp($1,"LASER_ON") == 0) {
                            set_var("laser",1);
                            A = rot_point();
                          }
                          else if(strcmp($1,"LASER_OFF") == 0) {
                            set_var("laser",0);
                            B = rot_point();

                            fprintf(tl,"%lu, %f, %f, %f, %f, %f, %f, %f, %f, 0, 0, 0, %f, %f\n",
                            tid++, A.x, A.y, A.z, B.x, B.y, B.z,
                            get_var_val("PUIS_LASER"), get_var_val("VIT_TIR"),
                            //coll_vec,
                            config.hrad, config.vrad);
                          }
                         }
  | CALL SEP STRING
  | MISC_ID
  | fn
  | COMMENT
;

if_body:
  %empty
  | if_body if_element
;

if_element:
  expr
  | SEP
  | NEWLINE
;


assignment:
  CMD opt_seps '=' opt_seps arith_expr        {if(!skip){set_var_incr($1,$5);}}
  | VAR opt_seps '=' opt_seps arith_expr          {if(!skip){set_var($1,$5);}}
  | CUSTOM_VAR opt_seps '=' opt_seps arith_expr {if(!skip){set_var($1,$5);}}
  | CUSTOM_VAR SEP arith_expr {if(!skip){set_var($1,$3);}}
;


val:
  VAR            {
                  /*printf("Getting VAR=%s\n",$1);*/
                  $$ = get_var_val($1);
                 }
  | CUSTOM_VAR   {
                  /*printf("Getting CUSTOM_VAR=%s\n",$1);*/
                  $$ = get_var_val($1);
                 }
  | INT          {$$=$1;}
  | FLOAT        {$$=$1;}
;

fn:
  MISC_ID '(' params ')' {$$=0;}
;

params:
  %empty
  | arith_expr
  | MISC_ID
  | params ',' arith_expr
  | params ',' MISC_ID
;

arith_expr:
  val         {$$=$1;}
  | fn        {$$=$1;}
  | arith_expr '+' arith_expr {$$=$1+$3; /*printf("%f+%f=%f\n", $1,$3,$$);*/}
  | arith_expr '-' arith_expr {$$=$1-$3;}
  | arith_expr '*' arith_expr {$$=$1*$3;}
  | arith_expr '/' arith_expr {$$=$1/$3;}
  | '(' arith_expr ')'        {$$=$2;}
  | '-' arith_expr            {$$=-$2;}
;

bool_expr:
  arith_expr '<' arith_expr {$$=$1<$3;}
  | arith_expr '>' arith_expr {$$=$1>$3;}
  | arith_expr '=' '=' arith_expr {$$=$1==$4;}
  | arith_expr '!' '=' arith_expr {$$=$1!=$4;}
  | arith_expr '<' '=' arith_expr {$$=$1<=$4;}
  | arith_expr '>' '=' arith_expr {$$=$1>=$4;}
  | '!' bool_expr                 {$$=!$2;}
  | '(' bool_expr ')'             {$$=$2;}
;


%%

void jump(char* label_name) {
    //size_t llen = strlen(label_name);
    //printf("\nlabel_len=%zu\n",llen);

    float offset = get_var_val(label_name); //+ strlen(label_name) + 1;

    if (debug) {
      khint_t k;
      int absent;

      k = strfloat_get(h, label_name);
      if ( kh_exist(h, k) ) {
        printf("Found in hm: label_name=%s\n",label_name);
        //offset = kh_val(h, k);
      }
      else {
        printf("NOT found in hm: label_name=%s\n",label_name);
      }
    }

    if (offset >= 0) {
        char line_label[256];
        snprintf(line_label, sizeof(line_label), "%s_line", label_name);
        float original_line = get_var_val(line_label);

        set_var("line", original_line); //reset line to line of label
        byte_counter = offset;          //reset byte_counter to offset of label
        skip = 0;
        jump_requested = 0;

        if(debug){
          printf("Jumping to line=%d, offset=%d\n\n",
                (int)original_line,(int)offset);
        }

        fseek(yyin, (long)offset, SEEK_SET);
        yyrestart(yyin); //Tells Flex to flush buffers and read from yyin again
        yy_flush_buffer(YY_CURRENT_BUFFER);
    }
}

//hashmap fns
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


void set_var_incr(char* varname, float fnum) {
    if (incr_mode) {
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

//file functions

void close_hmhis() {
  //delete last redundant comma
  fseek(hmhis, -3, SEEK_CUR);
  fprintf(hmhis,"]");
  fclose(hmhis);
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

//bison fns

int yyerror(char* s)
{
	printf("Error: %s, in line: %d\n", s, (int)get_var_val("line"));
	return 0;
}

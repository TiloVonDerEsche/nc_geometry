%{
  #include <stdio.h>
  #include <math.h>

  #include "helper.h"

  extern Config config;   
  extern int debug;
  extern strfloat_t* h;

  extern FILE* hmhis;
  extern FILE* tl;

  extern FILE* yyin;

  extern void yyrestart(FILE*);
  //struct yy_buffer_state;
  //typedef struct yy_buffer_state *YY_BUFFER_STATE;
  //extern YY_BUFFER_STATE yy_current_buffer; 

  //#define YY_CURRENT_BUFFER yy_current_buffer

  int yylex (void);
  int yyerror(char* s);
  void jump(char*);

  vec3D rot_point();

  void set_var_incr(char*, float);
  void set_var(char*, float);
  float get_var_val(char*);
  strfloat_t* init_hashmap();
  void write_track_line();
  int is_coord(char);

  char* pending_jump_label = NULL;
  int jump_requested = 0;
  int incr_mode = 0;
  int skip = 0;
  int track_written = 0;

  size_t tid = 0;
  vec3D A = {0,0,0};
  vec3D B = {0,0,0};

%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token SEP NEWLINE END OTHER
%token COMMENT

%token IF ENDIF
%token GOTO REPEAT
%token CALL
%token <char*> MISC_ID

%token <char*> LABEL
%token <char*> CMD
%token <char*> VAR
%token <char*> SPECIAL_CMD
%token <char*> CUSTOM_VAR

%token <char*> STRING
%token <int> INT
%token <float> FLOAT

%nterm <float> val
%nterm <float> fn
%nterm <float> arith_expr
%nterm <int> bool_expr

%left '+' '-'
%left '*' '/'

%%    

prog:
  lines YYEOF {printf("%lu tracks written to %s!\n",tid,config.track_list_csv);}
  | YYEOF     { printf("Warning: File %s is empty!\n",config.mpf_file); }
;

lines:
  %empty
  | lines line NEWLINE
;

line:
  opt_seps
  | opt_seps opt_skip exprs opt_seps
    {
     track_written--; //counter for how many lines not to print_track

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
                            if($1[0] == 'G') {
                              switch((int)$2) {
                                case 90: incr_mode=0;break;
                                case 91: incr_mode=1;break;
                                default: break;
                              }
                            }
                            else if(is_coord($1[0])
                              && !config.tracks_def_by_laser && !(track_written>0)) {
                              B = rot_point();
                              write_track_line();
                              A = rot_point();
                              //prevent mult writes in line with mult coord cmds
                              track_written = config.track_mid_len;
                            }

                            set_var($1,$2);
                          }
                         }
  | assignment
  | LABEL                //already handled in find_labels.h
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
                            if (config.tracks_def_by_laser){A=rot_point();}

                          }
                          else if(strcmp($1,"LASER_OFF") == 0) {
                            set_var("laser",0);
                            if (config.tracks_def_by_laser){B=rot_point();}

                            write_track_line();
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

    float offset = get_var_val(label_name);

    if (offset >= 0) {
        char line_label[256];
        snprintf(line_label, sizeof(line_label), "%s_line", label_name);
        float original_line = get_var_val(line_label);

        set_var("line", original_line); //reset line to line of label
        //byte_counter = offset;          //reset byte_counter to offset of label
        skip = 0;
        jump_requested = 0;

        if(debug){
          printf("Jumping to line=%d, offset=%d\n\n",
                (int)original_line,(int)offset);
        }

        fseek(yyin, (long)offset, SEEK_SET);
        yyrestart(yyin); //Tells Flex to flush buffers and read from yyin again
        //yy_flush_buffer(YY_CURRENT_BUFFER);
    }
}

void write_track_line() {
  fprintf(tl,"%lu, %f, %f, %f, %f, %f, %f, %f, %f, 0, 0, 0, %f, %f\n",
  tid++, A.x, A.y, A.z, B.x, B.y, B.z,
  get_var_val("PUIS_LASER"), get_var_val("VIT_TIR"),
  //coll_vec,
  config.hrad, config.vrad);
}

void set_var_incr(char* varname, float fnum) {
    if (incr_mode && is_coord(varname[0])) {
      set_var(varname, get_var_val(varname)+fnum);
    }
    else {
      set_var(varname,fnum);
    }
}

void close_hmhis() {
  //delete last redundant comma
  fseek(hmhis, -3, SEEK_CUR);
  fprintf(hmhis,"]");
  fclose(hmhis);
}

int yyerror(char* s) {
	printf("Error: %s, in line: %d\n", s, (int)get_var_val("line"));
	return 0;
}

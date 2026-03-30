%{
  #include <stdio.h>
  #include <math.h>

  #include "helper.h"
  #include "stack.c"

  extern Config config;
  extern int debug;
  extern strfloat_t* h;

  extern FILE* hmhis;
  extern FILE* tl;

  extern FILE* yyin;

  extern void yyrestart(FILE*);

  int yylex (void);
  int yyerror(char* s);

  int is_coord(char);
  vec3D net_point();

  strfloat_t* init_hashmap();
  float get_var_val(char*);
  void set_var_rot(char, float);
  void set_var_incr(char*, float);
  void set_var(char*, float);

  void write_track_line();
  void modify_tl();

  void request_jump(char*);
  void jump(size_t,long);
  void handle_repeat(char*, char*);

  size_t target_line = 0;
  long target_byte_offset = 0;

  int jump_requested = 0;
  int skip = 0;
  int incr_mode = 0;
  int rot_mode = 0;

  int track_written = 0; //bad name, this is a counter to skip lines

  size_t tid = 0;
  vec3D A = {0,0,0};
  vec3D B = {0,0,0};

  Stack ret_stack;
%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token SEP NEWLINE END OTHER
%token COMMENT
%token MSG

%token IF ENDIF
%token GOTO REPEAT
%token ROT
%token <char*> MISC_ID

%token <char*> LABEL
%token <char*> VAR
%token <char*> CMD
%token <char*> SPECIAL_CMD
%token <char*> CUSTOM_VAR

%token <char*> STRING
%token <int> INT
%token <float> FLOAT

%nterm <float> val
%nterm <float> fn
%nterm <float> arith_expr
%nterm <int> bool_expr
%nterm <int> lines
%nterm <int> prog

%left '+' '-'
%left '*' '/'

%nonassoc LOW_PREC
%left SEP

%initial-action {
    init_stack(&ret_stack);
    //h = init_hashmap(); //h is global in helper.c for now
}

%%

prog:
  lines YYEOF {
    printf("%lu tracks written to %s!\n",tid,config.track_list_csv);
    if ($1 == 0) {
      printf("Warning: File %s is empty!\n",config.mpf_file);
    }
  }
;

lines:
  %empty {$$ = 0;}
  | lines line NEWLINE {$$=$1+1;}
;

line:
  opt_seps
  | opt_seps opt_skip exprs opt_seps
    {
     rot_mode = 0; //reset linewise
     track_written--; //counter for how many lines not to print_track

     if(config.hmhis_to_file) {print_hashmap(h, hmhis);}
     if(config.hmhis_to_stdout) {print_hashmap(h, stdout);}

     if (jump_requested) {
          jump_requested = 0;
          skip = 0;
          jump(target_line,target_byte_offset);
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
  | exprs seps expr
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
                                case 54: origin_offset=(vec3D){0,0,0};break;
                                case 55: origin_offset=(vec3D){10,0,0};break;
                                case 56: origin_offset=(vec3D){0,10,0};break;
                                case 57: origin_offset=(vec3D){0,0,10};break;
                                case 58: origin_offset=(vec3D){10,15,0};break;
                                case 59: origin_offset=(vec3D){0,10,20};break;
                                case 90: incr_mode=0;break;
                                case 91: incr_mode=1;break;
                                default: break;
                              }
                            }
                            else if (is_coord($1[0])) {
                              if (($1[0] == 'X' || $1[0] == 'Y' || $1[0] == 'Z' )
                                && rot_mode) {
                                  set_var_rot($1[0], $2);
                              }

                              else if (!config.tracks_def_by_laser && !(track_written>0))
                              {
                                B = net_point();
                                write_track_line();
                                A = net_point();
                                //prevent mult writes in line with mult coord cmds
                                track_written = config.track_mid_len;
                              }
                            }

                            if (!rot_mode) {
                            set_var($1,$2);}
                          }
                         }
  | ROT                {rot_mode = 1;}
  | assignment
  | LABEL              {
                        if(!skip && !is_empty(&ret_stack)){

                          Elem top;
                          peek(&ret_stack, &top);

                          if(strcmp($1,top.label) == 0) {
                            jump_requested = 1;
                            skip = 1;

                            target_line = top.line;
                            target_byte_offset = top.byte_offset;

                            pop(&ret_stack, &top);
                          }
                        }
                       }
  | GOTO SEP MISC_ID   {if(!skip){
                          request_jump($3);
                          }
                        }
  | REPEAT SEP MISC_ID %prec LOW_PREC
                        {if(!skip){
                            handle_repeat($3,"END_LABEL");
                          }
                        }
  | REPEAT SEP MISC_ID SEP MISC_ID
                        {if(!skip){
                            handle_repeat($3,$5);
                          }
                        }
  | SPECIAL_CMD          {
                          if(strcmp($1,"LASER_ON") == 0) {
                            set_var("laser",1);
                            if (config.tracks_def_by_laser){A=net_point();}

                          }
                          else if(strcmp($1,"LASER_OFF") == 0) {
                            set_var("laser",0);
                            if (config.tracks_def_by_laser){B=net_point();}

                            write_track_line();
                          }
                         }
  | MSG SEP STRING
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
  CMD opt_seps '=' opt_seps arith_expr
  {if(!skip){
    if (($1[0] == 'X' || $1[0] == 'Y' || $1[0] == 'Z' ) && rot_mode) {
      set_var_rot($1[0],$5);
    }
    else {
     set_var_incr($1,$5);
  }}}
  | VAR opt_seps '=' opt_seps arith_expr        {if(!skip){set_var($1,$5);}}
  | CUSTOM_VAR opt_seps '=' opt_seps arith_expr {if(!skip){set_var($1,$5);}}
  | CUSTOM_VAR SEP arith_expr                   {if(!skip){set_var($1,$3);}}
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
  arith_expr '<' arith_expr       {$$=$1<$3;}
  | arith_expr '>' arith_expr     {$$=$1>$3;}
  | arith_expr '=' '=' arith_expr {$$=$1==$4;}
  | arith_expr '!' '=' arith_expr {$$=$1!=$4;}
  | arith_expr '<' '=' arith_expr {$$=$1<=$4;}
  | arith_expr '>' '=' arith_expr {$$=$1>=$4;}
  | '!' bool_expr                 {$$=!$2;}
  | '(' bool_expr ')'             {$$=$2;}
;


%%
void request_jump(char* target) {
  jump_requested = 1;
  skip = 1;

  char line_label[32+6];
  snprintf(line_label, sizeof(line_label), "%s_line", target);
  target_line = (size_t)get_var_val(line_label);
  target_byte_offset = get_var_val(target);
}

void jump(size_t target_line,long target_byte_offset) {
  if (target_byte_offset >= 0) {
      set_var("line", target_line);
      skip = 0;
      jump_requested = 0;

      if(debug){
        printf("Jumping to line=%lu, offset=%ld\n\n",
        target_line,target_byte_offset);}

      fseek(yyin, target_byte_offset, SEEK_SET);
      yyrestart(yyin);
  }
}

void handle_repeat(char* start_label, char* end_label) {
  request_jump(start_label);

  size_t line = (size_t)get_var_val("line");;

  char label[1024]; //non-sense size for now
  snprintf(label, sizeof(label), "REPEAT_%lu", line);

  push(&ret_stack,
    strdup(end_label), //return label
    line,       //return line
    (long)get_var_val(label) //return byte_offset
  );

  if (debug) {
    Elem temp;
    if (peek(&ret_stack, &temp)) {
        printf("Return: Label %s, Line: %zu, Offset: %ld)\n",
                temp.label, temp.line, temp.byte_offset);
    }
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

int yyerror(char* s) {
	printf("Error: %s, in line: %d\n", s, (int)get_var_val("line"));
	return 0;
}

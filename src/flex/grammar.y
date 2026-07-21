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
  void handle_repeat(char*, char*, size_t);

  size_t target_line = 0;
  long target_byte_offset = 0;

  int jump_requested = 0;
  int skip = 0;
  int incr_mode = 0;
  int rot_mode = 0;

  int track_written = 0; //bad name, this is a counter to skip lines

  size_t tid = 0;
  vec3D t_start = {0,0,0};
  vec3D t_end = {0,0,0};

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

%token <char> VAR
%token <char> G_CMD
%token <char> XYZ_CMD
%token <char> ABC_CMD
%token <char> CMD

%token <char*> LABEL
%token <char*> SPECIAL_CMD
%token <char*> CUSTOM_VAR

%token <char*> STRING
%token <int> INT
%token <float> FLOAT

%nterm <float> val
%nterm <float> fn
%nterm <float> arith_expr
%nterm <int> bool_expr
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
    if (get_var_val("line") <= 1) {
      printf("Warning: File %s is empty!\n",config.mpf_file);
    }
  }
;

lines:
  %empty
  | lines line NEWLINE
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
          printf("Jumped to line=%lu, byte=%ld\n\n",target_line, target_byte_offset);
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
          skip++; //ignore code lines, if condition is false
          if(debug) {printf("Skip=%d\n",skip);}
      }
  } if_body ENDIF {
      if (skip > 0) {
        skip--; //decr skip, to resume to normal code line execution
        //we are using skip as a counter, to allow nested false IF blocks
        //ToDo: Are nested false IF blocks possible?
        //Can't we just ignore the inner IF blocks, if the outer is false
        //-> I guess?
        if(debug) {printf("Skip=%d\n",skip);}
      }
    }
  | G_CMD arith_expr {if(!skip){
      //handle G codes
      switch((int)$2) {
        //Nullpunktverschiebung (NPV) =^ origin_offset
        case 54: origin_offset=(vec3D){0,0,139.4};break; //
        case 55: origin_offset=(vec3D){0,0,124.4};break; //
        case 56: origin_offset=(vec3D){0,10,0};break;
        case 57: origin_offset=(vec3D){0,0,10};break;
        case 58: origin_offset=(vec3D){10,15,0};break;
        case 59: origin_offset=(vec3D){0,10,20};break;
        //(Un-)Set incr_mode ->
        //assingments of coords happens absolute or relative
        case 90: incr_mode=0;break; //absolute
        case 91: incr_mode=1;break; //relative
        default: break;
      }
    }
  }
  | XYZ_CMD arith_expr {
    if(!skip) {
      if(rot_mode) {
        set_var_rot($1, $2);
      } else {
        set_var($1,$2);
        handle_tracks_def_by_coord_lines();
      }
    }
  }
  | ABC_CMD arith_expr {
    if(!skip) {
      set_var($1,$2);
      handle_tracks_def_by_coord_lines();
    }
  }
  | CMD arith_expr       {}
  | ROT                {rot_mode = 1;}
  | assignment
  | LABEL              {
                        if(!skip && !is_empty(&ret_stack)){

                          Elem top;
                          //Get stack top, without removing it from the stack
                          peek(&ret_stack, &top);
                          printf("$1_LABEL=%s, ret_stack_top.label=%s\n",$1,top.label);

                          //if the currently read LABEL="END_LABEL"
                          printf("strcmp($1,'END_LABEL')=%d\n",strcmp($1,"END_LABEL"));
                          if((strcmp($1,"END_LABEL") == 0)) {
                            //d->distance btw
                            //current runtime line and
                            //noted line of "<start_label>_END_LABEL" construct
                            int d = get_var_val("line")-get_var_val(top.label);
                            printf("d=%d\n...\n",d);

                            //lf -> label_finder
                            //tolerate off by one error, btw runtime- and lf-line
                            if(abs(d)<=1) {
                               //if the lines of "<start_label>_END_LABEL" construct,
                               //and the runtime line match, then we found the END_LABEL,
                               //to the currently active REPEAT block
                               printf("'END_LABEL' found! top.label=%s\n",top.label);

                               jump_requested = 1;
                               skip = 1;

                               target_line = top.line;
                               target_byte_offset = top.byte_offset;

                               printf("Jump Requested!\
                               \r\ntarget_line=%lu\ntarget_byte_offset=%ld\n",
                               target_line, target_byte_offset);

                               pop(&ret_stack, &top);
                            }
                          }
                          else if (strcmp($1,top.label) == 0) {
                            jump_requested = 1;
                            skip = 1;

                            target_line = top.line;
                            target_byte_offset = top.byte_offset;

                            printf("Jump Requested!\
                            \r\ntarget_line=%lu\ntarget_byte_offset=%ld\n",
                            target_line, target_byte_offset);

                            pop(&ret_stack, &top);
                          }
                          //else:
                          //goto not_found;

                          //end_label_found:


                          //not_found:
                        }
                       }
  | GOTO SEP MISC_ID   {if(!skip){
                          request_jump($3);}
                        }
  | REPEAT SEP MISC_ID %prec LOW_PREC
                        {if(!skip){
                            char start_end_l[32+11];
                            snprintf(start_end_l, sizeof(start_end_l), "%s_END_LABEL", $3);
                            handle_repeat($3,start_end_l,(size_t)get_var_val("line"));}
                        }
  | REPEAT SEP MISC_ID SEP MISC_ID
                        {if(!skip){
                            handle_repeat($3,$5,(size_t)get_var_val("line"));}
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

void handle_repeat(char* start_label, char* end_label, size_t linep) {
  char labelp[32]; //search towards +inf dir
  char labeln[32]; //search towards -inf dir

  size_t linen = linep;
  size_t line=-1;
  size_t byte_offset=-1;
  //ball search
  while (0<linen) {
    snprintf(labelp, sizeof(labelp), "REPEAT_%lu", linep);
    snprintf(labeln, sizeof(labeln), "REPEAT_%lu", linen);

    long byte_offsetp = (long)get_var_val(labelp);
    long byte_offsetn = (long)get_var_val(labeln);

    printf("labelp=%s, labeln=%s, linep=%lu, linen=%lu, line var_val=%lu\n",
      labelp,labeln,linep,linen,(size_t)get_var_val("line"));
    if (byte_offsetp <= 0 && byte_offsetn <= 0) {
      if (debug) {
        printf("Error: REPEAT return byte_offset=%ld\n",byte_offset);
      }
    }
    else {
      if (byte_offsetp > 0) {
        byte_offset=byte_offsetp;
        line=linep;
     }
     else if (byte_offsetn > 0) {
       byte_offset=byte_offsetn;
       line=linen;
     }
     break; //REPEAT found, we are finished
    }
    linen--;linep++;
  }
  printf("REPEAT found in line=%lu, byte_offset=%ld\n",line,byte_offset);

  push(&ret_stack,
    strdup(end_label), //return label
    line,       //return line
    byte_offset //return byte_offset
  );

  request_jump(start_label);

  if (debug > 0) {
    Elem temp;
    if (peek(&ret_stack, &temp)) {
        printf("Return: Label %s, Line: %zu, Offset: %ld)\n\n",
                temp.label, temp.line, temp.byte_offset);
    }
  }
}

void handle_tracks_def_by_coord_lines() {
  //handle (X,Y,Z) points rotated
  //in the experimental tracks_def_by_coord_lines mode
  if (!rot_mode && !config.tracks_def_by_laser && !(track_written>0))
  {
    t_end = net_point();
    write_track_line();
    t_start = net_point(); //TODO perhaps cleaner sol, than net_point weird global var inline function
    //prevent mult writes in line with mult coord cmds
    track_written = config.track_mid_len;
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
    //incr_mode only applies to (X,Y,Z) & (A,B,C), which we call coords
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

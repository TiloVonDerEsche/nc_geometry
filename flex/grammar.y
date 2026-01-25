%{
  #include <stdio.h>
  #include <math.h>

  #include "khashl_helper.h"

  int yylex (void);
  int yyerror(char* s);

  strfloat_t* h;
  khint_t k;
  int absent;

  FILE* hmhis;

  extern long byte_counter;
  extern int skip;
  char* pending_jump_label = NULL;
  int jump_requested = 0;

  void jump(char*);
  void set_var(char*, float);
  float get_var_val(char*);
  void init_hashmap();
  void init_hmhis();

%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token SEP NEWLINE SEMICOLON COMMENT OTHER
%token <char*> STRING
%token <int> INT
%token <float> FLOAT

%token EQ NEQ LTEQ GTEQ
%token IF ENDIF GOTO

%token <char*> VAR
%token <char*> CMD
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
                         print_hashmap(h, hmhis);
                         print_hashmap(h, stdout);

                         if (jump_requested) {
                              jump_requested = 0;
                              skip = 0; // Reset skip before jumping
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
          printf("Skip=%d\n",skip);
      }
  } if_body ENDIF {
      if (skip > 0) {
        skip--;
        printf("Skip=%d\n",skip);
      }
    }
  | CMD arith_expr       {if(!skip){ set_var($1,$2);}}
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
                          }
                          else if(strcmp($1,"LASER_OFF") == 0) {
                            set_var("laser",0);
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
  VAR '=' arith_expr          {if(!skip){set_var($1,$3);}}
  | CMD '=' arith_expr        {if(!skip){set_var($1,$3);}}
  | CUSTOM_VAR '=' arith_expr {if(!skip){set_var($1,$3);}}
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

        if(1 || debug){
          printf("Jumping to line=%d, offset=%d\n\n",
                (long)original_line,(long)offset);
        }

        fseek(yyin, (long)offset, SEEK_SET);
        yyrestart(yyin); //Tells Flex to flush buffers and read from yyin again
    }

    printf("fgetc=%c\n\n",fgetc(yyin));
    fseek(yyin, -1, SEEK_CUR);


}


//hashmap fns

void set_var(char* varname, float fnum) {
  k = strfloat_put(h, varname, &absent);
  if (absent) {
    kh_key(h, k) = strdup(varname);}
  kh_val(h, k) = fnum;
  //printf("Set %s to %f\n", varname, fnum);
}

float get_var_val(char* varname) {
  k = strfloat_get(h, varname);
  if ( kh_exist(h, k) ) {
    return kh_val(h, k);
  }

  return 0;
}


void init_hashmap() {
  khint_t k;
  int absent;

  h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 0;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");
  kh_val(h, k) = 0;

  //print_hashmap(h, stdout);
}

void init_hmhis() {
  printf("Opening: hmhis.json in write mode...\n");
  hmhis = fopen("./data/hmhis.json", "w");
  if (hmhis == NULL) {
      fprintf(stderr, "Error: Could not open hmhis.json (in write mode)!\n");
      return;
  }
  fprintf(hmhis,"[");

}

void close_hmhis() {
  //delete last redundant comma
  fseek(hmhis, -3, SEEK_CUR);
  fprintf(hmhis,"]");
  fclose(hmhis);
}

//bison fns

int yyerror(char* s)
{
	printf("Error: %s, in line: %d\n", s, (int)get_var_val("line"));
	return 0;
}

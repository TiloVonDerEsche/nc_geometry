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

  void jump(char*);
  void fast_forward_to_endif();
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
  seps                  {set_var("line",get_var_val("line")+1);}
  | seps exprs          {
                         set_var("line",get_var_val("line")+1);
                         print_hashmap(h, hmhis);
                         print_hashmap(h, stdout);
                        }
;

seps:
  %empty
  | seps SEP
;



exprs:
  expr
  | exprs SEP expr;
;

expr:
  IF SEP bool_expr {
      if (!$3) {
          fast_forward_to_endif();
      }
  } exprs ENDIF
  | CMD arith_expr       {set_var($1,$2);}
  | assignment
  | LABEL                {
                          set_var($1, (float)byte_counter);
                         }
  | GOTO SEP MISC_ID   {
                          jump($3);
                         }
  | SPECIAL_CMD          {
                          if(strcmp($1,"/LASER_ON") == 0) {
                            set_var("laser",1);
                          }
                          else if(strcmp($1,"LASER_ON") == 0) {
                            set_var("laser",1);
                          }
                          else if(strcmp($1,"/LASER_OFF") == 0) {
                            set_var("laser",0);
                          }
                          else if(strcmp($1,"LASER_OFF") == 0) {
                            set_var("laser",0);
                          }
                         }
  | CALL SEP STRING
  | MISC_ID
  | fn
  | COMMENT
  | error   {/*  | ',' SEP bool_expr {printf("bool_expr=%d",$3);}*/}

;

assignment:
  VAR '=' arith_expr     {set_var($1,$3);}
  | CMD '=' arith_expr   {set_var($1,$3);}
  | CUSTOM_VAR '=' arith_expr {set_var($1,$3);}
  | CUSTOM_VAR SEP arith_expr {set_var($1,$3);}
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
    float offset = get_var_val(label_name) + 5;

    //float offset=34;
    k = strfloat_get(h, label_name);
    if ( kh_exist(h, k) ) {
      printf("Found in hm: label_name=%s\n",label_name);
      //offset = kh_val(h, k);
    }
    else {
    printf("NOT found in hm: label_name=%s\n",label_name);
    }

    printf("Jump offset=%d\n\n",(long)offset);




    if (offset >= 0) {
        fseek(yyin, (long)offset, SEEK_SET);
        yyrestart(yyin); //Tells Flex to flush buffers and read from yyin again
    }
}

void fast_forward_to_endif() {
    int nesting = 1;
    while (nesting > 0) {
        int token = yylex();

        // If we hit End Of File, stop to prevent infinite loop
        if (token == 0) {
            fprintf(stderr, "Error: IF without matching ENDIF\n");
            break;
        }

        if (token == IF) {
            nesting++;
        } else if (token == ENDIF) {
            nesting--;
        }
        // All other tokens are simply consumed and ignored
    }
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
  kh_val(h, k) = 1;

  k = strfloat_put(h, "yyline", &absent);
  kh_key(h, k) = strdup("yyline");
  kh_val(h, k) = 1;

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
  printf("yyline=%d\n",yylineno);
  printf("yytext=%s\n",yytext);
	return 0;
}

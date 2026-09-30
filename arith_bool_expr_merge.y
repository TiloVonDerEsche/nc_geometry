arith_expr:
  val         {$$=$1;}
  | arith_expr '+' arith_expr {$$=$1+$3; /*printf("%f+%f=%f\n", $1,$3,$$);*/}
  | arith_expr '-' arith_expr {$$=$1-$3;}
  | arith_expr '*' arith_expr {$$=$1*$3;}
  | arith_expr '/' arith_expr {$$=$1/$3;}
  | '-' arith_expr %prec UNEG    {$$=-$2;}

  | arith_expr OR arith_expr      {$$=$1||$3;}
  | arith_expr AND arith_expr     {$$=$1&&$3;}
  | arith_expr EQ arith_expr      {$$=$1==$3;}
  | arith_expr NEQ arith_expr     {$$=$1!=$3;}
  | arith_expr LTEQ arith_expr    {$$=$1<=$3;}
  | arith_expr GTEQ arith_expr    {$$=$1>=$3;}
  | arith_expr '<' arith_expr     {$$=$1<$3;}
  | arith_expr '>' arith_expr     {$$=$1>$3;}
  | '!' arith_expr               {$$=!$2;}
  | '(' arith_expr ')'           {$$=$2;}
;

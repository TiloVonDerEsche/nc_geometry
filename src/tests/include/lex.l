%option noyywrap
/* the "incl" state is used for picking up the name
 * of an include file
 */
%x incl
%%
include             BEGIN(incl);

[a-z]+              ECHO;
[^a-z\n]*\n?        ECHO;

<incl>[ \t]*      /* eat the whitespace */
<incl>[^ \t\n]+   { /* got the include file name */
        yyin = fopen( yytext, "r" );

        if ( ! yyin )
            fprintf(stderr, "Failed to open %s!\n",yytext);

  yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

        BEGIN(INITIAL);
        }

<<EOF>> {
  yypop_buffer_state();

        if ( !YY_CURRENT_BUFFER )
            {
            yyterminate();
            }
        }

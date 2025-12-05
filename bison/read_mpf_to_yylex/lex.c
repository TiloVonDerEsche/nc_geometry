#include <ctype.h>
#include <stddef.h>

#include "helper.h"

extern strfloat_t* h;
khint_t kl;
extern int absent;

//////////////////////////////////////////////////
//------open files & read_mpf into program-----//
/////////////////////////////////////////////////
// printf("Opening: %s in read mode...\n",config->mpf_file);
// FILE* mpf = fopen(config->mpf_file, "r");
// if (mpf == NULL) {
//     fprintf(stderr, "Error: Could not open %s (in read mode)!\n", config->mpf_file);
//     return;
// }
//
// printf("Opening: hmhis.json in write mode...\n");
// FILE* hmhis = fopen("./data/hmhis.json", "w");
// if (hmhis == NULL) {
//     fprintf(stderr, "Error: Could not open hmhis.csv (in write mode)!\n");
//     return;
// }
// fprintf(hmhis,"["); //JSON literal object list paranthesis
//
// //read mpf into program str_arr, each entry is a line of the mpf
// char program[config->mpf_lines][config->max_line_len];
// char line_buf[config->max_line_len];
//
// size_t mi = 0;
// while ((fgets(line_buf, config->max_line_len, mpf) != NULL) && (mi < config->lines_to_read)) {
//   strcpy(program[mi],line_buf);
//   printf("Copied %s to program[%lu]\n",line_buf,mi);
//   mi++;
// }
// fclose(mpf);
//
// printf("Reading %ld lines from %s...\n",config->mpf_lines,config->mpf_file);
/////////////////////////////////////////////////

int
yylex (void)
{

  int c = getchar ();

  /* Ignore white space, get first nonwhite character. */
  while (c == ' ' || c == '\t')
    c = getchar ();

  if (c == EOF)
    return YYEOF;


  /* Char starts a number => parse the number. */
  if (c == '.' || isdigit (c))
    {
      ungetc (c, stdin);
      if (scanf ("%lf", &yylval.NUM) != 1)
        abort ();
      return NUM;
    }



  /* Char starts an identifier => read the name. */
  // Variable or Function name
  if (isalpha (c))
    {
      static ptrdiff_t bufsize = 0;
      static char *symbuf = 0;

      ptrdiff_t i = 0;
      do
        {
          /* If buffer is full, make it bigger. */
          if (bufsize <= i)
            {
              bufsize = 2 * bufsize + 40;
              symbuf = (char*) realloc (symbuf, (size_t) bufsize);
            }
          /* Add this character to the buffer. */
          symbuf[i++] = (char) c;
          /* Get another character. */
          c = getchar ();
        }
      while (isalnum (c) || c == '_');

      ungetc (c, stdin);
      symbuf[i] = '\0';

      // kl = strfloat_put(h, symbuf, &absent);
      // if (absent) {
      //   kh_key(h, kl) = strdup(symbuf);
      // }

      // symrec *s = getsym (symbuf);
      // if (!s)
      //   s = putsym (symbuf, VAR);
      yylval.VAR = strdup(symbuf); /* or yylval.FUN = s. */
      return VAR;
    }

  /* Any other character is a token by itself. */
  return c;
}

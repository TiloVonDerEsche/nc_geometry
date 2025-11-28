#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


#include "mfcalc.tab.c"
#include "lex.c"
// #include "khashl.h"
// #include "khashl_helper.h"

struct init
{
  char const *name;
  func_t *fun;
};


struct init const funs[] =
{
  { "atan", atan },
  { "cos",  cos  },
  { "exp",  exp  },
  { "ln",   log  },
  { "sin",  sin  },
  { "sqrt", sqrt },
  { 0, 0 },
};


/* The symbol table: a chain of 'struct symrec'. */
symrec *sym_table;


/* Put functions in table. */
static void
init_table (void)

{
  for (int i = 0; funs[i].name; i++)
    {
      symrec *ptr = putsym (funs[i].name, FUN);
      ptr->value.fun = funs[i].fun;
    }
}


symrec *
putsym (char const *name, int sym_type)
{
  symrec *res = (symrec *) malloc (sizeof (symrec));
  res->name = strdup (name);
  res->type = sym_type;
  res->value.var = 0; /* Set value to 0 even if fun. */
  res->next = sym_table;
  sym_table = res;
  return res;
}


symrec *
getsym (char const *name)
{
  for (symrec *p = sym_table; p; p = p->next)
    if (strcmp (p->name, name) == 0)
      return p;
  return NULL;
}



void
yyerror (char const *s)
{
  fprintf (stderr, "err: %s\n", s);
}


strfloat_t* h;
khint_t k;
int absent;

int main (int argc, char const* argv[])
{
  /* Enable parse traces on option -p. */
  if (argc == 2 && strcmp(argv[1], "-p") == 0)
    yydebug = 1;


  //yylloc.first_line = yylloc.last_line = 1;
  //yylloc.first_column = yylloc.last_column = 0;

  init_table ();

  h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 123;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");

  kh_val(h, k) = -1.2;
  k = strfloat_put(h, "laser_power", &absent);
  kh_key(h, k) = strdup("laser_power");
  kh_val(h, k) = 3500;

  print_hashmap(h, stdout);

  return yyparse ();
}

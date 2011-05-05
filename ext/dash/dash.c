#include "ruby.h"
#include "main.h"

VALUE rb_dash_main(int argc, VALUE * argv, VALUE self)
{
  int j;
  int result;
  char * * c_argv = ALLOCA_N(char *, argc);

  for(j = 0; j < argc; ++j)
  {
    c_argv[j] = StringValueCStr(argv[j]);
  }

  result = dash_main(argc, c_argv);

  return INT2NUM(result);
}

void Init_dash()
{
  rb_define_global_function("dash_main", rb_dash_main, -1);
}


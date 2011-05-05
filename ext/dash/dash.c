#include "ruby.h"
#include "main.h"

VALUE rb_dash_main(int argc, VALUE * argv, VALUE self)
{
  int j;
  int result;
  char * * c_argv = ALLOCA_N(char *, argc+1);
  printf("%p\n", c_argv);

  for(j = 0; j < argc; ++j)
  {
    c_argv[j] = StringValueCStr(argv[j]);
    printf("%d %s\n", j, c_argv[j]);
  }

  c_argv[argc] = 0;

  printf("calling dash_main - %p\n", c_argv);
  result = dash_main(argc, c_argv);

  return INT2NUM(result);
}

void Init_dash()
{
  rb_define_global_function("dash_main", rb_dash_main, -1);
}


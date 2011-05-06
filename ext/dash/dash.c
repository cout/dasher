#include "ruby.h"
#include "main.h"
#include "jobs.h"
#include "var.h"
#include "exec.h"
#include "eval.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

VALUE rb_mDash;
VALUE rb_cBuiltins;

VALUE
rb_dash_find_dot_file(VALUE self, VALUE basename)
{
	char *fullname;
	const char *path = pathval();
	struct stat statb;

	/* don't try this for absolute or relative paths */
	if (strchr(StringValueCStr(basename), '/'))
		return basename;

	while ((fullname = padvance(&path, StringValueCStr(basename))) != NULL) {
		if ((stat(fullname, &statb) == 0) && S_ISREG(statb.st_mode)) {
      VALUE result = rb_str_new2(fullname);
      stunalloc(fullname);
      return result;
		}
		stunalloc(fullname);
	}

  rb_raise(rb_eRuntimeError, "%s: not found", basename);
	/* NOTREACHED */
}

VALUE
rb_dash_setinputfile(VALUE self, VALUE fname, VALUE flags)
{
  int result = setinputfile(
      StringValueCStr(fname),
      NUM2INT(flags));
  return INT2NUM(result);
}

static VALUE rb_commandname;

VALUE
rb_dash_setcommandname(VALUE self, VALUE cname)
{
  rb_commandname = cname;
  commandname = StringValueCStr(rb_commandname);
  return rb_commandname;
}

VALUE
rb_dash_cmdloop(VALUE self, VALUE top)
{
  int result = cmdloop(NUM2INT(top));
  return INT2NUM(result);
}

VALUE
rb_dash_popfile(VALUE self)
{
  popfile();
  return Qnil;
}

VALUE
rb_dash_exitstatus(VALUE self)
{
  return INT2NUM(exitstatus);
}

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

VALUE rb_dash_stoppedjobs(VALUE self)
{
  return INT2NUM(stoppedjobs());
}

void Init_dash()
{
  rb_mDash = rb_define_module("Dash");
  rb_cBuiltins = rb_define_module_under(rb_mDash, "Builtins");

  rb_define_module_function(rb_mDash, "main", rb_dash_main, -1);
  rb_define_module_function(rb_mDash, "stoppedjobs", rb_dash_stoppedjobs, 0);
  rb_define_module_function(rb_mDash, "find_dot_file", rb_dash_find_dot_file, 1);
  rb_define_module_function(rb_mDash, "setinputfile", rb_dash_setinputfile, 2);
  rb_define_module_function(rb_mDash, "setcommandname", rb_dash_setcommandname, 1);
  rb_define_module_function(rb_mDash, "cmdloop", rb_dash_cmdloop, 1);
  rb_define_module_function(rb_mDash, "popfile", rb_dash_popfile, 0);
  rb_define_module_function(rb_mDash, "exitstatus", rb_dash_exitstatus, 0);
}


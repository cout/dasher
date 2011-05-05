require 'mkmf'
require 'set'

$defs << '-D_GNU_SOURCE'

$dash_srcs = Set.new(Dir['*.c'])
$dash_srcs << 'syntax.c'
$dash_srcs << 'nodes.c'
$dash_srcs << 'builtins.c'
$dash_srcs << 'signames.c'
$dash_srcs.delete('init.c')

$srcs = $dash_srcs.dup.to_a
$srcs << 'init.c'

$objs = $srcs.map { |f| f.sub(/\.c$/, '.o') }

have_header('alloca.h')
have_header('histedit.h')

have_func('getpwnam', 'pwd.h')
have_func('glob', 'glob.h')
have_func('fnmatch', 'fnmatch.h')
have_func('isalpha', 'ctype.h')
have_func('mempcpy', 'string.h')
have_func('stpcpy', 'string.h')
have_func('strchrnul', 'string.h')
have_func('strsignal', 'string.h')
have_func('bsearch', 'stdlib.h')
have_func('sysconf', 'unistd.h')
have_func('strtod', 'stdlib.h')
have_func('strtoimax', 'inttypes.h')
have_func('strtoumax', 'inttypes.h')
have_func('killpg', 'signal.h')
have_func('sigsetmask', 'signal.h')
have_func('getrlimit', 'sys/resource.h')
have_func('imaxdiv', 'stdlib.h')

have_library('edit')

# TODO: __alias__ - HAVE_ATTRIBUTE_ALIAS / HAVE_ALIAS_ATTRIBUTE

create_makefile('dash')

File.open('Makefile', 'a') do |makefile|

makefile.write <<END

builtins.c builtins.h: helpers/mkbuiltins builtins.def
\tsh ./helpers/mkbuiltins builtins.def

builtins.def: builtins.def.in
\t$(CC) -E -x c -o $@ $<

syntax.c syntax.h: helpers/mksyntax
\t./helpers/mksyntax

helpers/mksyntax: helpers/mksyntax.c parser.h
\t$(CC) $(CFLAGS) -I. helpers/mksyntax.c -o helpers/mksyntax

nodes.c nodes.h: helpers/mknodes nodetypes nodes.c.pat
\t./helpers/mknodes nodetypes nodes.c.pat

helpers/mknodes: helpers/mknodes.c parser.h
\t$(CC) $(CFLAGS) -I. helpers/mknodes.c -o helpers/mknodes

token.h: helpers/mktokens
\tsh helpers/mktokens

signames.c: helpers/mksignames
\t./helpers/mksignames

init.c: helpers/mkinit
\t./helpers/mkinit #{$dash_srcs.to_a.join(' ')}

$(OBJS): syntax.h nodes.h builtins.h token.h

END

end

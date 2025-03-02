import os

env = Environment(CC = 'gcc',
                  CPPPATH=['./inc', './build'],
                  CCFLAGS='-g -W -Wall -Wextra -Wundef -Wshadow -Wconversion')

flex_source = 'src/jacc_lexer.l'
bison_source = 'src/jacc_parser.y'

bison_target = 'build/jacc_parser.tab.c'
bison_header = 'build/jacc_parser.tab.h'

flex_target = 'build/jacc_lexer.yy.c'

env.Command([bison_target, bison_header], bison_source, 'bison -d -o $TARGET $SOURCE')

env.Command(flex_target, flex_source, 'flex -o $TARGET $SOURCE')

c_sources = [
    'src/ast.c',
    'src/lex.c',
    bison_target,
    flex_target
]

output = 'jacc'

env.Program(output, c_sources)

Clean(output, [flex_target, bison_target, bison_header])

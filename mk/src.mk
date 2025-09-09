MK += Makefile $(wildcard     mk/*.mk)
CM += CMake*   $(wildcard  cmake/*.cmake)

C  += $(wildcard src/*.c*)
H  += $(wildcard inc/*.h*)
CP += tmp/$(APP).yacc.cpp tmp/$(APP).lex.cpp
HP += tmp/$(APP).yacc.hpp

S  += $(wildcard lib/*.ini) $(wildcard lib/*.f)

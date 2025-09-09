bin/$(BINFILE): $(C) $(H) $(CP) $(HP)
	$(CXX) $(CFLAGS) -o $@ $(C) $(CP) $(L)
tmp/%.yacc.cpp: src/%.yacc
	bison -o $@ $<
tmp/%.lex.cpp: src/%.lex
	flex -o $@ $<

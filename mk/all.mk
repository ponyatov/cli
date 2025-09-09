.PHONY: all run
all: bin/$(BINFILE) $(S)
run: bin/$(BINFILE) $(S)
	$^

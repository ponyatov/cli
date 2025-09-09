.PHONY: ai tmp/$(APP).ai.md
ai: tmp/$(APP).ai.md
tmp/$(APP).ai.md:
	cat README.md doc/*.md lib/*.ini lib/*.f $(C) $(H) $(S) > $@ ; touch $@

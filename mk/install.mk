.PHONY : install update ref gz
install: $(WS)_install doc gz ref
	$(MAKE) update
update : $(WS)_update
	$(RUSTUP) self update && $(RUSTUP) update
	$(PIP) install -U    pip
	$(PIP) install -U -r requirements.txt
	$(NPM) update
ref:    $(REF)
gz :    $(GZ)

Debian_install:
# sudo dpkg --add-architecture i386
Debian_update:
	sudo apt update
	sudo apt install -uy `cat apt.$(WS)` $(APT)
	$(PIP) install -U    pip
	$(PIP) install -U -r requirements.txt

Msys_install: doc ref gz
	pacman -Suy
Msys_update:
	pacman -S $(shell cat apt.$(WS) | tr '\n' ' ') $(MSYS)

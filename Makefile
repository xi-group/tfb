MAKE=make
DEBUILD=debuild
TAR=tar
DATE=$(shell date +%Y-%m-%d)

.PHONY: tests

all:
	$(MAKE) -C src
	$(MAKE) -C doc

deb:
	$(DEBUILD) -us -uc -b -i

rpm:
	@echo "NOT IMPLEMENTED!"

static:
	@echo "NOT IMPLEMENTED!"

tests: all
	@echo "NOT IMPLEMENTED!"

clean:
	$(MAKE) -C src clean
	$(MAKE) -C doc clean
	@rm -rf *.tar.gz
	@rm -rf debian/.debhelper debian/files debian/tfb
	@rm -rf debian/tfb.debhelper.log debian/tfb.substvars

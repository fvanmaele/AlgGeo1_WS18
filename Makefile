.PHONY: scripts clean
all: scripts AlgGeo1.flt

scripts:
	$(MAKE) -C scripts all

AlgGeo1.flt:
	scripts/flatex AlgGeo1.tex

clean:
	$(MAKE) -C scripts clean
	@rm -v AlgGeo1.flt

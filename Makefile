.PHONY: scripts
scripts:
	$(MAKE) -C scripts all

AlgGeo1.flt:
	scripts/flatex AlgGeo1.tex

tags: AlgGeo1.flt
	scripts/create-tags AlgGeo1.flt >tags

plastex: tags
	plastex --renderer=Gerby --tags=tags --input-encoding=latin1 \
		--output-encoding=latin1 AlgGeo1.tex

.PHONY: clean
clean:
	$(MAKE) -C scripts clean
	@rm -v AlgGeo1.flt

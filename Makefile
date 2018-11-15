.PHONY: scripts
scripts:
	$(MAKE) -C scripts all

AlgGeo1.flt:
	scripts/flatex AlgGeo1.tex

tags: AlgGeo1.flt
	scripts/create-tags AlgGeo1.flt >tags

plastex: tags
	plastex --renderer=Gerby --tags=tags AlgGeo1.tex

AlgGeo1.pdf: AlgGeo1.tex Part1/*.tex Part2/*.tex
	pdflatex AlgGeo1.tex
	pdflatex AlgGeo1.tex

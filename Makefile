latex = AlgGeo1.tex Chapter1/*.tex Chapter2/*.tex

.PHONY: scripts
scripts:
	$(MAKE) -C scripts all

AlgGeo1.flt:
	latexpand --fatal AlgGeo1.tex --output AlgGeo1.flt

AlgGeo1.pdf: $(latex)
	pdflatex AlgGeo1.tex
	pdflatex AlgGeo1.tex

tags: $(latex)
	scripts/create-tags AlgGeo1.flt >tags

plastex: tags
	plastex --renderer=Gerby --tags=tags AlgGeo1.tex

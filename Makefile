latex = AlgGeo1.tex Chapter*/*.tex

.PHONY: scripts
scripts:
	$(MAKE) -C scripts all

AlgGeo1.flt: $(latex)
	latexpand --makeatletter --fatal AlgGeo1.tex --output AlgGeo1.flt

AlgGeo1.pdf: $(latex)
	pdflatex AlgGeo1.tex
	pdflatex AlgGeo1.tex

tags: AlgGeo1.flt
	scripts/create-tags AlgGeo1.flt >tags

plastex: tags
	plastex --renderer=Gerby --tags=tags AlgGeo1.tex

all: informe.pdf

informe.pdf: informe.tex
	pdflatex informe.tex
	rm -f *.aux *.dvi *.log

clean:
	rm -f *~ *.aux *.dvi *.log

mrproper: clean
	rm -f informe.pdf
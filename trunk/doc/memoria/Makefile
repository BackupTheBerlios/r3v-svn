all: memoria-tot.pdf

memoria-tot.pdf: page1.pdf empty.pdf page3.pdf memoria.pdf
	gs -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=/tmp/memoria-tot.pdf page1.pdf empty.pdf empty.pdf empty.pdf page3.pdf empty.pdf memoria.pdf; mv /tmp/memoria-tot.pdf .

page1.pdf: page1.tex
	pdflatex page1.tex
	rm -f *.aux *.dvi *.log

memoria.pdf: memoria.tex
	pdflatex memoria.tex
	pdflatex memoria.tex
	rm -f *.aux *.dvi *.log *.idx *.toc *.lof

clean:
	rm -f *~ *.aux *.dvi *.log page1.pdf memoria.pdf

mrproper: clean
	rm -f memoria-tot.pdf

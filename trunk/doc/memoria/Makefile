all: memoria-tot.pdf

memoria-tot.pdf: page1.pdf empty.pdf page3.pdf memoria.pdf page5.pdf
	pdftk page1.pdf empty.pdf empty.pdf empty.pdf page3.pdf empty.pdf page5.pdf empty.pdf memoria.pdf cat output memoria-tot.pdf

page1.pdf: page1.tex
	pdflatex page1.tex
	rm -f *.aux *.dvi *.log

page5.pdf:  page5.tex
	pdflatex page5.tex
	rm -f *.aux *.dvi *.log

memoria.pdf: memoria.tex images/*.png
	pdflatex memoria.tex
	pdflatex memoria.tex
	rm -f *.aux *.dvi *.log *.idx *.toc *.lof

clean:
	rm -f *~ *.aux *.dvi *.log *.idx *.toc *.lof page1.pdf memoria.pdf

mrproper: clean
	rm -f memoria-tot.pdf

TEX = xelatex
PRE =  $(TEX)
MOVE = mv

.PHONY: all view

TARGET = WorkshopIntroRevEng.pdf

all: main.pdf

view:
	open main.pdf

main.pdf: main.tex
	# Build twice on first time so blueslope graphic is not missing
	if [ ! -f main.aux ]; then $(TEX) main.tex; fi;
	$(TEX) main.tex
	$(MOVE) main.pdf $(TARGET)

clean:
	$(RM) main.out main.nav main.log main.aux missfont.log main.snm main.toc $(TARGET)


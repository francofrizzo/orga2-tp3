filename=informe

all: pdf see

pdf:
	latexmk -pdf -quiet ${filename}.tex

clean:
	latexmk -c

see:
	evince ${filename}.pdf &

TARGET = MeshGlide

all:
	gcc payload.c -o payload
	strip payload
	python encoder.py
	gcc packer.c -o NE-PAS-EXECUTER
	strip NE-PAS-EXECUTER
	cat payload2 >> NE-PAS-EXECUTER

.PHONY: clean
clean:
	rm payload
	rm payload2
	rm NE-PAS-EXECUTER

.POSIX:
PREFIX = /usr/local

.SUFFIXES:
all:
	cc -lsndio src/*.c -o matrix-1000
install:
	mkdir -p $(PREFIX)/bin
	cp matrix-1000 $(PREFIX)/bin
uninstall:
	rm $(PREFIX)/bin/matrix-1000

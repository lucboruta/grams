CC=gcc
CFLAGS=-O3 -m64 -ansi -pedantic-errors \
	-Wall -Wextra -Wcast-align -Wcast-qual -Wfloat-equal -Wformat=2 \
	-Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wmissing-prototypes \
	-Wno-missing-braces -Wno-unused-parameter -Wold-style-cast -Wold-style-definition \
	-Woverloaded-virtual -Wpointer-arith -Wstrict-aliasing -Wstrict-overflow=5 \
	-Wstrict-prototypes -Wuninitialized -Werror

HEADERS = grams.h
OBJECTS = grams.o

default: grams

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

grams: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f grams

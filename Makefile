CC=gcc
CFLAGS=-I. -O3
DEPS=rc4ok.h
OBJ=rc4ok.o demo_rc4ok.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

demo_rc4ok: $(OBJ)
	$(CC) -o demo_rc4ok $(OBJ) -lpthread

.PHONY: clean

clean:
	rm -f $(OBJ) *~ core


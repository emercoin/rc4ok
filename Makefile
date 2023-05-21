CC=gcc
CFLAGS=-I.
DEPS=rc4ok.h
OBJ=rc4ok.o demo_rc4ok.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

demo_rc4ok: $(OBJ)
	$(CC) -o demo_rc4ok $(OBJ)

.PHONY: clean

clean:
	rm -f $(OBJ) *~ core


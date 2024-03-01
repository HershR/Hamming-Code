CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS= -lm

.PHONY: all clean

all: encode decode entropy error

encode: encode.o bm.o bv.o hamming.o
	$(CC) -o encode $^ $(LFLAGS)

decode: decode.o bm.o bv.o hamming.o
	$(CC) -o decode $^ $(LFLAGS)

entropy: entropy.o
	$(CC) -o entropy $^ $(LFLAGS)

error: error.o
	$(CC) -o error $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode  entropy error *.o

format:
	clang-format -i -style=file *.h *.c

scan-build: clean
	scan-build make


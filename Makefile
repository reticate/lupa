CC = gcc
CFLAGS = -O2 -Wall -Wextra -fno-stack-protector -nostdlib -fPIC -Iinclude
LDFLAGS = -T linker.ld

SRC = src/shim.c src/util.c
OBJ = $(SRC:.c=.o)
OUT = shim.ko

.PHONY: all clean

all: $(OUT)

$(OUT): $(OBJ)
	ld $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)

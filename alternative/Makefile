# Makefile for reverse-c (alternative version)
# Richard Cavell (c) 2017-2019

CFLAGS += -std=c89 -pedantic
CFLAGS += -Wall -Werror -Wextra

.DEFAULT: all
.PHONY:   all

all: reverse
reverse: reverse.c reverse.h
	$(CC) $(CFLAGS) $< $(OUTPUT_OPTION)

.PHONY: clean

clean:
	$(RM) reverse

# Makefile for reverse-c
# Richard Cavell (c) 2017-2018

CFLAGS += -std=c89 -pedantic
CFLAGS += -Wall -Werror -Wextra

all: reverse

reverse: reverse.c

clean:
	$(RM) reverse

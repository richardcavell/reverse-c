# Makefile for reverse-c
# Richard Cavell (c) 2017-2018

CC     += -std=c89 -pedantic
CFLAGS += -Wall -Werror -Wextra

reverse: reverse.c
	$(CC) $(CFLAGS) $^ $(OUTPUT_OPTION)

clean:
	$(RM) reverse

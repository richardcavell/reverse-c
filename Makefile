CC     += -std=c89 -pedantic
CFLAGS += -Wall -Werror -Wextra

reverse: reverse.c
	$(CC) $(CFLAGS) $^ $(OUTPUT_OPTION)

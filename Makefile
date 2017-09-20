FILE_LIST := main.c ls.c file.c

CFLAGS := -Werror -Wall -std=c99

.PHONY: all
all:
	$(CC) $(CFLAGS) $(FILE_LIST) -o fcomment

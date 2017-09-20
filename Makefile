FILE_LIST := main.c ls.c file.c

.PHONY: all
all:
	$(CC) $(FILE_LIST) -o fcomment

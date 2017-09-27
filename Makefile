FILE_LIST := main.c cmd_ls.c cmd_set.c cmd_cp.c file.c terminal.c mfile.c
FILE_LIST += markdown.c fcomment.c

SRCS := $(addprefix $(SOURCE_PATH)/,$(FILE_LIST))

CFLAGS := -Werror -Wall -std=gnu99

.PHONY: all
all:
	$(CC) $(CFLAGS) $(SRCS) -o fcomment
	./fcomment set -v ./fcomment "CLI program to 1) attach comment to a file and 2) view attached comment"

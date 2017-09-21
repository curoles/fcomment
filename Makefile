FILE_LIST := main.c cmd_ls.c cmd_set.c file.c

SRCS := $(addprefix $(SOURCE_PATH)/,$(FILE_LIST))

CFLAGS := -Werror -Wall -std=gnu99

.PHONY: all
all:
	$(CC) $(CFLAGS) $(SRCS) -o fcomment
	./fcomment set

FILE_LIST := main.c ls.c file.c

SRCS := $(addprefix $(SOURCE_PATH)/,$(FILE_LIST))

CFLAGS := -Werror -Wall -std=c99

.PHONY: all
all:
	$(CC) $(CFLAGS) $(SRCS) -o fcomment

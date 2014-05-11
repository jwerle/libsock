
CFLAGS += -std=c99 -Wall -L../ -lsock -I../include

TCP ?= $(wildcard tcp/*.c)

tcp: $(TCP)

$(TCP):
	$(CC) $(@) $(CFLAGS) -o $(@:.c=)

.PHONY: tcp $(TCP)

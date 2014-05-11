
CFLAGS += -std=c99 -Wall -L../ -lsock -I../include

TCP ?= $(wildcard tcp/*.c)

tcp: $(TCP)

$(TCP):
	$(CC) $(@) $(CFLAGS) -o $(@:.c=)

clean:
	rm -f $(TCP:.c=)

.PHONY: tcp $(TCP)

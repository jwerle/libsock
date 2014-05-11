
CFLAGS += -std=c99 -Wall -I../include

TCP ?= $(wildcard tcp/*.c)

tcp: $(TCP)

$(TCP):
	$(CC) $(@) ../libsock.a $(CFLAGS) -o $(@:.c=)

clean:
	rm -f $(TCP:.c=)

.PHONY: tcp $(TCP)

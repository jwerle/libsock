
CFLAGS += -std=c99 -Wall

TCP ?= $(wildcard tcp/*.c)

tcp: $(TCP)

$(TCP):
	$(CC) $(@) -lsock $(CFLAGS) -o $(@:.c=)

clean:
	rm -f $(TCP:.c=)

.PHONY: tcp $(TCP)

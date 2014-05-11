
CC ?= cc
VALGRIND ?= valgrind
OS = $(shell uname)

SRC = $(filter-out src/main.c, $(wildcard src/*.c))
OBJS = $(SRC:.c=.o)
TESTS = $(wildcard tests/*.c)

PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man

MAN_FILES = $(wildcard man/*.md)

BIN ?= sock
LIB_NAME ?= sock

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 1

TARGET_NAME = lib$(LIB_NAME)
TARGET_STATIC = $(TARGET_NAME).a
TARGET_DSOLIB = $(TARGET_NAME).so.$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
TARGET_DSO = $(TARGET_NAME).so

CFLAGS += -std=c99 -Wall -O2 -fvisibility=hidden -fPIC -pedantic
CFLAGS += -DSOCK_VERSION='"$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"'
CFLAGS += -Ideps -Iinclude

LDFLAGS += -o $(TARGET_DSOLIB) -shared -soname $(TARGET_DSO).$(VERSION_MAJOR)

ifeq ($(OS), Darwin)
	LDFLAGS += -lc -Wl,-install_name,$(TARGET_DSO)
endif

$(BIN): $(TARGET_STATIC) $(TARGET_DSO)
	@#$(CC) $(CFLAGS) $(SRC) src/main.c -o $(BIN)

install: $(BIN)
	mkdir -p $(PREFIX)/include/$(LIB_NAME)
	cp include/sock/*.h $(PREFIX)/include/sock
	install include/sock.h $(PREFIX)/include
	install $(TARGET_DSO) $(PREFIX)/lib
	@#install $(BIN) $(PREFIX)/bin
	install man/*.1 $(MANPREFIX)/man1
	@#install man/*.3 $(MANPREFIX)/man3

$(TARGET_STATIC): $(OBJS)
	ar crus $(TARGET_STATIC) $(OBJS)

$(TARGET_DSO): $(OBJS)
ifeq ("Darwin","$(OS)")
	$(CC) -shared $(OBJS) $(OSX_LDFLAGS) -o $(TARGET_DSOLIB)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR)
else
	$(CC) -shared $(OBJS) -o $(TARGET_DSOLIB)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO)
	ln -s $(TARGET_DSOLIB) $(TARGET_DSO).$(VERSION_MAJOR)
	strip --strip-unneeded $(TARGET_DSO)
endif

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

test: $(OBJS) $(TARGET_STATIC) deps $(TESTS)

$(TESTS):
	@cc $(@) deps/ok/libok.a $(TARGET_STATIC) $(CFLAGS) -o $(@:.c=)
	@./$(@:.c=)

tests/tcp.c:
	cc $(@) deps/ok/libok.a $(TARGET_STATIC) $(CFLAGS) -o $(@:.c=);
	./tests/tcp-client.sh > /dev/null 2>&1 &
	./tests/tcp-reply.sh > /dev/null 2>&1 &
	./$(@:.c=);

deps:
	make -C deps/ok

clean:
	make clean -C deps/ok
	rm -f $(OBJS)
	rm -f $(BIN)
	rm -f $(TARGET_STATIC)
	rm -f $(TARGET_DSO)
	rm -f $(TARGET_DSOLIB)
	rm -f *.so*
	$(foreach test, $(TESTS), $(shell rm -f $(test:.c=)))

docs: $(MAN_FILES)

$(MAN_FILES):
	curl -# -F page=@$(@) -o $(@:%.md=%) http://mantastic.herokuapp.com

.PHONY: deps $(MAN_FILES) $(TESTS)

.POSIX:
.SUFFIXES:
.SUFFIXES: .c

DEFINES ?= -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
CFLAGS  ?=
LDFLAGS ?=
LDLIBS  ?=

BIN = \
	externalServer \
	localServer \
	localClient

.c:
	$(CC) $(DEFINES) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

all: $(BIN)

clean:
	rm -f $(BIN)
.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

BIN = getpgrp setpgrp
OBJ = $(@:=.o)
MAN = $(@:=.1)
HDR = arg.h


all: $(BIN)
$(OBJ): $(@:.o=.c) $(HDR)

getpgrp: getpgrp.o
	$(CC) -o $@ $@.o $(LDFLAGS)

setpgrp: setpgrp.o
	$(CC) -o $@ $@.o $(LDFLAGS)

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

install: $(BIN)
	mkdir -p -- "$(DESTDIR)$(PREFIX)/bin"
	mkdir -p -- "$(DESTDIR)$(MANPREFIX)/man1"
	cp -- $(BIN) "$(DESTDIR)$(PREFIX)/bin/"
	cp -- $(MAN) "$(DESTDIR)$(MANPREFIX)/man1/"

uninstall:
	-cd -- "$(DESTDIR)$(PREFIX)/bin/" && rm -f -- $(BIN)
	-cd -- "$(DESTDIR)$(MANPREFIX)/man1/" && rm -f -- $(MAN)

clean:
	-rm -rf -- $(BIN) *.o *.su

.SUFFIXES:
.SUFFIXES: .o .c

.PHONY: all install uninstall clean

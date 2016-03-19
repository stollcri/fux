#
# fux -- Automatically fix mistakes in previously typed commands
#  Copyright (c) 2016 Christopher Stoll (https://github.com/stollcri)
#

CC = cc

CFLAGS = -O3 -Wall -D NDEBUG -Wall
LFLAGS = -l ncurses
AOFILE = ./bin/fux
CFILES = fux.c dp.c

CFLAGS_DBG = -O0 -g -Wall
LFLAGS_DBG = -l ncurses
AOFILE_DBG = ./bin/fux
CFILES_DBG = $(CFILES)

# make sure scan-build is available
SCAN_BUILD_EXISTS := @true
SCAN_BUILD_WHICH := $(shell which scan-build)
ifeq ($(strip $(SCAN_BUILD_WHICH)),)
	SCAN_BUILD_EXISTS := @echo "\nERROR:\n scan-build not found.\n See: http://clang-analyzer.llvm.org/installation\n" && exit 1
endif

.PHONY: default fux test debug run analysis findtrouble run-old clean install

default: fux
test: debug run

debug:
	@mkdir -p ./bin/
	$(CC) $(CFLAGS_DBG) $(LFLAGS_DBG) -o $(AOFILE_DBG) $(CFILES_DBG)

analysis:
	echo "Variables should be checked for null after *alloc ... check the following ..."
	grep -inlR -e alloc\( . | xargs grep -inR -A1 -e alloc\(
	echo
	echo "Variables should be nulled after free ... check the following output ..."
	grep -inlR -e free\( . | xargs grep -inR -A1 -e free\(
	echo
	$(SCAN_BUILD_EXISTS)
	scan-build -o tst/out -v -V make debug

findtrouble:
	grep -inR -e alloc\( .
	grep -inR -e strcpy\( -e strcat\( -e scanf\( -e gets\( .

run:
	time echo "apt-get r\c" | bin/fux
	# time echo "brew indtall vim\c" | bin/fux
	# time echo "aptget insta remove vim\c" | bin/fux

clean:
	rm -r bin/*
	rm -r tst/out/20*

fux:
	@mkdir -p ./bin/
	$(CC) $(CFLAGS) $(LFLAGS) -o $(AOFILE) $(CFILES)

install:
	cp bin/fux /usr/local/bin/fux
	chown root:wheel /usr/local/bin/fux
	mkdir -p /usr/local/man/
	cp doc/fux /usr/local/man/fux.1
	chown root:wheel /usr/local/man/fux.1

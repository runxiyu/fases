#
#	Copyright (C) 2022 Ferass EL HAFIDI
#	Copyright (C) 2022 Leah Rowe
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

.POSIX:

# Commands

all: config clean prepbox genbox box
include ./config.mk

genbox:
	cat "box-templates/box_1-23.c"                                    > box.c
	for u in ${CORE}; do echo "int $${u%.c}_main(int, char**);" | sed "s/\[_/test_/g"; done>> box.c
	test ${INCLUDE_EXTRA} == n || for u in ${EXTRA}; do echo "int $${u%.c}_main(int, char**);"; done>> box.c
	cat "box-templates/box_45-49.c"                                  >> box.c
	for u in ${CORE}; do echo "	else if(!strcmp(basename(argv[0]), \"$${u%.c}\")) return $${u%.c}_main(argc, argv);" | sed "s/\[_/test_/g"; done >> box.c
	test ${INCLUDE_EXTRA} == n || for u in ${EXTRA}; do echo "	else if(!strcmp(argv[0], \"$${u%.c}\")) return $${u%.c}_main(argc, argv);"; done >> box.c
	cat "box-templates/box_70-73.c"                                  >> box.c
	for u in ${CORE}; do echo "		printf(\"$${u%.c} \");"; done    >> box.c
	test ${INCLUDE_EXTRA} == n || for u in ${EXTRA}; do echo "		printf(\"$${u%.c} \");"; done    >> box.c
	cat "box-templates/box_94-96.c"                                  >> box.c
	echo "/* Generated on $$(date) */"                               >> box.c

prepbox:
	mkdir -p box_tmp
	for f in ${CORE}; do sed "s/^int main(/int $$(echo "$$f")_main(/" < "core/"$$f".c" | sed "s/\"common/\"core\/common/g" | sed "s/\[_/test_/g" > "box_tmp/"$$f"_box.c"; done
	rm -f "box_tmp/[_box.c"
	test ${INCLUDE_EXTRA} == n || for f in ${EXTRA}; do sed "s/^int main(/int $$(echo "$$f")_main(/" < "extras/"$$f".c" | sed "s/printUsage()/$$(echo "$$f")_printUsage()/g" > "box_tmp/"$$f"_box.c"; done

box: box.o
	$(CC) $(CFLAGS) box_tmp/*.c core/common.c box.o -o box 
	rm -f version.h

clean:
	rm -f box *.o
	rm -Rf box_tmp
	for f in $(CORE); do rm -f core/$$f; done
	for f in $(EXTRA); do rm -f extras/$$f; done

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -r core/bin/* $(DESTDIR)$(PREFIX)/bin

install-box:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -r box $(DESTDIR)$(PREFIX)/bin

links:
	for u in ${CORE}; do ln -s "$(DESTDIR)$(PREFIX)/bin/box" "$(DESTDIR)$(PREFIX)/bin/$$u"; done

remove:
	rm -f $(DESTDIR)$(PREFIX)/bin/box
	for u in ${CORE}; do rm -f "$(DESTDIR)$(PREFIX)/bin/$$u"; done


# Utilities
# =========

box.o: prepbox
	$(CC) $(CFLAGS) $(NOLINKER) box.c -o box.o

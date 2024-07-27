# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2024 Runxi Yu <https://runxiyu.org>
# Copyright (C) 2022, 2023 Ferass El Hafidi <vitali64pmemail@protonmail.com>
# Copyright (C) 2022 Leah Rowe <leah@libreboot.org>

.POSIX:
.SUFFIXES: .o .c

include ./config.mk

all: box
OBJ=$(BINS:=.o)

box.c: box-templates/box_1.c box-templates/box_2.c box-templates/box_3.c box-templates/box_4.c $(BINS:=.c)
	for u in ${BINS}; do printf '#define FASESBOX_INCLUDES_%s 1\n' "$${u}"; done > box.c
	cat "box-templates/box_1.c" >> box.c
	for u in ${BINS}; do printf 'int %s_main(int, char**);\n' "$${u}"; done >> box.c
	cat "box-templates/box_2.c" >> box.c
	for u in ${BINS}; do printf '\telse if (!strcmp(basename(argv[0]), "%s")) return %s_main(argc, argv);\n' "$${u}" "$${u}" ; done >> box.c
	cat "box-templates/box_3.c" >> box.c
	for u in ${BINS}; do printf '\t\tprintf("%s");\n' "$${u}" ; done >> box.c
	cat "box-templates/box_4.c" >> box.c

box: box.c $(OBJ)
	$(CC) $(CFLAGS) $(BOXFLAGS) box.c $(BINS:=.o) common.c -o $@

.c.o:
	$(CC) $(CFLAGS) $(BOXFLAGS) $< -c -o $@

clean:
	$(RM) box $(OBJ)

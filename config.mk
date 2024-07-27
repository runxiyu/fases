# SPDX-License-Identifier: GPL-3.0-or-later
#
# Copyright (C) 2024 Runxi Yu <https://runxiyu.org>
# Copyright (C) 2022, 2023 Ferass El Hafidi <vitali64pmemail@protonmail.com>
# Copyright (C) 2022 Leah Rowe <leah@libreboot.org>

VERSION="fases v0.1r"$$(git rev-list --count HEAD)"."$$(git rev-parse --short HEAD)""
CC=cc
CFLAGS=-D_POSIX_C_SOURCE=200809L -DVERSION=\"$(VERSION)\" -I. -Wall -Werror -Wextra -g -pedantic -Wno-unused-parameter -std=c99
BOXFLAGS=-DFASESBOX

# Which programs do you want to build? This affects both the standalone
# programs produced if you run `make standalone', and the programs put
# into the multi-call binary if you run `make box' or simply `make'.
BINS=\
	basename\
	cat\
	chmod\
	chown\
	cmp\
	date\
	dirname\
	echo\
	ed\
	false\
	head\
	link\
	ln\
	ls\
	mkdir\
	more\
	mv\
	printf\
	rm\
	sleep\
	tail\
	tee\
	test\
	touch\
	tput\
	true\
	tty\
	uname\
	unlink\
	wc\
	errno\
	yes

DESTDIR=
PREFIX=/usr/local

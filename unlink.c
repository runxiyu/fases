/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) 2022, 2023 Ferass El Hafidi <vitali64pmemail@protonmail.com>
 */
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define REQ_PRINT_USAGE /* Require print_usage() from common.h */
#define REQ_ERRPRINT /* Require errprint() from common.h */
#define DESCRIPTION "Call the unlink() function."
#define OPERANDS    "file"
#include "common.h"

#ifdef FASESBOX
int unlink_main(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif
	if (argc != 2) {
		print_usage(argv[0], DESCRIPTION, OPERANDS, VERSION);
		return 1;
	}
	unlink(argv[1]);
	if (errno) return errprint(argv[0], argv[1], errno);
	return 0;
}

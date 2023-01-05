/*	sh - the standard command language interpreter
 *	Copyright (C) 2022 Ferass EL HAFIDI
 *
 *	Redistribution and use in source and binary forms, with or without 
 *	modification, are permitted provided that the following conditions are met:
 *	
 *	    1. Redistributions of source code must retain the above copyright notice, 
 *	       this list of conditions and the following disclaimer.
 *	    2. Redistributions in binary form must reproduce the above copyright 
 *	       notice, this list of conditions and the following disclaimer in the 
 *	       documentation and/or other materials provided with the distribution.
 *	    3. Neither the name of the copyright holder nor the names of its 
 *	       contributors may be used to endorse or promote products derived from 
 *	       this software without specific prior written permission.
 *	
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 *	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *	POSSIBILITY OF SUCH DAMAGE.
 */

/* Here's the POSIX specification of sh: */
/* https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html */

/* Define feature test macro. It doesn't compile with gcc without that for 
 * some reason.
 */
#define _POSIX_C_SOURCE 200809L

/* POSIX Header files */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <sys/wait.h>
#include <spawn.h>
#include <libgen.h>
#include <signal.h>

/* Our own header files */
#include "parser.h"
#include "version.h"

#ifndef COMPILETIME
#define COMPILETIME
#endif

void  commandLoop();
int   getopt(int argc, char *const argv[], const char *optstring);
char *pathSearch(const char *path, const char *name);
void  printUsage();

int main(int argc, char *const argv[]) {
	int argument;
	FILE *file;
	struct sigaction signal_action;
	signal_action.sa_handler = SIG_IGN;
	sigemptyset(&signal_action.sa_mask);

	while ((argument = getopt(argc, argv, "c")) != -1) {
		if (argument == '?') {
			printUsage();
			return 1;
		}
	}
	argc -= optind;
	argv += optind;

	sigaction(SIGINT, &signal_action, NULL);
	if (errno) return errno;

	if (argv[0]) {
		file = fopen(argv[0], "r");
		if (errno) return errno;
		commandLoop(file);
	}
	else commandLoop(stdin);
	return 0;
}

/* USAGE
 * -----
 * commandLoop()
 *
 * DESCRIPTION
 * -----------
 * This function is the actual command prompt.
 */

void commandLoop(FILE *filstr) {
	char *prompt = getenv("PS1");
	char *token, *tokenstate;
	char *command[4096];
	char name[4096], tempstr[4096];
	int command_argc;
	if (prompt == NULL) prompt = "$ ";
	for (;;) {
		for (int i = 0; i <= 4096; i++) {
			name[i]    = 0; /* (Re)Initialise name and command, 
			                 * very important. */
			command[i] = 0;
		}
		setvbuf(stdout, NULL, _IONBF, 0);
		if (filstr == stdin && isatty(STDIN_FILENO)) printf(prompt);
		if (fgets(name, 4096, filstr) == NULL)
			break;
		//read(fildes, name, 4096);
		if (!strcmp(name, "\n"))
			continue;
		else if (*name == 0 || *name == EOF) {
			putchar('\n');
			break;
		}
		name[strlen(name) - 1] = 0;
		strcpy(tempstr, name); /* We don't want to override `name`. */
		if (strtok(tempstr, ";") != NULL && strtok(NULL, ";") != NULL) {
			/* Hacky wacky hack. */
			strcpy(tempstr, name);
			token = strtok_r(tempstr, ";", &tokenstate);
			for (; token != NULL;) {
				command_argc = splitCommand(token, command); /* See parser.c */
				parseCommand(command_argc, command); /* See parser.c */
				token = strtok_r(NULL, ";", &tokenstate);
			}
		}
		else {
			command_argc = splitCommand(name, command); /* See parser.c */
			parseCommand(command_argc, command); /* See parser.c */
		}
	}
}

void printUsage() {
	printf("Ferass' Base System. (%s)\n\n"
	"Usage: sh [-c command_string]\n\n"
	"The standard command language interpreter.\n\n"
	"\t-c command_string\tExecute command_string and exit.\n", COMPILETIME);
}

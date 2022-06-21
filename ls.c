/*	ls - list files and directories in the given path
 *	Copyright (C) 2022 Ferass EL HAFIDI
 *	Copyright (C) 2022 Leah Rowe
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>

int getopt(int argc, char *const argv[], const char *optstring);

void printUsage() {
	printf("Ferass' Base System.\n\n"
	"Usage: "
	"ls [DIRECTORY] ...\n\n"
	"Print DIRECTORY's contents to stdout\n\n"
	"\t-a\tInclude names starting with a dot, including '.' and '..'\n"
	"\t-A\tSame as `-a` but don't include '.' and '..'\n"
	"\t-C\tPrint in columns\n");
}

int ls(char *dirname, char params[3]) {
	int file, column;
	DIR *directory;
	struct dirent *dirtree;

	directory = opendir(dirname);

	if (directory == NULL) {
		file = open(dirname, O_RDONLY);
		if (file == -1) {
			printf("ls: %s: No such file or directory\n", dirname);
			return 1;
		}
		printf("%s\n", dirname);
		close(file);
		return 0;
	}

	while ((dirtree = readdir(directory)) != NULL) {
		if (dirtree->d_name[0] != '.' && 
				params[0] != 'a' && params[0] != 'A') { 
			printf("%s", dirtree->d_name);
			if (params[1] != 'C') printf("\n");
		}
		if (params[0] == 'a') {
			printf("%s", dirtree->d_name);
		} else if (params[0] == 'A' && 
				strcmp(dirtree->d_name, ".") &&
				strcmp(dirtree->d_name, "..")) {
			printf("%s", dirtree->d_name);
		}
		if (params[1] != 'C' && (params[0]=='A' || params[0]=='a')) {
			printf("\n");
		} else if (params[1] == 'C' && 
				params[0] != 'a' && params[0] != 'A' && 
				dirtree->d_name[0] != '.') {
			if (column++ > 5) printf("\n");
			else printf("\t\t");
		} else if (params[1] == 'C' && 
				(params[0] == 'a' ||
					(params[0] == 'A' &&
					strcmp(dirtree->d_name, "." )
					&& strcmp(dirtree->d_name, "..")))) {
			if (column > 5) {
				printf("\n");
				column = 0;
			} else {
				printf("\t\t");
				column++;
			}
		}
	}
	printf("\n");

	closedir(directory);
	return 0;
}

int main(int argc, char *argv[]) {
	int status = 0;
	int success = 0;
	int argument, i;
	char params[6]; for(i=0; i<6; i++) params[i]=0;

	setvbuf(stdout, NULL, _IONBF, 0);

	while ((argument = getopt(argc, argv, "haAC")) != -1) {
		if (argument == 'h') {
			printUsage();
			return 0;
		} else if (argument=='a' || argument=='A' || argument=='C')
			params[argument == 'C' ? 1 : 0] = argument;
	}

	for (i = 1; i < argc; i++)
		if ((success |= (argv[i][0] != '-' ? 1 : 0)))
			status |= ls(argv[i], params);

	return success ? status : ls(".", params);
}
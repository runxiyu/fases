/*	ls - list files and directories in the given path
 *	Copyright (C) 2022 Ferass EL HAFIDI
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
#include <stdio.h>   /* To print the file's contents to stdout */
#include <dirent.h>  /* For opening directories */
#include <stdlib.h>  /* For the exit() function */
#include <string.h>  /* To check for arguments */
#include <stdbool.h> /* For 'true' and 'false' and the 'bool' variable type */

int listdir(char *directoryname) {
	DIR *dir; /* Define 'dir' */
	struct dirent *directory;
	dir=opendir(directoryname); /* Open the directory */
	if (dir == NULL) { /* Check if the directory has been opened successfully */
		FILE *file; /* Define 'file' */
		file=fopen(directoryname, "r"); /* Open the file in read-only mode */
		if (file == NULL) {
			printf("ls: %s: No such file or directory\n", directoryname);
			exit(1); /* If not, exit. */
		}
		fclose(file);
		printf("%s\n", directoryname);
		return 0;
	}
	while ((directory = readdir(dir)) != NULL) { /* Until there's nothing else to print */
		/* Print the directory's contents to stdout */
		/* Unless it starts with a dot */
		if (strncmp(directory->d_name, ".", 1))
			printf("%s\n", directory->d_name);
	}
	closedir(dir); /* Close the directory */
	return 0;
}

int main(int argc, char *argv[]) {
	/* Check for arguments */
	if (argc >= 2) {
		char args[26];    /* Number of supported arguments */
		int usedargs = 0; /* Number of used arguments */
		for(int i = 1; argv[i]; i++) { /* For loop */
			if (argv[i][usedargs + 1] == 'A') { 
				args[usedargs] = 'A';
				usedargs++;
			}
			if (argv[i][usedargs + 1] == 'h') {
				/* Print the help message */
				printf("Ferass' Base System.\n");
				printf("\n");
				printf("Usage: %s [DIR/FILE]\n", argv[0]);
				printf("\n");
				printf("List files and directories in DIR or list FILE.\n");
				printf("\n");
				exit(0);
			}
		}
		if (usedargs == 0) { /* If no valid options have been found */
			/* Use the first argument as the directory or file */
			listdir(argv[1]);
		}
		else {
			printf("%c %c\n", args[0], args[1]); /* Placeholder */
		}
	}
	else if (argc == 1) {
		/* Default to opening '.' */
		listdir(".");
	}
	return 0;
}

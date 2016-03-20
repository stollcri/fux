/**
 * fux -- utility to automatically fix mistakes in the last typed command
 *  https://github.com/stollcri/fux
 *
 *
 * Copyright (c) 2016, Christopher Stoll
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of fux nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fux.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dp.h"

static inline void exitapp(int exitcode) __attribute__ ((noreturn));
static inline void exitapp(int exitcode)
{
	exit(exitcode);
}

static inline void handleinterupt(int signum)
{
	exitapp(EXIT_SUCCESS);
}

static inline void setinterupthandlers()
{
	signal(SIGUSR1, handleinterupt);
	signal(SIGUSR2, handleinterupt);
	signal(SIGINT, handleinterupt);
}

static inline char **readconfigfile(char *configfilename, int *possibilitycount)
{
	FILE *configfile = fopen(configfilename, "r");
	if (configfile == NULL) {
		fprintf(stderr, "Error opening '%s': %s\n", configfilename, strerror(errno));
		exitapp(EXIT_FAILURE);
	}

	char currentchar;
	int stringposition = 0;
	int filelineposition = 0;
	int linelength = FILE_LINE_BLOCK_LENGTH;
	int linescount = FILE_LINES_BLOCK_COUNT;
	char *configline = (char*)malloc(linelength * sizeof(char));
	char **configlines = malloc(linescount * sizeof(char*));

	while ((currentchar = fgetc(configfile)) != EOF) {
		if (currentchar == '\n') {
			// end the last line
			configline[stringposition] = '\0';
			// add the line to the array
			configlines[filelineposition] = configline;
			// increment the line counter
			++filelineposition;
			// resize the line count as needed
			if (filelineposition >= linescount) {
				linescount += FILE_LINES_BLOCK_COUNT;
				configlines = realloc(configlines, linescount * sizeof(char*));
			}
			// reset the position for the next line
			stringposition = 0;
			// reset the line length
			linelength = FILE_LINE_BLOCK_LENGTH;
			// start a new line
			configline = (char*)malloc(linelength * sizeof(char));
		} else {
			configline[stringposition] = currentchar;
			++stringposition;
			// resize the line string as needed
			if (stringposition >= linelength) {
				linelength += FILE_LINE_BLOCK_LENGTH;
				configline = realloc(configline, linelength * sizeof(char));
			}
		}
	}
	if (ferror(configfile)) {
	    fprintf(stderr, "IO error: %s\n", strerror(errno));
	    exitapp(EXIT_FAILURE);
	}
	fclose(configfile);

	*possibilitycount = filelineposition;
	return configlines;
}

static inline void readcommandstring(FILE *commandfile, char *commandstring, int *commandlength)
{
	int stringposition = 0;
	char currentchar;
	while ((currentchar = fgetc(commandfile)) != EOF) {
		if (currentchar != '\n') {
			commandstring[stringposition] = currentchar;
			++stringposition;
			// resize the command string as needed
			if (stringposition >= *commandlength) {
				*commandlength = *commandlength + COMMAND_STRING_BLOCK_LENGTH;
				commandstring = realloc(commandstring, *commandlength * sizeof(char));
			}
		}
	}
	if (ferror(commandfile)) {
	    fprintf(stderr, "IO error: %s\n", strerror(errno));
	    exitapp(EXIT_FAILURE);
	}
	commandstring[stringposition] = '\0';
}

//~~~~~~
// MAIN
//~~~~~~

int main(int argc, char **argv)
{
	// first thing, prepare to be interupted
	// (this shouldn't be needed at all, but just in case)
	setinterupthandlers();

	char *argone = argv[1];
	FILE *infile = NULL;
	FILE *outfile = NULL;
	if (argc > 1) {
		// if the first argument is dash, then read from stdin
		if(!strcmp(argone, "-")) {
			infile = stdin;
			outfile = stdout;
		// if the first argument is not dash, assume it's a file name
		} else {
			infile = fopen(argone, "r");
			if (infile == NULL) {
				int showversion = 0;
				if ((argone[0] == '-') && (argone[1] == 'v')) {
					showversion = 1;
				} else if ((argone[0] == '-') && (argone[1] == '-') && (argone[2] == 'v')) {
					showversion = 1;
				} else {
					fprintf(stderr, "Error opening '%s': %s\n", argone, strerror(errno));
					exitapp(EXIT_FAILURE);
				}
				if (showversion == 1) {
					printf(PROGRAM_NAME " " PROGRAM_VERS "\n");
					printf(PROGRAM_COPY "\n");
					printf(PROGRAM_URLS "\n");
					exitapp(EXIT_SUCCESS);
				}
			}
		}
	// otherwise read from stdin
	} else {
		infile = stdin;
		outfile = stdout;
	}

	// read in the command string to process, then close source file
	int commandlength = COMMAND_STRING_BLOCK_LENGTH;
	char *commandstring = (char*)malloc(commandlength * sizeof(char));
	readcommandstring(infile, commandstring, &commandlength);
	fclose(infile);

	// we need to write results back to the source file
	if (outfile == NULL) {
		// re-open the source file for writing (and clear it)
		outfile = fopen(argone, "w");
		if (outfile == NULL) {
			fprintf(stderr, "Error opening '%s': %s\n", argone, strerror(errno));
			exitapp(EXIT_FAILURE);
		}
	}

	// load possible corrections
	int possibilitycount = 0;
	char **possibilities = NULL;
	possibilities = readconfigfile(".fux", &possibilitycount);

	// write the recomended command string, then close the target file
	char *recomendedstring = NULL;
	recomendedstring = bestmatch(commandstring, possibilities, possibilitycount);

	if (DEBUG_OUTPUT) fputs("# ", outfile);
	fputs(recomendedstring, outfile);
	fputs("\n", outfile);
	fclose(outfile);

	exitapp(EXIT_SUCCESS);
}

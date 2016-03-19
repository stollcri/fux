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

static inline void readcommandstring(FILE *commandfile, char *commandstring, int *commandlength)
{
	int stringposition = 0;
	char currentchar;
	while ((currentchar = fgetc(commandfile)) != EOF) {
		commandstring[stringposition] = currentchar;
		++stringposition;
		// resize the command string as needed
		if (stringposition >= *commandlength) {
			*commandlength = *commandlength + COMMAND_STRING_BLOCK_LENGTH;
			commandstring = realloc(commandstring, *commandlength * sizeof(char));
		}
	}
	commandstring[stringposition] = '\0';
}

//~~~~~~
// MAIN
//~~~~~~

int main(int argc, char **argv)
{
	// first thing, prepare to be interupted
	setinterupthandlers();

	FILE *infile = NULL;
	FILE *outfile = NULL;
	if (argc > 1) {
		// if the first argument is dash, then read from stdin
		if(!strcmp(argv[1], "-")) {
			infile = stdin;
			outfile = stdout;
		// if the first argument is not dash, assume it's a file name
		} else {
			infile = fopen(argv[1], "r");
			if (infile == NULL) {
				fprintf(stderr, "Error opening '%s': %s\n", argv[1], strerror(errno));
				exitapp(EXIT_FAILURE);
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
		outfile = fopen(argv[1], "w");
	}



	// write the recomended command string, then close the target file
	char *recomendedstring;

	int possibilitycount = 6;
	// TODO: load from file
	char *possibilities[possibilitycount];
	possibilities[0] = "brew install *";
	possibilities[1] = "git branch";
	possibilities[2] = "git push --set-upstream *";
	possibilities[3] = "make test";
	possibilities[4] = "sudo apt-get remove * --confirm";
	possibilities[5] = "sudo apt-get install * -?";
	recomendedstring = bestmatch(commandstring, possibilities, possibilitycount);
	printf("# %s\n", commandstring);

	fputs("# ", outfile);
	fputs(recomendedstring, outfile);
	fputs("\n", outfile);
	fclose(outfile);



	exitapp(EXIT_SUCCESS);
}

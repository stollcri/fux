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

#include "dp.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "libMinMax.c"

#include <stdio.h>

static int similarityscore(char charx, char chary)
{
/*
//                !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}
 charmatrix[0] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ! 
 charmatrix[1] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // " 
 charmatrix[2] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // # 
 charmatrix[3] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // $ 
 charmatrix[4] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // % 
 charmatrix[5] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // & 
 charmatrix[6] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ' 
 charmatrix[7] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ( 
 charmatrix[8] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ) 
 charmatrix[9] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // * 
charmatrix[10] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // + 
charmatrix[11] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // , 
charmatrix[12] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // - 
charmatrix[13] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // . 
charmatrix[14] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // / 
charmatrix[15] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 0 
charmatrix[16] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 1 
charmatrix[17] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 2 
charmatrix[18] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 3 
charmatrix[19] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 4 
//                !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}
charmatrix[20] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 5 
charmatrix[21] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 6 
charmatrix[22] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 7 
charmatrix[23] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 8 
charmatrix[24] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // 9 
charmatrix[25] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // : 
charmatrix[26] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ; 
charmatrix[27] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // < 
charmatrix[28] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // = 
charmatrix[29] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // > 
charmatrix[30] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ? 
charmatrix[31] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // @ 
charmatrix[32] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // A 
charmatrix[33] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // B 
charmatrix[34] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // C 
charmatrix[35] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // D 
charmatrix[36] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // E 
charmatrix[37] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // F 
charmatrix[38] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // G 
charmatrix[39] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // H 
//                !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}
charmatrix[40] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // I 
charmatrix[41] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // J 
charmatrix[42] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // K 
charmatrix[43] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // L 
charmatrix[44] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // M 
charmatrix[45] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // N 
charmatrix[46] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // O 
charmatrix[47] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // P 
charmatrix[48] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // Q 
charmatrix[49] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // R 
charmatrix[50] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // S 
charmatrix[51] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // T 
charmatrix[52] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // U 
charmatrix[53] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // V 
charmatrix[54] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // W 
charmatrix[55] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // X 
charmatrix[56] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // Y 
charmatrix[57] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // Z 
charmatrix[58] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // [ 
charmatrix[59] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // \ 
//                !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}
charmatrix[60] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ] 
charmatrix[61] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ^ 
charmatrix[62] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // _ 
charmatrix[63] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // ` 
charmatrix[64] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // a 
charmatrix[65] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // b 
charmatrix[66] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // c 
charmatrix[67] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // d 
charmatrix[68] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // e 
charmatrix[69] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // f 
charmatrix[70] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // g 
charmatrix[71] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // h 
charmatrix[72] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // i 
charmatrix[73] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // j 
charmatrix[74] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // k 
charmatrix[75] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // l 
charmatrix[76] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // m 
charmatrix[77] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // n 
charmatrix[78] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // o 
charmatrix[79] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // p 
//                !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}
charmatrix[80] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // q 
charmatrix[81] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // r 
charmatrix[82] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // s 
charmatrix[83] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // t 
charmatrix[84] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // u 
charmatrix[85] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // v 
charmatrix[86] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // w 
charmatrix[87] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // x 
charmatrix[88] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // y 
charmatrix[89] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // z 
charmatrix[90] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // { 
charmatrix[91] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // | 
charmatrix[92] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // }
*/
	return SCORE_MISMATCH;
}

static int characterscore(char charx, char chary)
{
	// exact match
	if(charx == chary){
		return SCORE_MATCH;
	// wildcard in the possibility
	// (could probably also be handled by character similarity matrix, this is a shortcut)
	} else if (chary == '*') {
		return SCORE_WILDCARD;
	// check character similarity
	} else {
		return similarityscore(charx, chary);
	}
}

static int gappedscore(char charx, char chary)
{
	if ((charx == ' ') || (chary == ' ')) {
		return PENALTY_GAP;
	} else {
		return PENALTY_MISMATCH;
	}
}

static int transposescore(char charx, char chary)
{
	// TODO: do something here
	return PENALTY_TRANSPOSE;
}

static int fillmatrix(int *dpmatrix, int xsize, int ysize, char *xstring, char *ystring)
{
	int result = 0;

	int currentxchar = 0;
	int currentychar = 0;
	int gapscore = 0;
	int charscore = 0;

	int currentindex = 0;
	int indexdiag = 0;
	int indexleft = 0;
	int indexabove = 0;

	for (int y = 0; y < ysize; ++y) {
		for (int x = 0; x < xsize; ++x) {
			currentxchar = xstring[x - 1];
			currentychar = ystring[y - 1];

			// TODO: start both of the above loops at 1
			// 		 and remove this condition
			// 		 and remove the print condition below
			if ((x > 0) & (y > 0)) {
				gapscore = gappedscore(currentxchar, currentychar);
				charscore = characterscore(currentxchar, currentychar);

				currentindex = (y * xsize) + x;
				indexdiag = ((y - 1) * xsize) + (x - 1);
				indexleft = (y * xsize) + (x - 1);
				indexabove = ((y - 1) * xsize) + x;
				
				// the value of the current pixels is:
				// - the maximum of:
				//   - the diagonal value and the current score (there is a series of matches)
				//   - 0 (just in case all scores are below zero)
				//   - the above value and the gap score (there is a gap in the matches)
				//   - the left value and the gap score (there is a gap in the matches)
				// - minus the cost of movement
				dpmatrix[currentindex] = max4( \
					dpmatrix[indexdiag] + charscore, \
					0, \
					dpmatrix[indexabove] + gapscore, \
					dpmatrix[indexleft] + gapscore \
				) + MOVE_COST;

				// values above and left are higher than diagonal, suggesting a transpose
				if ((dpmatrix[indexabove] > dpmatrix[indexdiag]) && (dpmatrix[indexleft] > dpmatrix[indexdiag])) {
					// give the diagonal the maximum value of the other neighbors
					dpmatrix[indexdiag] = max( \
						dpmatrix[indexabove], \
						dpmatrix[indexleft] \
					);
					// give current space the value of transposed caharacters, if less than current value
					dpmatrix[currentindex] = max( \
						dpmatrix[indexdiag] + transposescore(currentxchar, currentychar), \
						dpmatrix[currentindex] \
					);
				}
			}

			if ((x == 0) || (y == 0)) {
				if ((x == 0) && (y == 0)) {
					printf("  ");
				}
				if ((x != 0) && (y == 0)) {
					printf("_%c_ ", currentxchar);
				}
				if ((x == 0) && (y != 0)) {
					printf("%c ", currentychar);
				}
			} else {
				printf("%3d ", dpmatrix[currentindex]);
			}
		}
		printf("\n");
	}
	printf("\n");
	return result;
}

static struct backtrackresults *backtrack(int *dpmatrix, int xsize, int ysize, char *commandstring)
{
	struct backtrackresults *results = (struct backtrackresults*)malloc(sizeof(struct backtrackresults));

	// 1. Find the local maximum, in the bottom row or right column
	//    This determines the starting point for the backtrack
	
	int localmax = 0;
	int localmaxindex = 0;
	int currentindex = 0;
	int argumentxcut = 0;

	// check bottom-most row for a maximum
	for (int x = 1; x < xsize; ++x) {
		currentindex = (ysize * xsize) - x;
		if (dpmatrix[currentindex] > localmax) {
			localmaxindex = currentindex;
			localmax = dpmatrix[localmaxindex];
			argumentxcut = x;//xsize - x - 1;
			printf("=1=> %d : %d (%d, %d)\n", localmax, argumentxcut, currentindex, xsize);
		}
	}

	// check right-most column for a maximum
	for (int y = (ysize - 1); y > 0; --y) {
		currentindex = (y * xsize) + (xsize - 1);
		if (dpmatrix[currentindex] > localmax) {
			localmaxindex = currentindex;
			localmax = dpmatrix[localmaxindex];
			argumentxcut = (xsize - 1);
			printf("=2=> %d : %d (%d, %d)\n", localmax, argumentxcut, currentindex, xsize);
		}
	}

	// 2. Find the global maximum by backtracking from local max
	
	currentindex = localmaxindex;
	int globalmax = localmax;
	
	int indexdiag = 0;
	int indexleft = 0;
	int indexabove = 0;
	int valuediag = 0;
	int valueleft = 0;
	int valueabove = 0;

	int continuetesting = 1;
	while (continuetesting) {
		if (dpmatrix[currentindex] > globalmax) {
			globalmax = dpmatrix[currentindex];
			argumentxcut = abs((int)remainder((float)currentindex, (float)xsize));
			printf("=3=> %d : %d (%d, %d)\n", globalmax, argumentxcut, currentindex, xsize);
		}

		indexleft = currentindex - 1;
		indexabove = currentindex - xsize;
		indexdiag = indexabove - 1;
		// make sure we don't go outside the matrix bounds
		if (indexleft > 0) {
			valueleft = dpmatrix[indexleft];
		} else {
			valueleft = INT_MIN;
		}
		if (indexabove > 0) {
			valueabove = dpmatrix[indexabove];
		} else {
			valueabove = INT_MIN;
		}
		if (indexdiag > 0) {
			valuediag = dpmatrix[indexdiag];
		} else {
			valuediag = INT_MIN;
		}

		// no equals, so biased up (ties hit else)
		if (valueleft > valueabove) {
			// no equals, so biased diagonal (ties hit else)
			if (valueleft > valuediag) {
				// go left
				currentindex = indexleft;
			} else {
				// go diag
				currentindex = indexdiag;
			}
		} else {
			// no equals, so biased diagonal (ties hit else)
			if (valueabove > valuediag) {
				// go up
				currentindex = indexabove;
			} else {
				// go diag
				currentindex = indexdiag;
			}
		}

		if ((valueabove == INT_MIN) && (valueleft == INT_MIN) && (valuediag == INT_MIN)) {
			continuetesting = 0;
		}
	}

	// 3. Find the argument portion of the given command string
	int argindex = 0;
	char *commandargs = (char*)malloc((xsize - argumentxcut + 1) * sizeof(char));
	for (int i = (xsize - (argumentxcut + 1)); i < xsize; ++i) {
		commandargs[argindex] = commandstring[i];
		++argindex;
	}
	commandargs[argindex] = '\0';
	printf("<<< %s >>>\n", commandargs);

	results->commandargs = commandargs;
	results->score = globalmax;
	return results;
}

static struct backtrackresults *dpscore(char *commandstring, char *possibility)
{
	int score = 0;
	// add one to each dimension for the padding row/collumn
	int xsize = strlen(commandstring) + 1;
	int ysize = strlen(possibility) + 1;
	int *dpmatrix = (int*)calloc(xsize * ysize, sizeof(int));
	fillmatrix(dpmatrix, xsize, ysize, commandstring, possibility);

	struct backtrackresults *btresults;
	btresults = backtrack(dpmatrix, xsize, ysize, commandstring);

	score = btresults->score;
	printf("||| %d |||\n", score);

	if(dpmatrix) {
		free(dpmatrix);
	}
	return btresults;
}

char *buildcommand(char *commandstr, char *commandarg)
{
	int cmdsize = strlen(commandstr) + 1;
	int argsize = strlen(commandarg) + 1;
	int newsize = (cmdsize + argsize + 1);
	char *result = (char*)malloc(newsize * sizeof(char));

	int j = 0;
	for (int i = 0; i < cmdsize; ++i) {
		if (commandstr[i] != '*') {
			result[j] = commandstr[i];
			++j;
		} else {
			printf("ok\n");
			for (int k = 0; k < argsize; ++k) {
				result[j] = commandarg[k];
				++j;
			}
		}
	}
	result[j] = '\0';
	return result;
}

char *bestmatch(char *commandstring, char **possibilities, int possibilitycount)
{
	int maxscore = 0;
	int maxscoreid = 0;
	char *maxscoreargs = NULL;
	struct backtrackresults *btresults;

	for (int i = 0; i < possibilitycount; ++i) {
		btresults = dpscore(commandstring, possibilities[i]);
		btresults->score = btresults->score;
		if (btresults->score > maxscore) {
			maxscoreid = i;
			maxscore = btresults->score;
			maxscoreargs = btresults->commandargs;
		}
	}

	return buildcommand(possibilities[maxscoreid], maxscoreargs);
}

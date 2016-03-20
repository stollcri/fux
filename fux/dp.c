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
// 9 -- same value
// 8 -- difference in case (e.g. a, A -or- b, B)
// 7 -- 
// 6 -- next to on the keyboard (e.g. q, w -or- q, a)
// 5 -- looks similar (e.g. 1, l, I -or- 0, o, O)
// 4 -- diagonal on the keyboard (e.g. a, z -or- s, x)
// 3 -- can sound similar (e.g. c, k -or- f, ph)
// 2 -- difference in shift state (e.g. 1, ! -or- 2, @)
// 1 -- looks close (e.g. t, f -or- B, D)
// 0 -- no similarity
char *charmatrix[93];
//                !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}
 charmatrix[0] = "900000000000000020000000000000000000000000000000000000000000000000000000000000000000000000000"; // !
 charmatrix[1] = "090000800000000000000000000000000000000000000000000000000000000800000000000000000000000000000"; // "
 charmatrix[2] = "009000000000000000200000000000000000000000000000000000000000000000000000000000000000000000000"; // #
 charmatrix[3] = "000900000000000000020000000000000000000000000000000000000000000000000000000000000000000000000"; // $
 charmatrix[4] = "000090000000000000002000000000000000000000000000000000000000000000000000000000000000000000000"; // %
 charmatrix[5] = "000009000000000000000020000000000000000000000000000000000000000000000000000000000000000000000"; // &
 charmatrix[6] = "080000900000000000000000000000000000000000000000000000000000000800000000000000000000000000000"; // '
 charmatrix[7] = "000000090000000000000000200000000000000000000000000000000020000000000000000000000000000000000"; // (
 charmatrix[8] = "000000009000000200000000000000000000000000000000000000000000200000000000000000000000000000000"; // )
 charmatrix[9] = "000000000900000000000002000000000000000000000000000000000000000000000000000000000000000000000"; // *
charmatrix[10] = "000000000090000000000000000020000000000000000000000000000000000000000000000000000000000000000"; // +
charmatrix[11] = "000000000009000000000000000200000000000000000000000000000000000000000000000400000000000000000"; // ,
charmatrix[12] = "000000000000900000000000000000000000000000000000000000000000002000000000000000000000000000000"; // -
charmatrix[13] = "000000000000090000000000000002000000000000000000000000000000000000000000000400000000000000000"; // .
charmatrix[14] = "000000000000009000000000000000200000000000000000000000000000000000000000000000000000000000000"; // /
charmatrix[15] = "000000002000000900000000000000000000000000000050000000000000000000000000000000500000000000000"; // 0
charmatrix[16] = "200000000000000090000000000000000000000050000000000000000000000000000000000500000000000000000"; // 1
charmatrix[17] = "000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000"; // 2
charmatrix[18] = "002000000000000000900000000000000000000000000000000000000000000000000000000000000000000000000"; // 3
charmatrix[19] = "000200000000000000090000000000000000000000000000000000000000000000000000000000000000000000000"; // 4
charmatrix[20] = "000020000000000000009000000000000000000000000000000000000000000000000000000000000000000000000"; // 5
charmatrix[21] = "000000000000000000000900000000000000000000000000000000000000020000000000000000000000000000000"; // 6
charmatrix[22] = "000002000000000000000090000000000000000000000000000000000000000000000000000000000000000000000"; // 7
charmatrix[23] = "000000000200000000000009000000000000000000000000000000000000000000000000000000000000000000000"; // 8
charmatrix[24] = "000000020000000000000000920000000000000000000000000000000000000000000000000000000000000000000"; // 9
charmatrix[25] = "000000000000000000000000292000000000000000060000000000000000000000000000000000000000000000000"; // :
charmatrix[26] = "000000000000000000000000029000000000000000000000000000000000000000000000000600000000000000000"; // ;
charmatrix[27] = "000000000002000000000000000900000000000000000000000000000000000000000000000000000000000000000"; // <
charmatrix[28] = "000000000020000000000000000090000000000000000000000000000000000000000000000000000000000000000"; // =
charmatrix[29] = "000000000000020000000000000009000000000000000000000000000000000000000000000000000000000000000"; // >
charmatrix[30] = "000000000000002000000000000000900000000000000000000000000000000000000000000000000000000000000"; // ?
charmatrix[31] = "000000000000000002000000000000090000000000000000000000000000000000000000000000000000000000000"; // @
charmatrix[32] = "000000000000000000000000000000009000000000000000606000400400000080000000000000000000000000000"; // A
charmatrix[33] = "000000000000000000000000000000000901004400000600010006000000000008000000000000000000000000000"; // B
charmatrix[34] = "000000000000000000000000000000000094041000300010001006060000000000800000000000000000000000000"; // C
charmatrix[35] = "000000000000000000000000000000000149660000000000046300040000000000080000000000000000000000000"; // D
charmatrix[36] = "000000000000000000000000000000000006910000000000064000600000000000008000000000000000000000000"; // E
charmatrix[37] = "000000000000000000000000000000000046196000000003060404000000000000000800000000000000000000000"; // F
charmatrix[38] = "000000000000000000000000000000000410069600000010100604000000000000000080000000000000000000000"; // G
charmatrix[39] = "000000000000000000000000000000000400006906000400000000006000000000000008000000000000000000000"; // H
charmatrix[40] = "000000000000000050000000000000000000000094600060000060000000000000000000800500000000000000000"; // I
charmatrix[41] = "000000000000000000000000000000000000000649604400000060004000000000000000080000000000000000000"; // J
charmatrix[42] = "000000000000000000000000000000000030000066964040000000000000000000000000008000000000000000000"; // K
charmatrix[43] = "000000000000000000000000060000000000000000690064000000003000000000000000000800000000000000000"; // L
charmatrix[44] = "000000000000000000000000000000000000000004409600000000000000000000000000000080000000000000000"; // M
charmatrix[45] = "000000000000000000000000000000000600000404006900000000000000000000000000000008000000000000000"; // N
charmatrix[46] = "000000000000000500000000000000000010001060460096000000000000000000000000000000800000000000000"; // O
charmatrix[47] = "000000000000000000000000000000000000030000040069000000000000000000000000000000080000000000000"; // P
charmatrix[48] = "000000000000000000000000000000006000001000000000900000600000000000000000000000008000000000000"; // Q
charmatrix[49] = "000000000000000000000000000000000104660000000000090600000000000000000000000000000800000000000"; // R
charmatrix[50] = "000000000000000000000000000000006016400000000000009000640400000000000000000000000080000000000"; // S
charmatrix[51] = "000000000000000000000000000000000003046000000000060900006000000000000000000000000008000000000"; // T
charmatrix[52] = "000000000000000000000000000000000000000066000000000091106000000000000000000000000000800000000"; // U
charmatrix[53] = "000000000000000000000000000000000660044000000000000019000000000000000000000000000000080000000"; // V
charmatrix[54] = "000000000000000000000000000000004000600000000000606010900000000000000000000000000000008000000"; // W
charmatrix[55] = "000000000000000000000000000000000064000000000000004000090600000000000000000000000000000800000"; // X
charmatrix[56] = "000000000000000000000000000000000000000604030000000660009000000000000000000000000000000080000"; // Y
charmatrix[57] = "000000000000000000000000000000004000000000000000004000060900000000000000000000000000000008000"; // Z
charmatrix[58] = "000000020000000000000000000000000000000000000000000000000090000000000000000000000000000000200"; // [
charmatrix[59] = "000000000000000000000000000000000000000000000000000000000009000000000000000000000000000000020"; // \0
charmatrix[60] = "000000002000000000000000000000000000000000000000000000000000900000000000000000000000000000002"; // ]
charmatrix[61] = "000000000000000000000200000000000000000000000000000000000000090000000000000000000000000000000"; // ^
charmatrix[62] = "000000000000200000000000000000000000000000000000000000000000009000000000000000000000000000000"; // _
charmatrix[63] = "080000800000000000000000000000000000000000000000000000000000000900000000000000000000000000000"; // `
charmatrix[64] = "000000000000000000000000000000008000000000000000000000000000000090000000000000006060004004000"; // a
charmatrix[65] = "000000000000000000000000000000000800000000000000000000000000000009010044000006000100060000000"; // b
charmatrix[66] = "000000000000000000000000000000000080000000000000000000000000000000940410003000100010060600000"; // c
charmatrix[67] = "000000000000000000000000000000000008000000000000000000000000000001496600000000000463000400000"; // d
charmatrix[68] = "000000000000000000000000000000000000800000000000000000000000000000069100000000000640006000000"; // e
charmatrix[69] = "000000000000000000000000000000000000080000000000000000000000000000461960000000030604040000000"; // f
charmatrix[70] = "000000000000000000000000000000000000008000000000000000000000000004100696000000101006040000000"; // g
charmatrix[71] = "000000000000000000000000000000000000000800000000000000000000000004000069060004000000000060000"; // h
charmatrix[72] = "000000000000000000000000000000000000000080000000000000000000000000000000946000600000600000000"; // i
charmatrix[73] = "000000000000000000000000000000000000000008000000000000000000000000000006496044000000600040000"; // j
charmatrix[74] = "000000000000000000000000000000000000000000800000000000000000000000300000669640400000000000000"; // k
charmatrix[75] = "000000000004040050000000006000000000000050080000000000000000000000000000006900640000000030000"; // l
charmatrix[76] = "000000000000000000000000000000000000000000008000000000000000000000000000044096000000000000000"; // m
charmatrix[77] = "000000000000000000000000000000000000000000000800000000000000000006000004040069000000000000000"; // n
charmatrix[78] = "000000000000000500000000000000000000000000000080000000000000000000100010604600960000000000000"; // o
charmatrix[79] = "000000000000000000000000000000000000000000000008000000000000000000000300000400690000000000000"; // p
charmatrix[80] = "000000000000000000000000000000000000000000000000800000000000000060000010000000009000006000000"; // q
charmatrix[81] = "000000000000000000000000000000000000000000000000080000000000000001046600000000000906000000000"; // r
charmatrix[82] = "000000000000000000000000000000000000000000000000008000000000000060164000000000000090006404000"; // s
charmatrix[83] = "000000000000000000000000000000000000000000000000000800000000000000030460000000000609000060000"; // t
charmatrix[84] = "000000000000000000000000000000000000000000000000000080000000000000000000660000000000911060000"; // u
charmatrix[85] = "000000000000000000000000000000000000000000000000000008000000000006600440000000000000190000000"; // v
charmatrix[86] = "000000000000000000000000000000000000000000000000000000800000000040006000000000006060109000000"; // w
charmatrix[87] = "000000000000000000000000000000000000000000000000000000080000000000640000000000000040000906000"; // x
charmatrix[88] = "000000000000000000000000000000000000000000000000000000008000000000000006040300000006600090000"; // y
charmatrix[89] = "000000000000000000000000000000000000000000000000000000000800000040000000000000000040000609000"; // z
charmatrix[90] = "000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000900"; // {
charmatrix[91] = "000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000090"; // |
charmatrix[92] = "000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000009"; // ]

	int charxpos = charx - (32 + 1);
	if ((charxpos >= 0) && (charxpos <= 92)) {
		int charypos = chary - (32 + 1);
		if ((charypos >= 0) && (charypos <= 92)) {
			char *currentrow = charmatrix[charxpos];
			int currentcol = (currentrow[charypos] - 48);
			if (currentcol) {
				return currentcol + SCORE_SIMILARITY;
			} else {
				return SCORE_MISMATCH;
			}
		} else {
			return SCORE_MISMATCH;
		}
	} else {
		return SCORE_MISMATCH;
	}
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

	if (DEBUG_PRINT_MATRIX) printf("\n");
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

			if (DEBUG_PRINT_MATRIX) {
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
		}
		if (DEBUG_PRINT_MATRIX) printf("\n");
	}
	if (DEBUG_PRINT_MATRIX) printf("\n");
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
			if (DEBUG_PRINT_ARGPOS) printf("=1=> %d : %d (%d, %d)\n", localmax, argumentxcut, currentindex, xsize);
		}
	}

	// check right-most column for a maximum
	for (int y = (ysize - 1); y > 0; --y) {
		currentindex = (y * xsize) + (xsize - 1);
		if (dpmatrix[currentindex] > localmax) {
			localmaxindex = currentindex;
			localmax = dpmatrix[localmaxindex];
			argumentxcut = 1;
			if (DEBUG_PRINT_ARGPOS) printf("=2=> %d : %d (%d, %d)\n", localmax, argumentxcut, currentindex, xsize);
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
			if (DEBUG_PRINT_ARGPOS) printf("=3=> %d : %d (%d, %d)\n", globalmax, argumentxcut, currentindex, xsize);
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
	if (DEBUG_PRINT_ARGVAL) printf("===> '%s'\n", commandargs);

	int scorepercent = (int)(((double)globalmax / (double)(xsize * EXPECTED_BOX_SCORE)) * 100);

	results->commandargs = commandargs;
	results->score = scorepercent;
	return results;
}

static struct backtrackresults *dpscore(char *commandstring, char *possibility)
{
	// add one to each dimension for the padding row/collumn
	int xsize = strlen(commandstring) + 1;
	int ysize = strlen(possibility) + 1;
	int *dpmatrix = (int*)calloc(xsize * ysize, sizeof(int));
	fillmatrix(dpmatrix, xsize, ysize, commandstring, possibility);

	struct backtrackresults *btresults;
	btresults = backtrack(dpmatrix, xsize, ysize, commandstring);

	if (DEBUG_PRINT_SCORES) printf("%4d %s\n", btresults->score, possibility);

	if(dpmatrix) {
		free(dpmatrix);
	}
	return btresults;
}

char *buildcommand(char *commandstr, char *commandarg, int score)
{
	int cmdsize = strlen(commandstr) + 1;
	int argsize = strlen(commandarg) + 1;
	int newsize = (cmdsize + argsize + 1);
	if (score < MINIMUM_GOOD_SCORE) {
		newsize += 2; 
	}
	char *result = (char*)malloc(newsize * sizeof(char));
	int wildcardused = 0;

	int resultindex = 0;
	if (score < MINIMUM_GOOD_SCORE) {
		result[resultindex] = '#';
		resultindex = 1;
	}
	for (int cmdindex = 0; cmdindex < cmdsize; ++cmdindex) {
		if (commandstr[cmdindex] != '*') {
			result[resultindex] = commandstr[cmdindex];
			++resultindex;
		} else {
			if (!wildcardused) {
				wildcardused = 1;
				for (int argindex = 0; argindex < argsize; ++argindex) {
					result[resultindex] = commandarg[argindex];
					++resultindex;
				}
			}
		}
	}
	result[resultindex] = '\0';
	return result;
}

char *bestmatch(char *commandstring, char **possibilities, int possibilitycount)
{
	int maxscore = 0;
	int maxscoreid = 0;
	char *maxscoreargs = NULL;
	struct backtrackresults *btresults = NULL;

	for (int i = 0; i < possibilitycount; ++i) {
		if (possibilities[i]) {
			btresults = dpscore(commandstring, possibilities[i]);
			if (btresults) {
				if (btresults->score > maxscore) {
					maxscoreid = i;
					maxscore = btresults->score;
					maxscoreargs = btresults->commandargs;
				}
				free(btresults);
			}
		}
	}

	if (maxscoreargs) {
		return buildcommand(possibilities[maxscoreid], maxscoreargs, maxscore);
	} else {
		return commandstring;
	}
}

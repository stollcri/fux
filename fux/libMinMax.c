#ifndef LIBMINMAX_C
#define LIBMINMAX_C

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


static inline int max(int a, int b)
{
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

static inline int max3(int a, int b, int c)
{
	if (a > b) {
		if (a > c) {
			return a;
		} else {
			return c;
		}
	} else {
		if (b > c) {
			return b;
		} else {
			return c;
		}
	}
}

static inline int max4(int a, int b, int c, int d)
{
	if (a > b) {
		if (a > c) {
			if (a > d) {
				return a;
			} else {
				return d;
			}
		} else {
			if (c > d) {
				return c;
			} else {
				return d;
			}
		}
	} else {
		if (b > c) {
			if (b > d) {
				return b;
			} else {
				return d;
			}
		} else {
			if (c > d) {
				return c;
			} else {
				return d;
			}
		}
	}
}

static inline int min(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

static inline int min3(int a, int b, int c)
{
	if (a < b) {
		if (a < c) {
			return a;
		} else {
			return c;
		}
	} else {
		if (b < c) {
			return b;
		} else {
			return c;
		}
	}
}

static inline int min4(int a, int b, int c, int d)
{
	if (a < b) {
		if (a < c) {
			if (a < d) {
				return a;
			} else {
				return d;
			}
		} else {
			if (c < d) {
				return c;
			} else {
				return d;
			}
		}
	} else {
		if (b < c) {
			if (b < d) {
				return b;
			} else {
				return d;
			}
		} else {
			if (c < d) {
				return c;
			} else {
				return d;
			}
		}
	}
}

#endif

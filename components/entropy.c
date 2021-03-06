/* See LICENSE file for copyright and license details. */
#if defined(__linux__)
	#include <stdio.h>

	#include "../util.h"

	const char *
	entropy(void)
	{
		int num;

		return (pscanf("/proc/sys/kernel/random/entropy_avail",
		               "%d", &num) == 1) ?
		       bprintf("%d", num) : NULL;
	}
#elif defined(__OpenBSD__)
	const char *
	entropy(void)
	{
		/* Unicode Character 'INFINITY' (U+221E) */
		return "\xe2\x88\x9e";
	}
#endif

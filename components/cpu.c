/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"

#if defined(__linux__)
	const char *
	cpu_freq(void)
	{
		int freq;

		return (pscanf("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
		               "%i", &freq) == 1) ?
		       bprintf("%d", (freq + 500) / 1000) : NULL;
	}

	const char *
	cpu_perc(void)
	{
		int perc;
		static long double a[7];
		static int valid;
		long double b[7];

		memcpy(b, a, sizeof(b));
		/* cpu user nice system idle iowait irq softirq */
		if (pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf",
		           &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6]) != 7) {
			return NULL;
		}
		if (!valid) {
			valid = 1;
			return NULL;
		}

		perc = 100 * ((b[0] + b[1] + b[2] + b[5] + b[6]) -
		              (a[0] + a[1] + a[2] + a[5] + a[6])) /
		             ((b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) -
		              (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]));

		return bprintf("%d", perc);
	}
#elif defined(__OpenBSD__)
	#include <sys/param.h>
	#include <sys/sched.h>
	#include <sys/sysctl.h>

	const char *
	cpu_freq(void)
	{
		int freq, mib[2];
		size_t size;

		mib[0] = CTL_HW;
		mib[1] = HW_CPUSPEED;

		size = sizeof(freq);

		if (sysctl(mib, 2, &freq, &size, NULL, 0) < 0) {
			fprintf(stderr, "sysctl 'HW_CPUSPEED': %s\n", strerror(errno));
			return NULL;
		}

		return bprintf("%d", freq);
	}

	const char *
	cpu_perc(void)
	{
		int mib[2], perc;
		static int valid;
		static long int a[CPUSTATES];
		long int b[CPUSTATES];
		size_t size;

		mib[0] = CTL_KERN;
		mib[1] = KERN_CPTIME;

		size = sizeof(a);

		memcpy(b, a, sizeof(b));
		if (sysctl(mib, 2, &a, &size, NULL, 0) < 0) {
			fprintf(stderr, "sysctl 'KERN_CPTIME': %s\n", strerror(errno));
			return NULL;
		}
		if (!valid) {
			valid = 1;
			return NULL;
		}

		perc = 100 *
		      ((a[CP_USER] + a[CP_NICE] + a[CP_SYS] + a[CP_INTR]) -
		       (b[CP_USER] + b[CP_NICE] + b[CP_SYS] + b[CP_INTR])) /
		      ((a[CP_USER] + a[CP_NICE] + a[CP_SYS] + a[CP_INTR] + a[CP_IDLE]) -
		       (b[CP_USER] + b[CP_NICE] + b[CP_SYS] + b[CP_INTR] + b[CP_IDLE]));

		return bprintf("%d", perc);
	}
#endif

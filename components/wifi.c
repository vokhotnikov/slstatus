/* See LICENSE file for copyright and license details. */
#if defined(__linux__)
	#include <errno.h>
	#include <ifaddrs.h>
	#include <linux/wireless.h>
	#include <sys/socket.h>
	#include <stdio.h>
	#include <limits.h>
	#include <string.h>
	#include <sys/ioctl.h>
	#include <unistd.h>

	#include "../util.h"

	const char *
	wifi_perc(const char *iface)
	{
		int i, cur;
		float perc;
		int total = 70; /* the max of /proc/net/wireless */
		char *p, *datastart;
		char path[PATH_MAX];
		char status[5];
		FILE *fp;

		snprintf(path, sizeof(path), "%s%s%s", "/sys/class/net/", iface,
		         "/operstate");
		if (!(fp = fopen(path, "r"))) {
			fprintf(stderr, "fopen '%s': %s\n", path,
			        strerror(errno));
			return NULL;
		}
		p = fgets(status, 5, fp);
		fclose(fp);
		if(!p || strcmp(status, "up\n") != 0) {
			return NULL;
		}

		if (!(fp = fopen("/proc/net/wireless", "r"))) {
			fprintf(stderr, "fopen '/proc/net/wireless': %s\n",
			        strerror(errno));
			return NULL;
		}

		for (i = 0; i < 3; i++) {
			if (!(p = fgets(buf, sizeof(buf) - 1, fp)))
				break;
		}
		fclose(fp);
		if (i < 2 || !p) {
			return NULL;
		}

		if (!(datastart = strstr(buf, iface))) {
			return NULL;
		}

		datastart = (datastart+(strlen(iface)+1));
		sscanf(datastart + 1, " %*d   %d  %*d  %*d\t\t  %*d\t   "
		       "%*d\t\t%*d\t\t %*d\t  %*d\t\t %*d", &cur);

		perc = (float)cur / total * 100.0;

		return bprintf("%.0f", perc);
	}

	const char *
	wifi_essid(const char *iface)
	{
		static char id[IW_ESSID_MAX_SIZE+1];
		int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		struct iwreq wreq;

		memset(&wreq, 0, sizeof(struct iwreq));
		wreq.u.essid.length = IW_ESSID_MAX_SIZE+1;
		snprintf(wreq.ifr_name, sizeof(wreq.ifr_name), "%s", iface);

		if (sockfd < 0) {
			fprintf(stderr, "socket 'AF_INET': %s\n",
			        strerror(errno));
			return NULL;
		}
		wreq.u.essid.pointer = id;
		if (ioctl(sockfd,SIOCGIWESSID, &wreq) < 0) {
			fprintf(stderr, "ioctl 'SIOCGIWESSID': %s\n", strerror(errno));
			close(sockfd);
			return NULL;
		}

		close(sockfd);

		if (!strcmp(id, "")) {
			return NULL;
		}

		return id;
	}
#elif defined(__OpenBSD__)
	/* unimplemented */
#endif

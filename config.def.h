/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
static const int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/*
 * function             description                     argument (example)
 *
 * battery_perc         battery percentage              battery name (BAT0)
 * battery_state        battery charging state          battery name (BAT0)
 * cpu_perc             cpu usage in percent            NULL
 * cpu_freq             cpu frequency in MHz            NULL
 * datetime             date and time                   format string (%F %T)
 * disk_free            free disk space in GB           mountpoint path (/)
 * disk_perc            disk usage in percent           mountpoint path (/)
 * disk_total           total disk space in GB          mountpoint path (/")
 * disk_used            used disk space in GB           mountpoint path (/)
 * entropy              available entropy               NULL
 * gid                  GID of current user             NULL
 * hostname             hostname                        NULL
 * ipv4                 IPv4 address                    interface name (eth0)
 * ipv6                 IPv6 address                    interface name (eth0)
 * kernel_release       `uname -r`                      NULL
 * keyboard_indicators  caps/num lock indicators        NULL
 * load_avg             load average                    format string
 *                                                      (%.2f %.2f %.2f)
 * num_files            number of files in a directory  path
 *                                                      (/home/foo/Inbox/cur)
 * ram_free             free memory in GB               NULL
 * ram_perc             memory usage in percent         NULL
 * ram_total            total memory size in GB         NULL
 * ram_used             used memory in GB               NULL
 * run_command          custom shell command            command (echo foo)
 * swap_free            free swap in GB                 NULL
 * swap_perc            swap usage in percent           NULL
 * swap_total           total swap size in GB           NULL
 * swap_used            used swap in GB                 NULL
 * temp                 temperature in degree celsius   sensor file
 *                                                      (/sys/class/thermal/...)
 * uid                  UID of current user             NULL
 * uptime               system uptime                   NULL
 * username             username of current user        NULL
 * vol_perc             OSS/ALSA volume in percent      mixer file (/dev/mixer)
 * wifi_perc            WiFi signal in percent          interface name (wlan0)
 * wifi_essid           WiFi ESSID                      interface name (wlan0)
 */
static const struct arg args[] = {
	/* function format          argument */
	{ temp, "[t %s°C]", "/sys/class/thermal/thermal_zone0/temp"    },
	{ cpu_perc, "[C %s%%]", NULL    },
	{ ram_perc, "[M %s%%]", NULL    },
	{ battery_perc, "[B %s%%]", "BAT1"    },
	{ datetime, "[%s]", "%a, %b %d %H:%M" },
};

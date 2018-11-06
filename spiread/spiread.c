#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <time.h>
#include <sched.h>
#include <string.h>

#define PARPORT "/dev/parport0"

#define ioctl_assert(x,y,z) do{if(ioctl(x,y,z)==-1){perror(#y);exit(1);}}while(0)

int main(int argc, char **argv)
{
	struct timespec ts;
	int val, fd;
	struct sched_param p;

	memset(&p, 0, sizeof(p));
	p.sched_priority = 1;
	sched_setscheduler(0, SCHED_RR, &p);

	if ((fd = open(PARPORT, O_RDWR)) == -1) {
		perror(PARPORT);
		return 1;
	}

	ts.tv_sec = 0;
	ts.tv_nsec = 5000;

	ioctl_assert(fd, PPCLAIM, 0);;

	val = 0x80;
	ioctl_assert(fd, PPWDATA, &val);

	printf("\nReading SPI from STK200...\n");

	for (;;) {
		unsigned char x;
		int i;

		val = 0;
		ioctl_assert(fd, PPRSTATUS, &val);

		if (!(val & 0x40)) {
			usleep(10000);
			continue;
		}

		for (i = 0, x = 0; i < 8; i++) {
			val = 0x80 | 0x10;
			ioctl_assert(fd, PPWDATA, &val);

			nanosleep(&ts, NULL);

			val = 0;
			ioctl_assert(fd, PPRSTATUS, &val);
			if ((val & 0x40))
				x |= (128 >> i);
			
			val = 0x80;
			ioctl_assert(fd, PPWDATA, &val);

			nanosleep(&ts, NULL);
		}

		printf("%c", x);
		fflush(stdout);

		usleep(10000);
	}
}

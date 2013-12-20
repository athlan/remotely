#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <curses.h>

#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)

void getCurrentResolution(int *dim)
{
	char bash_cmd[] = "xdpyinfo -display :0 | sed 's/^ *dimensions: *\\([0-9x]*\\).*/\\1/;t;d'";
	char buffer[100];
	FILE *pipe;
	char xRes[5] = {0}; 
	char yRes[5] = {0};
	char *currentRes = xRes;
	int j = 0, i = 0;
	pipe = popen(bash_cmd, "r");
	if (NULL == pipe) {
		perror("pipe");
		exit(1);
	}

	while (fgets(buffer, 100, pipe) != NULL);

	for (i = 0; i < strlen(buffer) - 1; i++) {
		if (buffer[i] != 'x')
		{
			currentRes[j] = buffer[i];
			j++;
		}
		else {
			currentRes = yRes;
			j = 0;
		}
	}
	dim[0] = atoi(xRes);
	dim[1] = atoi(yRes);
}

int
main(void)
{
    int                    fd;
    struct uinput_user_dev uidev;
    int                    i,j;
    char buffer[255] = {0};

    int dim[2];

    getCurrentResolution(dim);

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if(fd < 0)
        die("error: open");

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl");
    for (j = 1 ; j < KEY_MAX ; j++)
    {
        if(ioctl(fd, UI_SET_KEYBIT,j) < 0)
         die("error: ioctl");
    }

    if(ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_ABSBIT, ABS_X) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_ABSBIT, ABS_Y) < 0)
        die("error: ioctl");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;
    uidev.absmin[ABS_X] = 0;
    uidev.absmax[ABS_X] = dim[0];
    uidev.absmin[ABS_Y] = 0;
    uidev.absmax[ABS_Y] = dim[1];

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

    sleep(2);
    initscr();

    printf("Dimensions: %d, %d", dim[0], dim[1]);
    printf("Uinput handler prepared waiting for data...");
   fflush(stdout); 
    struct input_event ev;
    struct input_event synEv;
    while(1)
    {
	double xTmp, yTmp;
	char buffer[255];

	read(STDIN_FILENO, buffer, sizeof(buffer));

	int result = sscanf(buffer, "x:%lf:y:%lf;", &xTmp, &yTmp);
        int x = xTmp * dim[0];
	int y = yTmp * dim[1];

memset(&ev, 0, sizeof(ev));
        ev.type = EV_ABS;
	ev.code = ABS_X;
	ev.value = x;

	if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write");    
      
        memset(&synEv, 0, sizeof(struct input_event));
        synEv.type = EV_SYN;
        synEv.code = 0;
        synEv.value = 0;
        if(write(fd, &synEv, sizeof(struct input_event)) < 0)
            die("error: write");
  
	memset(&ev, 0, sizeof(ev));
        ev.type = EV_ABS;
	ev.code = ABS_Y;
	ev.value = y;

	if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write");    
     memset(&synEv, 0, sizeof(struct input_event));
        synEv.type = EV_SYN;
        synEv.code = 0;
        synEv.value = 0;
        if(write(fd, &synEv, sizeof(struct input_event)) < 0)
            die("error: write");
  

    }
    endwin();

    sleep(2);

    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl");

    close(fd);

    return 0;
}
 


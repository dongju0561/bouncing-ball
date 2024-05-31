#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

typedef struct dev_fb_t
{
	int fbfd;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize;
	char *fbp;
}dev_fb;

typedef struct pixel_t
{
	int x;
	int y;
}pixel;

typedef struct {
	int speed;
    int dx; //case: -1, 0, 1
    int dy; //case: -1, 0, 1
} Speed;

typedef struct
{
	pixel pos;
	Speed speed;
}Ball;



#endif
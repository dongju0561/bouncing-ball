
#ifndef __FBDRAW_H__
#define __FBDRAW_H__

#include"structure.h"

#define FB_OPEN_FAIL 1
#define FB_GET_FINFO_FAIL 2
#define FB_GET_VINFO_FAIL 3
#define FB_MMAP_FAIL 4
#define DIAMETER 10
typedef unsigned char uchar;

int fb_init(dev_fb* fb);
pixel fb_toPixel(int x,int y);

void fb_drawPixelPx(dev_fb* fb, pixel px, char r, char g, char b);
void fb_drawPixel(dev_fb* fb, int x, int y, char r, char g, char b);
void fb_drawPixelwithAlpha(dev_fb* fb, int x, int y, char r, char g, char b, char a);
void fb_fillScr(dev_fb* fb, char r, char g, char b);

void fb_drawBox(dev_fb* fb, pixel px, int w, int h, char r, char g, char b);
void fb_drawBoxWidthAlpa(dev_fb* fb, pixel px, int w, int h, char r, char g, char b, char a);
void fb_fillBox(dev_fb* fb, pixel px, int w, int h, char r, char g, char b);
void fb_drawLine(dev_fb* fb, pixel start, pixel end, char r, char g, char b);

void fb_drawChar(dev_fb* fb, char c, pixel start, short height, char r, char g, char b);
void fb_printStr(dev_fb* fb, const char* str, pixel* cursor, short height, char r, char g, char b);

void fb_drawFilledCircle(dev_fb* fb, pixel center,char r, char g, char b);


void fb_close(dev_fb* fb);

#endif

#include "makise_primitives.h"

#define MAX_POLY_CORNERS   200





//**************************************************************************************
void makise_d_clear(MakiseBuffer* b, uint32_t c)
{
    uint8_t k = 0;
    for (int i = 0; i < (8/b->pixeldepth); i++)
    {
	k |= (c & b->depthmask) << i*b->pixeldepth;
    }
    memset(b->buffer, k, b->size);
}

void makise_d_point(MakiseBuffer* b, uint16_t x, uint16_t y, uint32_t c)
{
    makise_pset(b, x, y, c);
}
void makise_d_rect(MakiseBuffer* b, int16_t x, int16_t y, uint16_t w,
		   uint16_t h, uint32_t c)
{
    if(x >= b->width || y >= b->height)
	return;
    if(x < 0)
    {
	if(-x > w)
	    return;
	w = w + x;
	x = 0;
    }
    if(y < 0)
    {
	if(-y > h)
	    return;
	h = h + y;
	y = 0;
    }
    if(x + w >= b->width)
	w = b->width - x - 1;
    if(y + h >= b->height)
	h = b->height - y - 1;
    
    if(w == 0 || h == 0)
	return;

    uint32_t k, kb, j;

    uint8_t C = 0; //byte filled with colors for memset
    for (int i = 0; i < (8/b->pixeldepth); i++)
    {
	C |= (c & b->depthmask) << i*b->pixeldepth;
    }

    w-=1;
    h-=1;
    
    makise_d_line(b, x, y, x + w, y, c);
    makise_d_line(b, x, y + h, x + w, y + h, c);
    makise_d_line(b, x, y, x, y + h, c);
    makise_d_line(b, x + w, y, x + w, y + h, c);
    /*
      if depth = 4bits
      byte 1       byte 2
      coordinates |1,0   0,0| |3,0   2,0|
      binaryvalue |0000 0000| |0100 0101|
    */
}
void makise_d_rect_filled(MakiseBuffer* b, int16_t x, int16_t y, uint16_t w,
			  uint16_t h, uint32_t c, uint32_t fill_c)
{
    if(x >= b->width || y >= b->height)
	return;
    if(x < 0)
    {
	if(-x > w)
	    return;
	w = w + x;
	x = 0;
    }
    if(y < 0)
    {
	if(-y > h)
	    return;
	h = h + y;
	y = 0;
    }
    if(x + w >= b->width)
	w = b->width - x - 1;
    if(y + h >= b->height)
	h = b->height - y - 1;
    
    if(w == 0 || h == 0)
	return;

    uint32_t k, kb, j;

    uint8_t C = 0; //byte filled with colors for memset
    for (int i = 0; i < (8/b->pixeldepth); i++)
    {
	C |= (c & b->depthmask) << i*b->pixeldepth;
    }

    h -= 1;
    w -= 1;
    
    makise_d_line(b, x, y, x + w, y, c);
    makise_d_line(b, x, y + h, x + w, y + h, c);
    makise_d_line(b, x, y, x, y + h, c);
    makise_d_line(b, x + w, y, x + w, y + h, c);
    /*
      if depth = 4bits
      byte 1       byte 2
      coordinates |1,0   0,0| |3,0   2,0|
      binaryvalue |0000 0000| |0100 0101|
    */
    for (int i = y+1; i < h + y; i++) {
	makise_d_line(b, x + 1, i, x+w - 1, i, fill_c);
    }    
}
void makise_d_circle(MakiseBuffer* b, uint16_t xc, uint16_t yc, uint16_t r, uint32_t c)
{
    //middle point algorythm
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    makise_pset(b, xc, yc + r, c);
    makise_pset(b, xc, yc - r, c);
    makise_pset(b, xc + r, yc, c);
    makise_pset(b, xc - r, yc, c);

    while(x < y)
    {
	// ddF_x == 2 * x + 1;
	// ddF_y == -2 * y;
	// f == x*x + y*y - r*r + 2*x - y + 1;
	if(f >= 0)
	{
	    y--;
	    ddF_y += 2;
	    f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;
	makise_pset(b, xc + x, yc + y, c);
	makise_pset(b, xc - x, yc + y, c);
	makise_pset(b, xc + x, yc - y, c);
	makise_pset(b, xc - x, yc - y, c);
	makise_pset(b, xc + y, yc + x, c);
	makise_pset(b, xc - y, yc + x, c);
	makise_pset(b, xc + y, yc - x, c);
	makise_pset(b, xc - y, yc - x, c);
    }
    
}
void makise_d_circle_filled(MakiseBuffer* b, uint16_t xc, uint16_t yc, uint16_t r, uint32_t c, uint32_t fill_c)
{
    //middle point algorythm
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t dd;
    makise_d_line(b, xc - r, yc, xc + r, yc, fill_c);
    makise_pset(b, xc, yc + r, c);
    makise_pset(b, xc, yc - r, c);
    makise_pset(b, xc + r, yc, c);
    makise_pset(b, xc - r, yc, c);

    while(x < y)
    {
	// ddF_x == 2 * x + 1;
	// ddF_y == -2 * y;
	// f == x*x + y*y - r*r + 2*x - y + 1;
	if(f >= 0)
	{
	    y--;
	    ddF_y += 2;
	    f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;

	if(y != r)
	{
	    makise_d_line(b, xc - x + 1, yc + y, xc + x - 1, yc + y, fill_c);
	    makise_d_line(b, xc - x + 1, yc - y, xc + x - 1, yc - y, fill_c);
	}
	if(x != r)
	{
	    makise_d_line(b, xc - y + 1, yc + x, xc + y - 1, yc + x, fill_c);
	    makise_d_line(b, xc - y + 1, yc - x, xc + y - 1, yc - x, fill_c);
	}
	makise_pset(b, xc + x, yc + y, c);
	makise_pset(b, xc - x, yc + y, c);
	makise_pset(b, xc + x, yc - y, c);
	makise_pset(b, xc - x, yc - y, c);
	makise_pset(b, xc + y, yc + x, c);
	makise_pset(b, xc - y, yc + x, c);
	makise_pset(b, xc + y, yc - x, c);
	makise_pset(b, xc - y, yc - x, c);
    }
    
}
void makise_d_line(MakiseBuffer* b, int16_t x0, int16_t y0,
		   int16_t x1, int16_t y1, uint32_t c)
{
    if(x1 < 0 && x0 < 0)
	return;
    if(x1 >= b->width && x0 >= b->width)
	return;
    if(y1 < 0 && y0 < 0)
	return;
    if(y1 >= b->height && y0 >= b->height)
	return;
    x1 = x1 < 0 ? 0 : x1;
    y1 = y1 < 0 ? 0 : y1;
    x0 = x0 < 0 ? 0 : x0;
    y0 = y0 < 0 ? 0 : y0;
    
    x0 =  x0 < b->width-1 ? x0 : b->width - 1;
    x1 =  x1 < b->width-1 ? x1 : b->width - 1;
    y0 =  y0 < b->height-1 ? y0 : b->height - 1;
    y1 =  y1 < b->height-1 ? y1 : b->height - 1;
    
    int16_t dy = 0;
    
    dy = abs(y0 - y1);				/* The difference between the y's */
    
    if(dy == 0) //if line is horizontal we can memset it
    {
	uint32_t C = 0; //byte filled with colors for memset
	uint32_t kb, j;
	    
	for (int i = 0; i < (32/b->pixeldepth); i++)
	{
	    C |= (c & b->depthmask) << i*b->pixeldepth; 
	}

	j = x0;
	while ((j % (32 / b->pixeldepth)) && j <= x1) { //coordinate position in the array must in the beginning of the byte
	    makise_pset(b, j, y1, c);
	    j++;
	}

	
	kb = (y0*(b->width) + j) * (b)->pixeldepth/32;
	while(j <= x1 && x1 - j >= 32/b->pixeldepth)
	{
	    b->buffer[kb] = C;
	    kb += 1;
	    j += 32/b->pixeldepth;
	}
	j-=1;
	while (j <= x1) {
	    makise_pset(b, j, y0, c);
	    j++;
	}
	return;
    }

    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;
 
    for(;;){
	makise_pset(b, x0, y0, c);
	if (x0==x1 && y0==y1) break;
	e2 = err;
	if (e2 >-dx) { err -= dy; x0 += sx; }
	if (e2 < dy) { err += dx; y0 += sy; }
    }
}
void makise_d_polyline(MakiseBuffer*b, MakisePoint* points, uint32_t count, uint32_t c)
{
    if(count == 0 || points == 0)
	return;

    uint32_t i = 0;

    for(i = 1; i < count; i++)
    {
	makise_d_line(b, points[i - 1].x, points[i - 1].y, points[i].x, points[i].y, c);
    }
}

void makise_dex_polyline(MakiseBuffer*b, int32_t x, int32_t y, double rot, MakisePoint* points, uint32_t count, uint32_t c)
{
    if(count == 0 || points == 0)
	return;

    if(x == 0 && y == 0 && rot == 0)
	makise_d_polyline(b, points, count, c);
    
    uint32_t i = 0;
    float co = cos(rot), si = sin(rot);
    float lx, ly, cx, cy;

    lx = (points[0].x) * co - (points[0].y) * si + x;
    ly = (points[0].x) * si + (points[0].y) * co + y;
    for(i = 1; i < count; i++)
    {
	cx = (points[i].x) * co - (points[i].y) * si + x;
	cy = (points[i].x) * si + (points[i].y) * co + y;

	makise_d_line(b, lx , ly, cx, cy, c);
	lx = cx;
	ly = cy;
    }
    
}
#include "makise.h"

uint32_t makise_init(MakiseGUI * gui, MakiseDriver* driver, MakiseBuffer* buffer)
{
    gui->driver = driver;
    gui->buffer = buffer;
    gui->draw = 0;
    buffer->height = driver->lcd_height;
    buffer->width = driver->lcd_width;
    driver->gui = gui;
    buffer->gui = gui;
    
    buffer->depthmask = MAKISEGUI_BUFFER_DEPTHMASK;
    buffer->pixeldepth = MAKISEGUI_BUFFER_DEPTH;
    buffer->width = driver->lcd_width;
    buffer->height = driver->lcd_height;
    
    //buffer len in bits
    uint32_t len = buffer->height * buffer->width * buffer->pixeldepth;
    //count in bytes
    uint32_t lenb = len/8;
    if(lenb * 8 < len)
	lenb += 1;

    buffer->size = lenb;
    
    return lenb;
}
uint8_t makise_start(MakiseGUI * gui)
{
    assert_param(gui == 0);
    assert_param(gui->driver == 0);

    gui->driver->start(gui);
}

uint32_t kpset, kpset32, kpsett;
inline uint32_t makise_pget(MakiseBuffer *b, uint16_t x, uint16_t y)
{
    if((x) < b->width && (y) < (b)->height)		
    {							
	kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
	kpset32 = kpset/32;
	return (b)->depthmask & ((b)->buffer[kpset32] >> (kpset-(kpset32)*32));
    }						
    return 0;
}

inline void makise_pset(MakiseBuffer *b, uint16_t x, uint16_t y, uint32_t c)
{
    if((x) < (b)->width && (y) < (b)->height)
	
    {									
	kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
	kpset32 = kpset/32;
	kpsett = kpset - kpset32*32;

	if(((b)->depthmask & ((b)->buffer[kpset32] >> (kpsett))) == c)
	    return;
	
	(b)->buffer[kpset32] = ((b)->buffer[kpset32] & ~((b)->depthmask << kpsett)) | (c << kpsett); 
    }
}
//if partial_render = 0, then entire buffer will be rendered, if == 1, then will be rendered only first part, if == 2 then will be rendered second part
void makise_render(MakiseGUI *gui, uint8_t partial_render)
{
    MakiseDriver * d = gui->driver;
    uint16_t c;

    uint32_t y = d->posy, //start position y
	x = 0,
	i = 0, //pixel in drivers's buffer
	cu,
	bu = 0,
	m; //end position y

    if(partial_render == 0) //render full buffer
    {
	m = d->posy + d->buffer_height;
	d->posy += d->buffer_height;
	partial_render = 1;
    }
    else if(partial_render == 1) //render first half (called by halfcplt callback)
    {
	m = d->posy + (d->buffer_height / 2);
	y = d->posy;
    }
    else if(partial_render == 2) //render second half (called by cplt callback)
    {
	m = d->posy + d->buffer_height;
	y = d->posy + (d->buffer_height / 2);
	i = d->size / 4;
	d->posy += d->buffer_height;
    }

    cu = (y * gui->buffer->width) * gui->buffer->pixeldepth/32;
    for (; y < m; y++) {	
	for (x = 0; x < d->buffer_width; x+=1)
	{
	    c = makise_color_get(((((uint32_t*)gui->buffer->buffer)[cu]) >> bu) & gui->buffer->depthmask);
	    bu += gui->buffer->pixeldepth;
	    if(bu>=32)
	    {
		bu = 0;
		cu ++;
	    }
	    //c+=bc;
	    ((uint16_t*)d->buffer)[i] = c;
	    i+=1;
	}
    }
    
}
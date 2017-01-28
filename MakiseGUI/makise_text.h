#ifndef MAKISE_TEXT_H
#define MAKISE_TEXT_H 1

#include <string.h>
#include "makise.h"


#define MDTextLenAuto UINT32_MAX 

typedef struct _MakiseFont MakiseFont;
typedef enum
{
    MDTextPlacement_LeftUp = 0,
    MDTextPlacement_Center,
    MDTextPlacement_CenterUp,
    MDTextPlacement_CenterDown,
} MDTextPlacement;

#include "fonts.h"

typedef struct _MakiseFont
{
    const uint8_t * table;
    uint16_t width;
    uint16_t height;
    const uint8_t * char_width;
    const uint16_t * char_index;
    uint16_t offset;
    uint16_t num_char;
    uint16_t space_char;    
} MakiseFont;

void makise_d_char(MakiseBuffer *b, uint16_t ch, uint16_t x, uint16_t y, const MakiseFont *font, uint32_t c); //draw single char
void makise_d_string(MakiseBuffer *b, char *s, uint32_t len, uint16_t x, uint16_t y, MDTextPlacement place, const MakiseFont *font, uint32_t c); //draw string
uint32_t makise_d_string_width(char *s, uint32_t len, const MakiseFont *font); //get width of string being drew using that font


#endif
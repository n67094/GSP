#ifndef CORE_FONT_H
#define CORE_FONT_H

#include "types.h"

#define TILE_FONT_ALPHA 97
#define TILE_FONT_DIGITS 129

static const LabelFont default_font = {
	.glyphs = (LabelGlyph[]){
		{.ascii_code  = 'a', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA},
		{.ascii_code  = 'b', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 1},
		{.ascii_code  = 'c', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 2},
		{.ascii_code  = 'd', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 3},
		{.ascii_code  = 'e', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 4},
		{.ascii_code  = 'f', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 5},
		{.ascii_code  = 'g', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 6},
		{.ascii_code  = 'h', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 7},
		{.ascii_code  = 'i', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 8},
		{.ascii_code  = 'j', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 9},
		{.ascii_code  = 'k', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 10},
		{.ascii_code  = 'l', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 11},
		{.ascii_code  = 'm', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 12},
		{.ascii_code  = 'n', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 13},
		{.ascii_code  = 'o', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 14},
		{.ascii_code  = 'p', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 15},
		{.ascii_code  = 'q', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 16},
		{.ascii_code  = 'r', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 17},
		{.ascii_code  = 's', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 18},
		{.ascii_code  = 't', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 19},
		{.ascii_code  = 'u', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 20},
		{.ascii_code  = 'v', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 21},
		{.ascii_code  = 'w', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 22},
		{.ascii_code  = 'x', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 23},
		{.ascii_code  = 'y', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 24},
		{.ascii_code  = 'z', .height = 5, .width = 6, .tile_id = TILE_FONT_ALPHA + 25},
		{.ascii_code  = '0', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS},
		{.ascii_code  = '1', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 1},
		{.ascii_code  = '2', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 2},
		{.ascii_code  = '3', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 3},
		{.ascii_code  = '4', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 4},
		{.ascii_code  = '5', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 5},
		{.ascii_code  = '6', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 6},
		{.ascii_code  = '7', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 7},
		{.ascii_code  = '8', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 8},
		{.ascii_code  = '9', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 9},
		{.ascii_code  = ':', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 10},
		{.ascii_code  = '.', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 11},
		{.ascii_code  = ' ', .height = 5, .width = 6, .tile_id = TILE_FONT_DIGITS + 12},
	},
	.start_alpha_id = 0,
	.start_digit_id = 26,
	.start_special_id = 36,
	.size = 39
};

void FontInit(void);

#endif

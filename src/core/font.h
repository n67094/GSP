#ifndef CORE_FONT_H
#define CORE_FONT_H

#include "types.h"

static const LabelFont default_font = {
	.glyphs = (LabelGlyph[]){
		{.ascii_code  = 'a', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'b', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'c', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'd', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'e', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'f', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'g', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'h', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'i', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'j', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'k', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'l', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'm', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'n', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'o', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'p', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'q', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'r', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 's', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 't', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'u', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'v', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'w', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'x', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'y', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = 'z', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '0', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '1', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '2', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '3', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '4', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '5', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '6', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '7', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '8', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '9', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = ':', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = '.', .height = 5, .width = 6, .tile_id = 1},
		{.ascii_code  = ' ', .height = 5, .width = 6, .tile_id = 1},
	},
	.start_alpha_id = 0,
	.start_digit_id = 26,
	.start_special_id = 36,
	.size = 39
}

#endif

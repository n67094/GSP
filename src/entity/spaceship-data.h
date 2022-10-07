#ifndef ENTITY_SPACESHIP_H
#define ENTITY_SPACESHIP_H

#include "../../data/bitmaps/capsule.tiles.h"
#include "../../data/bitmaps/wall.tiles.h"

#include "../types.h"

const SegmentData capsule_wall = {
	.type = 0x2, .radius_1 = 32, .radius_2 = 16, .height = -32,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = capsule_tiles
};

/*
const SegmentData capsule_segments[] = {capsule_wall};

const PartData capsule = {
	.num_segments = 1, .length = 32, .segments_ptr = capsule_segments
};
*/

#endif

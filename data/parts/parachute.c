#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData parachute_wall = {
	.type = 0x2, .radius_1 = 16, .radius_2 = 0, .height = -8,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = wall_tiles
};

const SegmentData parachute_bottom_base = {
	.type = 0x1, .radius_1 = 16, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};


const SegmentData parachute_segments[] = {parachute_wall, parachute_bottom_base};

const PartData parachute = {
	.num_segments = 2, .length = 8, .segments_ptr = parachute_segments
};
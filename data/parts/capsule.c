#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData capsule_wall = {
	.type = 0x2, .radius_1 = 32, .radius_2 = 16, .height = -32,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = capsule_tiles
};


const SegmentData capsule_segments[] = {capsule_wall};

const PartData capsule = {
	.num_segments = 1, .length = 32, .segments_ptr = capsule_segments
};
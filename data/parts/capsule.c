#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData capsule_top_base = {
	.type = 0x1, .radius_1 = 16, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData capsule_wall = {
	.type = 0x2, .radius_1 = 32, .radius_2 = 16, .height = -32,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = capsuleBitmap
};

const SegmentData capsule_bottom_base = {
	.type = 0x1, .radius_1 = 32, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};


const SegmentData capsule_segments[] = {capsule_top_base, capsule_wall, capsule_bottom_base};

const PartData capsule = {
	.num_segments = 3, .length = 32, .segments_ptr = capsule_segments
};

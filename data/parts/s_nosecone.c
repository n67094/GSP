#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData s_nosecone_wall = {
	.type = 0x2, .radius_1 = 16, .radius_2 = 0, .height = -16,
	.gfx_width = 5, .gfx_height = 6, .gfx_data = noseconeBitmap
};

const SegmentData s_nosecone_bottom_base = {
	.type = 0x1, .radius_1 = 16, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};


const SegmentData s_nosecone_segments[] = {s_nosecone_wall, s_nosecone_bottom_base};

const PartData s_nosecone = {
	.num_segments = 2, .length = 16, .segments_ptr = s_nosecone_segments
};

#include "../../types.h"
#include "part_textures.h"

const SegmentData Capsule_wall = {
	.type = 0x2, .radius_1 = 32, .radius_2 = 16, .height = -32,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = Capsule_TextureBitmap
};

const SegmentData Capsule_segments[] = {Capsule_wall};

const PartData Capsule = {
	.num_segments = 1, .length = 32, .segments_ptr = Capsule_segments
};
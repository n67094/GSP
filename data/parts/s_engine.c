#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData s_engine_top_base = {
	.type = 0x1, .radius_1 = 16, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData s_engine_wall_1 = {
	.type = 0x2, .radius_1 = 16, .radius_2 = 0, .height = 8,
	.gfx_width = 5, .gfx_height = 7, .gfx_data = m_engineBitmap
};

const SegmentData s_engine_wall_2 = {
	.type = 0x2, .radius_1 = 8, .radius_2 = 0, .height = -16,
	.gfx_width = 2, .gfx_height = 5, .gfx_data = m_engine_2Bitmap
};

const SegmentData s_engine_segments[] = {s_engine_top_base, s_engine_wall_1, s_engine_wall_2};

const PartData s_engine = {
	.num_segments = 3, .length = 24, .segments_ptr = s_engine_segments
};

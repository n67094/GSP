#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData s_engine_exhaust_wall = {
	.type = 0x2, .radius_1 = 8, .radius_2 = 0, .height = 32,
	.gfx_width = 5, .gfx_height = 6, .gfx_data = m_exhaustBitmap
};

const SegmentData s_engine_exhaust_segments[] = {s_engine_exhaust_wall};

const PartData s_engine_exhaust = {
	.num_segments = 1, .length = 32, .segments_ptr = s_engine_exhaust_segments
};

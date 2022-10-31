#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData m_engine_top_base = {
	.type = 0x1, .radius_1 = 32, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData m_engine_wall_1 = {
	.type = 0x2, .radius_1 = 32, .radius_2 = 0, .height = 16,
	.gfx_width = 5, .gfx_height = 7, .gfx_data = m_engineBitmap
};

const SegmentData m_engine_wall_2 = {
	.type = 0xa, .radius_1 = 8, .radius_2 = 0, .height = -16,
	.gfx_width = 2, .gfx_height = 5, .gfx_data = m_engine_2Bitmap
};

const SegmentData m_engine_segments[] = {m_engine_top_base, m_engine_wall_1, m_engine_wall_2};

const PartData m_engine = {
	.num_segments = 3, .length = 32, .segments_ptr = m_engine_segments
};

#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData m_engine_exhaust_wall = {
	.type = 0x2, .radius_1 = 8, .radius_2 = 0, .height = 64,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = wall_tiles
};

const SegmentData m_engine_exhaust_segments[] = {m_engine_exhaust_wall};

const PartData m_engine_exhaust = {
	.num_segments = 1, .length = 24, .segments_ptr = m_engine_exhaust_segments
};
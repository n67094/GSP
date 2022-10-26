#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData m_fuel_tank_top_base = {
	.type = 0x1, .radius_1 = 32, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData m_fuel_tank_wall = {
	.type = 0x0, .radius_1 = 32, .radius_2 = 32, .height = 64,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = m_fuel_tankBitmap
};

const SegmentData m_fuel_tank_bottom_base = {
	.type = 0x1, .radius_1 = 32, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData m_fuel_tank_wall_segments[] = {m_fuel_tank_top_base, m_fuel_tank_wall, m_fuel_tank_bottom_base};

const PartData m_fuel_tank = {
	.num_segments = 3, .length = 64, .segments_ptr = m_fuel_tank_wall_segments
};

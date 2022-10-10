#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData m_fuel_tank_wall = {
	.type = 0x0, .radius_1 = 32, .radius_2 = 32, .height = 64,
	.gfx_width = 6, .gfx_height = 7, .gfx_data = wall_tiles
};

const SegmentData m_fuel_tank_wall_segments[] = {m_fuel_tank_wall};

const PartData m_fuel_tank = {
	.num_segments = 1, .length = 64, .segments_ptr = m_fuel_tank_wall_segments
};
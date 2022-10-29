#include "../../src/types.h"
#include "../../src/entity/parts-data.h"

const SegmentData s_fuel_tank_top_base = {
	.type = 0x1, .radius_1 = 16, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData s_fuel_tank_wall = {
	.type = 0x0, .radius_1 = 16, .radius_2 = 16, .height = 32,
	.gfx_width = 6, .gfx_height = 6, .gfx_data = s_fuel_tankBitmap
};

const SegmentData s_fuel_tank_bottos_base = {
	.type = 0x1, .radius_1 = 16, .radius_2 = 0, .height = 0,
	.gfx_width = 6, .gfx_height = 0, .gfx_data = test_baseBitmap
};

const SegmentData s_fuel_tank_wall_segments[] = {s_fuel_tank_top_base, s_fuel_tank_wall, s_fuel_tank_bottos_base};

const PartData s_fuel_tank = {
	.num_segments = 3, .length = 32, .segments_ptr = s_fuel_tank_wall_segments
};

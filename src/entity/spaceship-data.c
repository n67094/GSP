#include "../types.h"
#include "parts-data.h"

const PartData *center_column_parts[] = {&parachute, &capsule, &m_fuel_tank, &m_engine, &m_engine_exhaust};

ColumnData center_column = {
	.num_parts = 5, .parts_ptr = center_column_parts,
	.length = 200, .x_pos = 0, .z_pos = 0, .y_pos = 100
};
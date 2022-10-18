#include "../types.h"
#include "parts-data.h"

const PartData *center_column_parts[] = {&parachute, &capsule, &m_fuel_tank, &m_engine, &m_engine_exhaust};

ColumnData center_column = {
	.num_parts = 5, .parts_ptr = center_column_parts,
	.length = 200, .radius = 0, .angle = 0, .y_pos = 100
};

const PartData *side_column_parts[] = {&s_nosecone, &s_fuel_tank, &s_engine, &s_engine_exhaust};

ColumnData side_column_1 = {
	.num_parts = 4, .parts_ptr = side_column_parts,
	.length = 104, .radius = 48, .angle = 0, .y_pos = 40
};

ColumnData side_column_2 = {
	.num_parts = 4, .parts_ptr = side_column_parts,
	.length = 104, .radius = 48, .angle = 341, .y_pos = 40
};

ColumnData side_column_3 = {
	.num_parts = 4, .parts_ptr = side_column_parts,
	.length = 104, .radius = 48, .angle = 683, .y_pos = 40
};

ColumnData *ship_columns[] = {&center_column, &side_column_1, &side_column_2, &side_column_3};
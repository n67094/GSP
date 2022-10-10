#include "../types.h"
#include "parts-data.h"

const PartData *center_column_parts[] = {&capsule, &m_fuel_tank};

ColumnData center_column = {
	.num_parts = 2, .parts_ptr = center_column_parts
};
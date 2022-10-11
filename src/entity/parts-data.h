#ifndef ENTITY_SPACESHIP_H
#define ENTITY_SPACESHIP_H

#include "../types.h"

//texture bitmaps
extern const unsigned char capsule_tiles[16384];
extern const unsigned char wall_tiles[16384];
extern const unsigned char test_baseBitmap[4096];

//parts
extern const SegmentData capsule_segments[];
extern const PartData capsule;

extern const SegmentData m_fuel_tank_wall_segments[];
extern const PartData m_fuel_tank;

extern const SegmentData parachute_segments[];
extern const PartData parachute;

extern const SegmentData m_engine_segments[];
extern const PartData m_engine;

extern const SegmentData m_engine_exhaust_segments[];
extern const PartData m_engine_exhaust;


#endif

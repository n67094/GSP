#ifndef TYPES_H
#define TYPES_H

#include <seven/base/types.h>
#include <seven/video/object.h>

typedef struct Object Object;

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef const u8  cu8;
typedef const u16 cu16;
typedef const u32 cu32;
typedef const u64 cu64;

typedef const s8  cs8;
typedef const s16 cs16;
typedef const s32 cs32;
typedef const s64 cs64;

typedef struct SphereData {
	s32 pitch;
	s32 spin;
} SphereData;

//this is the data for one segment, a segment is a piece that uses a single texture. There are two main types of segments, walls, and bases.
//Walls are the parts that go around cylinders/cones. Bases are the flat part at the end of a cylinder/cone.
typedef struct SegmentData{
	cu8 type; //encodes various data about the segment so that the algorithm knows how to draw it.
	//bit 0: 0 = wall, 1 = base.
	//bit 1 if wall: 0 = cylinder, 1 = cone.
	//bit 1 if base: 0 = always draw, 1 = do not draw if segment is mated with another part.
	//bit 2: 0 = segment texture has no transparant pixels, 1 = segment texture has some transparant pixels.
	//bit 3: 0 = static dimensions, 1 = variable dimensions. For example, Engine exhaust will have variable height.
	//bit 4: reserved
	//bit 5: reserved
	//bit 6: reserved
	//bit 7: reserved
	cu8 radius_1; //the in-game radius of one side of this segment. (Top side, if cone)
	cu8 radius_2; //the in-game radius of the other side of this segment. Only used for cones.
	cu8 height; //the in-game height of this segment. Not used for base segments
	cu8 gfx_width; //the width of the texture in pixels.
	cs8 gfx_height; //the height of the texture in pixels. Negative means smaller radius is on top (cones only)
	cu8 *gfx_data; //pointer to the start of this segments texture.
} SegmentData;

//This is the data for one part. A rocket is built up from many of these pre-defined parts
typedef struct PartData{
	cu8 num_segments; //the number of segments needed to draw this part.
	cu8 length; //The length of this part from end to end, including all segments.
	const SegmentData *segments_ptr; //a pointer to an array of NumSegments SegmentData structs.
	//if this were to be developed into a full game, other const details about the part could go here, like mass, cost, etc.
} PartData;

//This is the data for one column of parts. A column is made up of many parts attached end-to-end.
typedef struct ColumnData{
	u8 bum_parts; //the number of parts that make up this column.
	PartData *Parts_ptr; //a pointer to an array of NumParts PartData structs.
	u32 length; //The total length of this column in in-game units, from tip to tail.
	s32 x_pos; //The X position of this column relative to the center column in in-game units, X is towards the camera when Spin and Pitch = 0.
	s32 z_pos; //The Z position of this column relative to the center column in in-game units, Z is towars the top of the screen when Spin and Pitch = 0.
	s32 y_pos; //The Y position of the top of this column relative to the top of the center column in in-game units.
} ColumnData;

//This is the data for the ship as a whole. The ship is made up of one central column, with potentially more columns attached radially.
typedef struct ShipData{
	u32 num_columns; //the number of columns that make up this ship.
	ColumnData *columns_ptr; //a pointer to an array of NumColumns ColumnData structs.
	s32 pitch; //the pitch of the ship, reletive to the camera, where 256 is pointing the front of the ship at the camera. Must be between -256 and 256.
	s32 spin; //the spin of the ship, (or roll), reletive to the camera. 
	u32 length; //The longest dimension of the ship in in-game units, used to make sure the entire ship can fit into the background layer.
} ShipData;

#endif

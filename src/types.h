#ifndef TYPES_H
#define TYPES_H

#include "./core/types.h"

typedef struct SphereData {
	s32 pitch;
	s32 spin;
	u32 buffer;
} SphereData;

typedef struct RotationMatrix {
	s32 X1; 
	s32 X2;
	s32 X3;
	s32 Y1;
	s32 Y2;
	s32 Y3;
	s32 Z1;
	s32 Z2;
	s32 Z3;
} RotationMatrix;

//this is the data for one segment, a segment is a piece that uses a single texture. There are two main types of segments, walls, and bases.
//Walls are the parts that go around cylinders/cones. Bases are the flat part at the end of a cylinder/cone.
typedef struct SegmentData{
	cu8 type; //encodes various data about the segment so that the algorithm knows how to draw it.
	//bit 0: 0 = wall, 1 = base.
	//bit 1 if wall: 0 = cylinder, 1 = cone.
	//bit 1 if base: 0 = do not draw if segment is mated with another part, 1 = always draw 
	//bit 2: 0 = static dimensions, 1 = engine exhaust
	//bit 3: 1 if engine bell
	//bit 4: unused
	//bit 5: unused
	//bit 6: unused
	//bit 7: unused
	cu8 radius_1; //the in-game radius of one side of this segment. (larger side, if cone)
	cu8 radius_2; //the in-game radius of the other side of this segment. Only used for cones.
	cs8 height; //the in-game height of this segment. Not used for base segments. Negative means smaller radius is on top (cones only)
	cu8 gfx_width; //the width of the texture in 2^n pixels.
	cs8 gfx_height; //the height of the texture in 2^n pixels.
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
	u8 num_parts; //the number of parts that make up this column.
	const PartData **parts_ptr; //a pointer to an array of NumParts PartData struct pointers. First part is on top
	u32 length; //The total length of this column in in-game units, from tip to tail.
	u32 radius; //The distance of the center axis of this column to the center axis of the spaceship
	u32 angle; //The angular position of this column around the center axis of the spaceship.
	s32 y_pos; //The Y position of the top of this column relative to the center of the spaceship
} ColumnData;

//This is the data for the ship as a whole. The ship is made up of one central column, with potentially more columns attached radially.
typedef struct ShipData{
	u32 num_columns; //the number of columns that make up this ship.
	ColumnData **columns_ptr; //a pointer to an array of NumColumns ColumnData structs.
	RotationMatrix *rotation_matrix; //x is along axis of ship, y is vertical when pitch and spin are 0, z is towards screen when pitch and spin is zero
	s32 roll_pos; //When value becomes greater than 256, rotation matrix is updated, and this value is reduced by 256
	s32 pitch_pos; //When value becomes greater than 256, rotation matrix is updated, and this value is reduced by 256
	s32 yaw_pos; //When value becomes greater than 256, rotation matrix is updated, and this value is reduced by 256
	s32 roll_vel; //how much the roll_pos changes per frame
	s32 pitch_vel; //how much the roll_pos changes per frame
	s32 yaw_vel; //how much the roll_pos changes per frame
	s32 alpha; //the rotation about the X axis
	s32 beta; //the rotation about the Y axis
	s32 gamma; //the rotation about the Z axis
	u32 length; //The longest dimension of the ship in in-game units, used to make sure the entire ship can fit into the background layer.
} ShipData;

typedef struct CameraData{
	RotationMatrix *rotation_matrix;
	s32 spin;
	s32 pitch;
	s32 zoom;
}CameraData;

#endif

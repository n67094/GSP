#ifndef CORE_LABEL_H
#define CORE_LABEL_H

#include "types.h"

/*
 * Print glyph as object/sprites. Glyph are stored in OVRAM
 */

void LabelInit(LabelFont *font);

/*
 * @text is the text to draw
 * @pos_x should be between 0 (left) and 240
 * @pos_x should be between 0 (top) and 160
 * @oam_start_id the OAM index where object will be inserted from 0 to 128
 * @oam_buffer the OAM buffer where to insert the  object
 * return the number of object draw which is equals at the number of slot taken on the oam_buffer
 * or zero if no font was initialized
 */
int LabelDraw(char *text, u32 pos_x, u32 pos_y, u32 oam_start_id, Object* oam_buffer, u32 palette_id);

#endif

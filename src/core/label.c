#include <seven/video/object.h>
#include <stdbool.h>

#include "../debug/log.h"

#include "seven/base/types.h"
#include "types.h"
#include "object.h"
#include "label.h"

static const LabelFont *text_object_current_font;

static void LabelSetObject(Object *object, u32 pos_x, u32 pos_y, u32 palette_id, u32 tile_id) {
  ObjectSetAttr(object,
    OBJ_SHAPE_SQUARE,
    OBJ_SIZE_8X8,
    OBJ_PALETTE_NUMBER(palette_id) | OBJ_TILE_NUMBER(tile_id)
  );
  ObjectSetPos(object, pos_x, pos_y);
}

static int LabelPrint(u8 id, u32*pos_x, u32 pos_y, Object* object, u32 palette_id) {
	LabelGlyph *glyph = &text_object_current_font->glyphs[id];

	u32 tile_id = glyph->tile_id;

	LabelSetObject(object, *pos_x, pos_y, palette_id, tile_id);

	*pos_x += glyph->width;

	return 1;
}

static int LabelPrintSpecial(char ascii, u32 *pos_x, u32 pos_y, Object* object, u32 palette_id)
{
	bool found = false;
	int special_id = text_object_current_font->start_special_id;
	while(special_id < text_object_current_font->size && found == false) {

		if(text_object_current_font->glyphs[special_id].ascii_code == ascii) {
			found = true;
		} else {
			++special_id;
		}
	}

	if(!found) return 0;

	LabelGlyph *glyph = &text_object_current_font->glyphs[special_id];

	u32 tile_id = glyph->tile_id;

	LabelSetObject(object, *pos_x, pos_y, palette_id, tile_id);

	*pos_x += glyph->width;
	
	return 1;
}

void LabelInit(const LabelFont *font)
{
	text_object_current_font = font;
}

int LabelDraw(char *text, u32 pos_x, u32 pos_y, u32 oam_start_id, Object* oam_buffer, u32 palette_id)
{
	if(text_object_current_font == NULL) return 0;

	u32 items_draw = 0;

	int i;
	for(i = 0; text[i] != '\0'; ++i) {
		Object *object = &oam_buffer[oam_start_id + i];
		
		if(text[i] >= 48 && text[i] <= 57) {
			items_draw += LabelPrint(text[i] - 48 + text_object_current_font->start_digit_id, &pos_x, pos_y, object, palette_id);
		} else if(text[i] >= 65 && text[i] <= 90) {
			items_draw += LabelPrint(text[i] - 65 + text_object_current_font->start_alpha_id, &pos_x, pos_y, object, palette_id);
		} else if (text[i] >= 97 && text[i] <= 122) {
			items_draw += LabelPrint(text[i] - 97 + text_object_current_font->start_alpha_id, &pos_x, pos_y, object, palette_id);
		} else if(text[i] == ' ' || text[i] == '.' || text[i] == ':') {
			items_draw += LabelPrintSpecial(text[i], &pos_x, pos_y, object, palette_id);
		}
	}

	return items_draw;
}


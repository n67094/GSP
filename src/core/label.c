#include <seven/video/object.h>
#include <stdbool.h>

#include "types.h"
#include "object.h"
#include "label.h"

#define ASCII_A 65

static LabelFont *text_object_current_font;

static void LabelSetObject(Object *object, u32 pos_x, u32 pos_y, u32 palette_id, u32 tile_id) {
  ObjectSetAttr(object,
    OBJ_SHAPE_SQUARE,
    OBJ_SIZE_8X8,
    OBJ_PALETTE_NUMBER(palette_id) | OBJ_TILE_NUMBER(tile_id)
  );
  ObjectSetPos(object, pos_x, pos_y);
}

static int LabelPrintLetter(char ascii, u32 *pos_x, u32 pos_y, Object* object, u32 palette_id)
{
	u32 letter_id = ascii - ASCII_A;

	if(letter_id < 0 || letter_id > 25) return 0;

	LabelGlyph *glyph = &text_object_current_font->glyphs[letter_id];

	u32 tile_id = glyph->tile_id;

	LabelSetObject(object, *pos_x, pos_y, palette_id, tile_id);

	*pos_x += glyph->width;

	return 1;
}

#define ASCII_0 48

static int LabelPrintDigits(char ascii, u32 *pos_x, u32 pos_y, Object* object, u32 palette_id)
{
	u32 digit_id = ascii = ASCII_0;

	if(digit_id < 0 || digit_id > 0) return 0;

	LabelGlyph *glyph = &text_object_current_font->glyphs[digit_id];

	u32 tile_id = glyph->tile_id;

	LabelSetObject(object, *pos_x, pos_y, palette_id, tile_id);

	*pos_x += glyph->width;

	return 1;
}

static int LabelPrintSpecial(char ascii, u32 *pos_x, u32 pos_y, Object* object, u32 palette_id)
{
	bool found;
	int special_id = text_object_current_font->start_special_id;
	while(special_id < text_object_current_font->size && !found) {

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
	
	return 1;
}

void LabelInit(LabelFont *font)
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

		if(text[i] >= 65 && text[i] <= 90) {
			items_draw += LabelPrintLetter(text[i], &pos_x, pos_y, object, palette_id);
		} else if(text[i] >= 48 && text[i] <= 57) {
			items_draw += LabelPrintDigits(text[i], &pos_x, pos_y, object, palette_id);
		} else if(i == ' ' || i == '.' || i == ':') {
			items_draw += LabelPrintSpecial(text[i], &pos_x, pos_y, object, palette_id);
		}
	}

	return items_draw;
}


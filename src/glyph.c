#include "glyph.h"

void tileset_initialize(
		Tileset *tileset, 
		const char* texture_path,
		int source_tile_width, 
		int source_tile_height,
		int target_tile_width,
		int target_tile_height,
		Color default_color
	)
{
	tileset->texture = LoadTexture(texture_path);
	tileset->source_tile_width = source_tile_width;
	tileset->source_tile_height = source_tile_height;
	tileset->target_tile_width = target_tile_width;
	tileset->target_tile_height = target_tile_height;
	tileset->default_color = default_color;
}

void tileset_free(Tileset *tileset)
{
	UnloadTexture(tileset->texture);
	free(tileset);
}


void glyph_initialize(
		Glyph *glyph, 
		Tileset *tileset, 
		int source_tile_x, 
		int source_tile_y
	)
{
	glyph->source_tile_x = source_tile_x;
	glyph->source_tile_y = source_tile_y;
	glyph->tileset = tileset;
}

void glyph_free(Glyph *glyph)
{
	free(glyph);
}

void glyph_draw(Glyph* glyph, int pos_x, int pos_y, Color color)
{
	float width = glyph->tileset->target_tile_width;
	float height = glyph->tileset->target_tile_height;

	DrawTexturePro(
		glyph->tileset->texture,
		(Rectangle) {
				glyph->source_tile_x * glyph->tileset->source_tile_width,
				glyph->source_tile_y * glyph->tileset->source_tile_height,
				glyph->tileset->source_tile_width,
				glyph->tileset->source_tile_height
			},
		(Rectangle) {
				pos_x, 
				pos_y, 
				width, 
				height
			},
		(Vector2) {
				-1, -1
				//width / 2, height / 2
			}, // origin
		0, // rotation
		color
	);
}

void glyph_draw_pro(Glyph *glyph, int pos_x, int pos_y, int width, int height, Color color)
{
	DrawTexturePro(
			glyph->tileset->texture,
			(Rectangle){ 
					glyph->source_tile_x * glyph->tileset->source_tile_width,
					glyph->source_tile_y * glyph->tileset->source_tile_height,
					glyph->tileset->source_tile_width,
					glyph->tileset->source_tile_height 
				},
			(Rectangle){ pos_x, pos_y, width, height },
			(Vector2){ pos_x + width / 2, pos_y + height / 2 }, // rotation origin
			0, // rotation
			color
		);
}

int glyph_get_idx_by_char(char ch) 
{
	switch(ch) {
		case 'A': return UPPER_A;
		case 'B': return UPPER_B;
		case 'C': return UPPER_C;
		case 'D': return UPPER_D;
		case 'E': return UPPER_E;
		case 'F': return UPPER_F;
		case 'G': return UPPER_G;
		case 'H': return UPPER_H;
		case 'I': return UPPER_I;
		case 'J': return UPPER_J;
		case 'K': return UPPER_K;
		case 'L': return UPPER_L;
		case 'M': return UPPER_M;
		case 'N': return UPPER_N;
		case 'O': return UPPER_O;
		case 'P': return UPPER_P;
		case 'Q': return UPPER_Q;
		case 'R': return UPPER_R;
		case 'S': return UPPER_S;
		case 'T': return UPPER_T;
		case 'U': return UPPER_U;
		case 'V': return UPPER_V;
		case 'W': return UPPER_W;
		case 'X': return UPPER_X;
		case 'Y': return UPPER_Y;
		case 'Z': return UPPER_Z;

		case 'a': return LOWER_A;
		case 'b': return LOWER_B;
		case 'c': return LOWER_C;
		case 'd': return LOWER_D;
		case 'e': return LOWER_E;
		case 'f': return LOWER_F;
		case 'g': return LOWER_G;
		case 'h': return LOWER_H;
		case 'i': return LOWER_I;
		case 'j': return LOWER_J;
		case 'k': return LOWER_K;
		case 'l': return LOWER_L;
		case 'm': return LOWER_M;
		case 'n': return LOWER_N;
		case 'o': return LOWER_O;
		case 'p': return LOWER_P;
		case 'q': return LOWER_Q;
		case 'r': return LOWER_R;
		case 's': return LOWER_S;
		case 't': return LOWER_T;
		case 'u': return LOWER_U;
		case 'v': return LOWER_V;
		case 'w': return LOWER_W;
		case 'x': return LOWER_X;
		case 'y': return LOWER_Y;
		case 'z': return LOWER_Z;

		case '1': return NUM_1;
		case '2': return NUM_2;
		case '3': return NUM_3;
		case '4': return NUM_4;
		case '5': return NUM_5;
		case '6': return NUM_6;
		case '7': return NUM_7;
		case '8': return NUM_8;
		case '9': return NUM_9;
		case '0': return NUM_0;

		case '!': return SYM_1;
		case '@': return SYM_2;
		case '#': return SYM_3;
		case '$': return SYM_4;
		case '%': return SYM_5;
		case '^': return SYM_6;
		case '&': return SYM_7;
		case '*': return SYM_8;
		case '(': return SYM_9;
		case ')': return SYM_0;

		case '-': return DASH;
		case '_': return UNDERSCORE;
		case '=': return EQUALS;
		case '+': return PLUS;
		case '\\': return B_SLASH;
		case '|': return PIPE;
		case '[': return L_BRACKET;
		case '{': return L_BRACE;
		case ']': return R_BRACKET;
		case '}': return R_BRACE;
		case ';': return SEMICOLON;
		case ':': return COLON;
		case '\'': return S_QUOTE;
		case '"': return D_QUOTE;
		case ',': return COMMA;
		case '<': return LESSTHAN;
		case '.': return PERIOD;
		case '>': return MORETHAN;
		case '/': return F_SLASH;
		case '?': return QUESTION;
	}
	return NONE;
}

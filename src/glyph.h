#ifndef GLYPH_H
#define GLYPH_H

#include <raylib.h>

#include <stdlib.h>
#include <stdio.h>

// Roguelib - src/glyph.h
// github.com/leftbones/roguelib

typedef struct Tileset {
	Texture texture;
	int source_tile_width;
	int source_tile_height;
	int target_tile_width;
	int target_tile_height;
	Color default_color;
} Tileset;

void tileset_initialize(
		Tileset *tileset,
		const char* texture_path, 
		int source_tile_width, 
		int source_tile_height, 
		int target_tile_width,
		int target_tile_height,
		Color default_color
	);
void tileset_free(Tileset *tileset);


// Glyph - a character from the tileset, stored for convenience
typedef struct Glyph {
    int source_tile_x; // x position in the tileset
    int source_tile_y; // y position in the tileset
	Tileset *tileset;
} Glyph;



void glyph_initialize(
		Glyph *glyph, 
		Tileset *tileset, 
		int source_tile_x, 
		int source_tile_y
	);
void glyph_free(Glyph *glyph);
void glyph_draw(Glyph* glyph, int pos_x, int pos_y, Color color);
void glyph_draw_pro(Glyph *glyph, int pos_x, int pos_y, int width, int height, Color color);
int glyph_get_idx_by_char(char ch);


// Glyph Definitions
// ------------------------------

// Quick Note About Directions...
//
// Unidirectional glyph labeled 'N' means it is "north facing", which could be thought of as "right side up"
// Example: 'SBOX_TN' has its connecting end on its south side: ┬
//
// Multi-directional glyphs may also be labeled in this way, but sometimes will be labeled with U/D/L/R instead.
//
// I am aware this naming scheme directly contradicts the unicode standard, but writing all of these was super tedious
// and I really don't feel like doing it all again.


// Blank
// ------------------------------
#define    NONE         0 // NUL
#define    SPACE       32 // Space
#define    NBSP       255 // No-Break Space


// Alphanumeric
// ------------------------------

#define    UPPER_A     65 // A
#define    UPPER_B     66 // B
#define    UPPER_C     67 // C
#define    UPPER_D     68 // D
#define    UPPER_E     69 // E
#define    UPPER_F     70 // F
#define    UPPER_G     71 // G
#define    UPPER_H     72 // H
#define    UPPER_I     73 // I
#define    UPPER_J     74 // J
#define    UPPER_K     75 // K
#define    UPPER_L     76 // L
#define    UPPER_M     77 // M
#define    UPPER_N     78 // N
#define    UPPER_O     79 // O
#define    UPPER_P     80 // P
#define    UPPER_Q     81 // Q
#define    UPPER_R     82 // R
#define    UPPER_S     83 // S
#define    UPPER_T     84 // T
#define    UPPER_U     85 // U
#define    UPPER_V     86 // V
#define    UPPER_W     87 // W
#define    UPPER_X     88 // X
#define    UPPER_Y     89 // Y
#define    UPPER_Z     90 // Z

#define    LOWER_A     97 // a
#define    LOWER_B     98 // b
#define    LOWER_C     99 // c
#define    LOWER_D    100 // d
#define    LOWER_E    101 // e
#define    LOWER_F    102 // f
#define    LOWER_G    103 // g
#define    LOWER_H    104 // h
#define    LOWER_I    105 // i
#define    LOWER_J    106 // j
#define    LOWER_K    107 // k
#define    LOWER_L    108 // l
#define    LOWER_M    109 // m
#define    LOWER_N    110 // n
#define    LOWER_O    111 // o
#define    LOWER_P    112 // p
#define    LOWER_Q    113 // q
#define    LOWER_R    114 // r
#define    LOWER_S    115 // s
#define    LOWER_T    116 // t
#define    LOWER_U    117 // u
#define    LOWER_V    118 // v
#define    LOWER_W    119 // w
#define    LOWER_X    120 // x
#define    LOWER_Y    121 // y
#define    LOWER_Z    122 // z

#define    NUM_1       49 // 1
#define    NUM_2       50 // 2
#define    NUM_3       51 // 3
#define    NUM_4       52 // 4
#define    NUM_5       53 // 5
#define    NUM_6       54 // 6
#define    NUM_7       55 // 7
#define    NUM_8       56 // 8
#define    NUM_9       57 // 9
#define    NUM_0       48 // 0


// Basic Symbols
// ------------------------------

#define    SYM_1       33 // !
#define    SYM_2       64 // @
#define    SYM_3       35 // #
#define    SYM_4       36 // $
#define    SYM_5       37 // %
#define    SYM_6       94 // ^
#define    SYM_7       38 // &
#define    SYM_8       42 // *
#define    SYM_9       40 // (
#define    SYM_0       41 // )

#define    DASH        45 // -
#define    UNDERSCORE  95 // _
#define    EQUALS      61 // =
#define    PLUS        43 // +
#define    BRACE_L    123 // {
#define    BRACE_R    125 // }
#define    BRACKET_L   91 // [
#define    BRACKET_R   93 // ]
#define    QUOTE_S     39 // '
#define    QUOTE_D     34 // "
#define    SEMICOLON   59 // ;
#define    COLON       58 // :
#define    LESSTHAN    60 // <
#define    MORETHAN    62 // >
#define    COMMA       44 // ,
#define    PERIOD      46 // .
#define    QUESTION    63 // ?
#define    F_SLASH     47 // /
#define    B_SLASH     92 // \. <-- C made me put this dot here :(
#define    PIPE       124 // |
#define    TILDE      126 // ~
#define    GRAVE       96 // `

// Duplicates (to avoid confusion)
#define    EXCLAM      33 // !
#define    AT          64 // @
#define    HASH        35 // #
#define    DOLLAR      36 // $
#define    PERCENT     37 // %
#define    CARET       94 // ^
#define    AMPERSAND   38 // &
#define    ASTERISK    42 // *
#define    STAR        42 // *
#define    DOT         46 // .
#define    S_QUOTE     39 // '
#define    D_QUOTE     34 // "
#define    MINUS       45 // -
#define    GREATERTHAN 62 // >
#define    APOSTROPHE  39 // '
#define    BACKTICK    96 // `
#define    L_BRACE    123 // {
#define    LBRACE     123 // {
#define    R_BRACE    125 // }
#define    RBRACE     125 // }
#define    L_BRACKET   91 // [
#define    LBRACKET    91 // [
#define    R_BRACKET   93 // ]
#define    RBRACKET    93 // ]
#define    PAREN_L     40 // (
#define    L_PAREN     40 // (
#define    LPAREN      40 // (
#define    PAREN_R     41 // )
#define    R_PAREN     41 // )
#define    RPAREN      41 // )
#define    SLASH_F     47 // /
#define    F_SLASH     47 // /
#define    FSLASH      47 // /
#define    SLASH_B     92 // \.
#define    B_SLASH     92 // \.
#define    BSLASH      92 // \.


// Box Drawing
// ------------------------------

// Single Line Box
#define    SBOX_WN    196 // Wall N
#define    SBOX_WE    179 // Wall E
#define    SBOX_WS    196 // Wall S
#define    SBOX_WW    179 // Wall W
#define    SBOX_TL    218 // Corner TL
#define    SBOX_TR    191 // Corner TR
#define    SBOX_BL    192 // Corner BL
#define    SBOX_BR    217 // Corner BR
#define    SBOX_TN    194 // T-Shape N
#define    SBOX_TE    180 // T-Shape E
#define    SBOX_TS    193 // T-Shape S
#define    SBOX_TW    195 // T-Shape W
#define    SBOX_VH    197 // Cross VH


// Double Line Box
#define    DBOX_WN    205 // Wall N
#define    DBOX_WE    186 // Wall E
#define    DBOX_WS    205 // Wall S
#define    DBOX_WW    186 // Wall W
#define    DBOX_TL    201 // Corner TL
#define    DBOX_TR    187 // Corner TR
#define    DBOX_BL    200 // Corner BL
#define    DBOX_BR    188 // Corner BR
#define    DBOX_TN    203 // T-Shape N
#define    DBOX_TE    185 // T-Shape E
#define    DBOX_TS    202 // T-Shape S
#define    DBOX_TW    204 // T-Shape W
#define    DBOX_VH    206 // Cross VH

// Special Box Shapes
#define    BOX_TN_VDHS    210 // T-Shape N, V Double, H Single
#define    BOX_TS_VDHS    208 // T-Shape S, V Double, H Single
#define    BOX_TE_VDHS    181 // T-Shape E, V Double, H Single
#define    BOX_TW_VDHS    198 // T-Shape W, V Double, H Single


// "Control Characters"
// ------------------------------
// The original names of these symbols are hard to remember, so I made up my own dumb names for them

#define    FACE_LIGHT     1
#define    FACE_DARK      2
#define    HEART          3
#define    DIAMOND        4
#define    CLUB           5
#define    SPADE          6
#define    BULLET         7
#define    HOLE_SQUARE    8
#define    EMPTY_BULLET   9
#define    DONUT_SQUARE   10
#define    MALE           11
#define    FEMALE         12
#define    MUSIC_NOTE_1   13
#define    MUSIC_NOTE_2   14
#define    SUN            15
#define    TRIANGLE_R     16
#define    TRIANGLE_L     17
#define    UD_ARROW       18
#define    DOUBLE_EXCLAM  19
#define    PARAGRAPH      20
#define    SIMOLEON       21
#define    UNDERBLOCK     22
#define    UD_ARROW_FLAT  23
#define    U_ARROW        24
#define    D_ARROW        25
#define    R_ARROW        26
#define    L_ARROW        27
#define    FILE_SEP       28
#define    LR_ARROW       29
#define    TRIANGLE_U     30
#define    TRIANGLE_D     31


// Misc. Characters
// ------------------------------

#define    SHIFT_LEFT    174
#define    SHIFT_RIGHT   175

#define    SHADE_LIGHT   176
#define    SHADE_MEDIUM  177
#define    SHADE_DARK    178

#define    FULL_BLOCK    219
#define    HALF_BLOCK_D  220
#define    HALF_BLOCK_L  221
#define    HALF_BLOCK_R  222
#define    HALF_BLOCK_U  223

#define    ALMOST        247
#define    DEGREE        248
#define    BULLET_S      249
#define    MIDDLE_DOT    250
#define    BLACK_SQUARE  254



// Greek Letters
// ------------------------------

#define    ALPHA          224
#define    SHARP          225
#define    GAMMA          226
#define    GREEK_PI       227 // Greek specified so as to not redefine PI from <math.h>
#define    SIGMA_UPPER    228
#define    SIGMA_LOWER    229
#define    TAU            231
#define    PHI_UPPER      232
#define    PHI_LOWER      237
#define    THETA          233
#define    OMEGA          234
#define    DELTA          235
//#define    EPSILON        238



#endif // GLYPH_H

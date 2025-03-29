// File Name: lcd_font.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __LCD_FONT_H
#define __LCD_FONT_H

#include "stdint.h"

//LCD英文字库8x16 Source code pro
extern const uint8_t lcdEnFontSourceCodePro8x16[128 * 16];
//LCD英文字库16x32 Source code pro
extern const uint8_t lcdEnFontSourceCodePro16x32[128 * 64];

//LCD数字字库16x16  方正姚体
extern const uint8_t lcdNumFontFZYTK16x16[12 * 32];
//LCD数字字库24x32  方正姚体
extern const uint8_t lcdNumFontFZYTK24x32[12 * 96];

#endif //__LCD_FONT_H

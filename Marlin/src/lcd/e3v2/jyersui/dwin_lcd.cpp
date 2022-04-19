/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/********************************************************************************
 * @file     lcd/e3v2/jyersui/dwin_lcd.cpp
 * @brief    DWIN screen control functions
 ********************************************************************************/

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD_JYERSUI)

#include "dwin_lcd.h"

/*-------------------------------------- System variable function --------------------------------------*/

void DWIN_Startup() {}

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Draw the degree (°) symbol
// Color: color
//  x/y: Upper-left coordinate of the first pixel
void DWIN_Draw_DegreeSymbol(uint16_t Color, uint16_t x, uint16_t y) {
  DWIN_Draw_Point(Color, 1, 1, x + 1, y);
  DWIN_Draw_Point(Color, 1, 1, x + 2, y);
  DWIN_Draw_Point(Color, 1, 1, x, y + 1);
  DWIN_Draw_Point(Color, 1, 1, x + 3, y + 1);
  DWIN_Draw_Point(Color, 1, 1, x, y + 2);
  DWIN_Draw_Point(Color, 1, 1, x + 3, y + 2);
  DWIN_Draw_Point(Color, 1, 1, x + 1, y + 3);
  DWIN_Draw_Point(Color, 1, 1, x + 2, y + 3);
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  DWIN_ICON_Show(false, false, true, libID, picID, x, y);
}

/*---------------------------------------- Numeric related functions ----------------------------------------*/
//  * From DWIN UI Enhanced implementation
//  * Version: 3.9.1
//  * Date: 2022/02/08

// Draw a numeric value
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  signedMode: 1=signed; 0=unsigned
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of digits
//  fNum: Number of decimal digits
//  x/y: Upper-left coordinate
//  value: Integer value
void DWIN_Draw_Value(uint8_t bShow, bool signedMode, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, int32_t value) {
  size_t i = 0;
  DWIN_Byte(i, 0x14);
  // Bit 7: bshow
  // Bit 6: 1 = signed; 0 = unsigned number;
  // Bit 5: zeroFill
  // Bit 4: zeroMode
  // Bit 3-0: size
  DWIN_Byte(i, (bShow * 0x80) | (signedMode * 0x40) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, signedMode && (value >= 0) ? iNum + 1 : iNum);
  DWIN_Byte(i, fNum);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  // Write a big-endian 64 bit integer
  const size_t p = i + 1;
  for (char count = 8; count--;) { // 7..0
    ++i;
    DWIN_SendBuf[p + count] = value;
    value >>= 8;
  }
  DWIN_Send(i);
}

// Draw a numeric value
//  value: positive unscaled float value
void DWIN_Draw_Value(uint8_t bShow, bool signedMode, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
  const int32_t val = round(value * POW(10, fNum));
  DWIN_Draw_Value(bShow, signedMode, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, val);
}


#endif // DWIN_CREALITY_LCD_JYERSUI

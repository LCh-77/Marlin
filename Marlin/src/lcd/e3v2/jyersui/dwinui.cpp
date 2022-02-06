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

/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.11.1
 * Date: 2022/01/19
 *
 * Modded for JYERSUI by LCH-77
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD_JYERSUI)

#include "../../../inc/MarlinConfig.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin_defines.h"

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

xy_int_t DWINUI::cursor = { 0 };
uint16_t DWINUI::pencolor = Color_White;
uint16_t DWINUI::textcolor = Def_Text_Color;
uint16_t DWINUI::backcolor = Def_Background_Color;
uint8_t  DWINUI::font = font8x16;

void DWINUI::init() {
  DEBUG_ECHOPGM("\r\nDWIN handshake ");
  delay(750);   // Delay here or init later in the boot process
  const bool success = DWIN_Handshake();
  if (success) DEBUG_ECHOLNPGM("ok."); else DEBUG_ECHOLNPGM("error.");
  DWIN_Frame_SetDir(1);
  TERN(SHOW_BOOTSCREEN,,DWIN_Frame_Clear(Color_Bg_Black));
  DWIN_UpdateLCD();
  cursor.x = 0;
  cursor.y = 0;
  pencolor = Color_White;
  textcolor = Def_Text_Color;
  backcolor = Def_Background_Color;
  font = font8x16;
}

// Set text/number font
void DWINUI::setFont(uint8_t cfont) {
  font = cfont;
}

// Get font character width
uint8_t DWINUI::fontWidth(uint8_t cfont) {
  switch (cfont) {
    case font6x12 : return 6;
    case font8x16 : return 8;
    case font10x20: return 10;
    case font12x24: return 12;
    case font14x28: return 14;
    case font16x32: return 16;
    case font20x40: return 20;
    case font24x48: return 24;
    case font28x56: return 28;
    case font32x64: return 32;
    default: return 0;
  }
}

// Get font character height
uint8_t DWINUI::fontHeight(uint8_t cfont) {
  switch (cfont) {
    case font6x12 : return 12;
    case font8x16 : return 16;
    case font10x20: return 20;
    case font12x24: return 24;
    case font14x28: return 28;
    case font16x32: return 32;
    case font20x40: return 40;
    case font24x48: return 48;
    case font28x56: return 56;
    case font32x64: return 64;
    default: return 0;
  }
}

// Get screen x coordinates from text column
uint16_t DWINUI::ColToX(uint8_t col) {
  return col * fontWidth(font);
}

// Get screen y coordinates from text row
uint16_t DWINUI::RowToY(uint8_t row) {
  return row * fontHeight(font);
}

// Set text/number color
void DWINUI::SetColors(uint16_t fgcolor, uint16_t bgcolor) {
  textcolor = fgcolor;
  backcolor = bgcolor;
}
void DWINUI::SetTextColor(uint16_t fgcolor) {
  textcolor = fgcolor;
}
void DWINUI::SetBackgroundColor(uint16_t bgcolor) {
  backcolor = bgcolor;
}

// Moves cursor to point
//  x: abscissa of the display
//  y: ordinate of the display
//  point: xy coordinate
void DWINUI::MoveTo(int16_t x, int16_t y) {
  cursor.x = x;
  cursor.y = y;
}
void DWINUI::MoveTo(xy_int_t point) {
  cursor = point;
}

// Moves cursor relative to the actual position
//  x: abscissa of the display
//  y: ordinate of the display
//  point: xy coordinate
void DWINUI::MoveBy(int16_t x, int16_t y) {
  cursor.x += x;
  cursor.y += y;
}
void DWINUI::MoveBy(xy_int_t point) {
  cursor += point;
}

// Draw a Centered string using DWIN_WIDTH
void DWINUI::Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string) {
  const int8_t x = _MAX(0U, DWIN_WIDTH - strlen_P(string) * fontWidth(size)) / 2 - 1;
  DWIN_Draw_String(bShow, size, color, bColor, x, y, string);
}

// Draw a char at cursor position
void DWINUI::Draw_Char(const char c) {
  const char string[2] = { c, 0};
  DWIN_Draw_String(false, font, textcolor, backcolor, cursor.x, cursor.y, string, 1);
  MoveBy(fontWidth(font), 0);
}

// Draw a string at cursor position
//  color: Character color
//  *string: The string
//  rlimit: For draw less chars than string length use rlimit
void DWINUI::Draw_String(const char * const string, uint16_t rlimit) {
  DWIN_Draw_String(false, font, textcolor, backcolor, cursor.x, cursor.y, string, rlimit);
  MoveBy(strlen(string) * fontWidth(font), 0);
}
void DWINUI::Draw_String(uint16_t color, const char * const string, uint16_t rlimit) {
  DWIN_Draw_String(false, font, color, backcolor, cursor.x, cursor.y, string, rlimit);
  MoveBy(strlen(string) * fontWidth(font), 0);
}

// Draw a signed floating point number
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  bColor: Background color
//  iNum: Number of whole digits
//  fNum: Number of decimal digits
//  x/y: Upper-left point
//  value: Float value
void DWINUI::Draw_Signed_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
  DWIN_Draw_FloatValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, value < 0 ? -value : value);
  DWIN_Draw_String(bShow, size, color, bColor, x - 6, y, value < 0 ? F("-") : F(" "));
}

// Draw a circle
//  color: circle color
//  x: the abscissa of the center of the circle
//  y: ordinate of the center of the circle
//  r: circle radius
void DWINUI::Draw_Circle(uint16_t color, uint16_t x, uint16_t y, uint8_t r) {
  int a = 0, b = 0;
  while (a <= b) {
    b = SQRT(sq(r) - sq(a));
    if (a == 0) b--;
    DWIN_Draw_Point(color, 1, 1, x + a, y + b);   // Draw some sector 1
    DWIN_Draw_Point(color, 1, 1, x + b, y + a);   // Draw some sector 2
    DWIN_Draw_Point(color, 1, 1, x + b, y - a);   // Draw some sector 3
    DWIN_Draw_Point(color, 1, 1, x + a, y - b);   // Draw some sector 4
    DWIN_Draw_Point(color, 1, 1, x - a, y - b);   // Draw some sector 5
    DWIN_Draw_Point(color, 1, 1, x - b, y - a);   // Draw some sector 6
    DWIN_Draw_Point(color, 1, 1, x - b, y + a);   // Draw some sector 7
    DWIN_Draw_Point(color, 1, 1, x - a, y + b);   // Draw some sector 8
    a++;
  }
}

// Draw a circle filled with color
//  bcolor: fill color
//  x: the abscissa of the center of the circle
//  y: ordinate of the center of the circle
//  r: circle radius
void DWINUI::Draw_FillCircle(uint16_t bcolor, uint16_t x,uint16_t y,uint8_t r) {
  int a = 0, b = 0;
  while (a <= b) {
    b = SQRT(sq(r) - sq(a)); // b=sqrt(r*r-a*a);
    if (a == 0) b--;
    DWIN_Draw_Line(bcolor, x-b,y-a,x+b,y-a);
    DWIN_Draw_Line(bcolor, x-a,y-b,x+a,y-b);
    DWIN_Draw_Line(bcolor, x-b,y+a,x+b,y+a);
    DWIN_Draw_Line(bcolor, x-a,y+b,x+a,y+b);
    a++;
  }
}

// Color Interpolator
//  val : Interpolator minv..maxv
//  minv : Minimum value
//  maxv : Maximum value
//  color1 : Start color
//  color2 : End color
uint16_t DWINUI::ColorInt(int16_t val, int16_t minv, int16_t maxv, uint16_t color1, uint16_t color2) {
  uint8_t B,G,R;
  float n;
  n = (float)(val-minv)/(maxv-minv);
  R = (1-n)*GetRColor(color1) + n*GetRColor(color2);
  G = (1-n)*GetGColor(color1) + n*GetGColor(color2);
  B = (1-n)*GetBColor(color1) + n*GetBColor(color2);
  return RGB(R,G,B);
}

// Color Interpolator through Red->Yellow->Green->Blue
//  val : Interpolator minv..maxv
//  minv : Minimum value
//  maxv : Maximum value
uint16_t DWINUI::RainbowInt(int16_t val, int16_t minv, int16_t maxv) {
  uint8_t B,G,R;
  const uint8_t maxB = 28;
  const uint8_t maxR = 28;
  const uint8_t maxG = 38;
  const int16_t limv = _MAX(abs(minv), abs(maxv));
  float n;
  if (minv>=0) {
    n = (float)(val-minv)/(maxv-minv);
  } else {
    n = (float)val/limv;
  }
  n = _MIN(1, n);
  n = _MAX(-1, n);
  if (n < 0) {
    R = 0;
    G = (1+n)*maxG;
    B = (-n)*maxB;
  } else if (n < 0.5) {
    R = maxR*n*2;
    G = maxG;
    B = 0;
  } else {
    R = maxR;
    G = maxG*(1-n);
    B = 0;
  }
  return RGB(R,G,B);
}

// Draw a checkbox
//  Color: frame color
//  bColor: Background color
//  x/y: Upper-left point
//  mode : 0 : unchecked, 1 : checked
void DWINUI::Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool checked=false) {
  DWIN_Draw_String(true, font8x16, color, bcolor, x + 4, y, checked ? F("x") : F(" "));
  DWIN_Draw_Rectangle(0, color, x + 2, y + 2, x + 17, y + 17);
}

// Clear Menu by filling the menu area with background color
void DWINUI::ClearMenuArea() {
  DWIN_Draw_Rectangle(1, backcolor, 0, TITLE_HEIGHT, DWIN_WIDTH - 1, STATUS_Y - 1);
}

#endif // DWIN_CREALITY_LCD_JYERSUI

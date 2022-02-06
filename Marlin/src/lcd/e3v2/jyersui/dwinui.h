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
#pragma once

/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.11.1
 * Date: 2022/01/19
 *
 * Modded for JYERSUI by LCH-77
 */

#include "dwin_lcd.h"
#include "../common/dwin_set.h"
#include "../common/dwin_font.h"
#include "../common/dwin_color.h"

// Custom icons
//#define DWIN_CREALITY_LCD_CUSTOM_ICONS
#if ENABLED(DWIN_CREALITY_LCD_CUSTOM_ICONS)
  // index of every custom icon should be >= CUSTOM_ICON_START
  #define CUSTOM_ICON_START         ICON_Checkbox_F
  #define ICON_Checkbox_F           200
  #define ICON_Checkbox_T           201
  #define ICON_Fade                 202
  #define ICON_Mesh                 203
  #define ICON_Tilt                 204
  #define ICON_Brightness           205
  #define ICON_AxisD                249
  #define ICON_AxisBR               250
  #define ICON_AxisTR               251
  #define ICON_AxisBL               252
  #define ICON_AxisTL               253
  #define ICON_AxisC                254
#else
  #define ICON_Fade                 ICON_Version
  #define ICON_Mesh                 ICON_Version
  #define ICON_Tilt                 ICON_Version
  #define ICON_Brightness           ICON_Version
  #define ICON_AxisD                ICON_Axis
  #define ICON_AxisBR               ICON_Axis
  #define ICON_AxisTR               ICON_Axis
  #define ICON_AxisBL               ICON_Axis
  #define ICON_AxisTL               ICON_Axis
  #define ICON_AxisC                ICON_Axis
  #define ICON_ESDiag               ICON_Info
  #define ICON_Lock                 ICON_Cool
  #define ICON_Reboot               ICON_ResumeEEPROM
  #define ICON_ProbeAlarm           ICON_SetEndTemp
  #define ICON_ProbeSelfTest        ICON_SetEndTemp
  #define ICON_ProbeDeploy          ICON_SetEndTemp
  #define ICON_ProbeTest            ICON_SetEndTemp
  #define ICON_ProbeTestCount       ICON_SetEndTemp
  #define ICON_FWRetLength          ICON_StepE
  #define ICON_FWRetSpeed           ICON_Setspeed
  #define ICON_FWRetZRaise          ICON_MoveZ
  #define ICON_FWRecExtLength       ICON_StepE
  #define ICON_FWRecSpeed           ICON_Setspeed
  #define ICON_HSMode               ICON_StockConfiguration
#endif

// Extended UI Colors
#define Color_Aqua          RGB(0,63,31)
#define Color_Light_White   0xBDD7
#define Color_Green         RGB(0,63,0)
#define Color_Light_Green   0x3460
#define Color_Cyan          0x07FF
#define Color_Light_Cyan    0x04F3
#define Color_Blue          RGB(0,0,31)
#define Color_Light_Blue    0x3A6A
#define Color_Magenta       0xF81F
#define Color_Light_Magenta 0x9813
#define Color_Light_Red     0x8800
#define Color_Orange        0xFA20
#define Color_Light_Orange  0xFBC0
#define Color_Light_Yellow  0x8BE0
#define Color_Brown         0xCC27
#define Color_Light_Brown   0x6204
#define Color_Black         0x0000
#define Color_Grey          0x18E3
#define Check_Color         0x4E5C
#define Confirm_Color       0x34B9
#define Cancel_Color        0x3186

// UI element defines and constants
#define DWIN_FONT_MENU font8x16
#define DWIN_FONT_STAT font10x20
#define DWIN_FONT_HEAD font10x20
#define DWIN_FONT_ALERT font10x20
#define STATUS_Y 354
#define LCD_WIDTH (DWIN_WIDTH / 8)

constexpr uint16_t TITLE_HEIGHT = 30,                          // Title bar height
                   MLINE = 53,                                 // Menu line height
                   TROWS = (STATUS_Y - TITLE_HEIGHT) / MLINE,  // Total rows
                   MROWS = TROWS - 1,                          // Other-than-Back
                   ICOX = 26,                                  // Menu item icon X position
                   LBLX = 60,                                  // Menu item label X position
                   VALX = 210,                                 // Menu item value X position
                   MENU_CHR_W = 8, MENU_CHR_H = 16,            // Menu font 8x16
                   STAT_CHR_W = 10;

// Menuitem Y position
#define MYPOS(L) (TITLE_HEIGHT + MLINE * (L))

// Menuitem caption Offset
#define CAPOFF ((MLINE - MENU_CHR_H) / 2)

// Menuitem caption Y position
#define MBASE(L) (MYPOS(L) + CAPOFF)

typedef struct { uint16_t left, top, right, bottom; } rect_t;
typedef struct { uint16_t x, y, w, h; } frame_rect_t;

namespace DWINUI {
  extern xy_int_t cursor;
  extern uint16_t pencolor;
  extern uint16_t textcolor;
  extern uint16_t backcolor;
  extern uint8_t  font;

  // DWIN LCD Initialization
  void init();

  // Set text/number font
  void setFont(uint8_t cfont);

  // Get font character width
  uint8_t fontWidth(uint8_t cfont);
  inline uint8_t fontWidth() { return fontWidth(font); };

  // Get font character height
  uint8_t fontHeight(uint8_t cfont);
  inline uint8_t fontHeight() { return fontHeight(font); };

  // Get screen x coordinates from text column
  uint16_t ColToX(uint8_t col);

  // Get screen y coordinates from text row
  uint16_t RowToY(uint8_t row);

  // Set text/number color
  void SetColors(uint16_t fgcolor, uint16_t bgcolor);
  void SetTextColor(uint16_t fgcolor);
  void SetBackgroundColor(uint16_t bgcolor);

  // Moves cursor to point
  //  x: abscissa of the display
  //  y: ordinate of the display
  //  point: xy coordinate
  void MoveTo(int16_t x, int16_t y);
  void MoveTo(xy_int_t point);

  // Moves cursor relative to the actual position
  //  x: abscissa of the display
  //  y: ordinate of the display
  //  point: xy coordinate
  void MoveBy(int16_t x, int16_t y);
  void MoveBy(xy_int_t point);

  // Draw a line from the cursor to xy position
  //  color: Line segment color
  //  x/y: End point
  inline void LineTo(uint16_t color, uint16_t x, uint16_t y) {
    DWIN_Draw_Line(color, cursor.x, cursor.y, x, y);
  }
  inline void LineTo(uint16_t x, uint16_t y) {
    DWIN_Draw_Line(pencolor, cursor.x, cursor.y, x, y);
  }

  // Draw an Icon with transparent background from the library ICON
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_Icon(uint8_t icon, uint16_t x, uint16_t y) {
    DWIN_ICON_Show(ICON, icon, x, y);
  }

  // Draw an Icon from the library ICON with its background
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_IconWB(uint8_t icon, uint16_t x, uint16_t y) {
    DWIN_ICON_Show(true, false, false, ICON, icon, x, y);
  }

  // Draw a positive integer
  //  bShow: true=display background color; false=don't display background color
  //  zeroFill: true=zero fill; false=no zero fill
  //  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of digits
  //  x/y: Upper-left coordinate
  //  value: Integer value
  inline void Draw_Int(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Int(uint8_t iNum, long value) {
    DWIN_Draw_IntValue(false, true, 0, font, textcolor, backcolor, iNum, cursor.x, cursor.y, value);
    MoveBy(iNum * fontWidth(font), 0);
  }
  inline void Draw_Int(uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(false, true, 0, font, textcolor, backcolor, iNum, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(false, true, 0, font, color, backcolor, iNum, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(true, true, 0, font, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Int(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(true, true, 0, size, color, bColor, iNum, x, y, value);
  }

  // Draw a floating point number
  //  bShow: true=display background color; false=don't display background color
  //  zeroFill: true=zero fill; false=no zero fill
  //  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of whole digits
  //  fNum: Number of decimal digits
  //  x/y: Upper-left point
  //  value: Float value
  inline void Draw_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, float value) {
    DWIN_Draw_FloatValue(false, true, 0, font, textcolor, backcolor, iNum, fNum,  cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(false, true, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(false, true, 0, font, color, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(true, true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(true, true, 0, size, color, bColor, iNum, fNum, x, y, value);
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
  void Draw_Signed_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value);
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, float value) {
    Draw_Signed_Float(false, true, 0, font, textcolor, backcolor, iNum, fNum, cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(false, true, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t size, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(false, true, 0, size, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(true, true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(true, true, 0, size, color, bColor, iNum, fNum, x, y, value);
  }

  // Draw a char at cursor position
  void Draw_Char(const char c);

  // Draw a string at cursor position
  //  color: Character color
  //  *string: The string
  //  rlimit: For draw less chars than string length use rlimit
  void Draw_String(const char * const string, uint16_t rlimit = 0xFFFF);
  void Draw_String(uint16_t color, const char * const string, uint16_t rlimit = 0xFFFF);
  inline void Draw_String(FSTR_P  string, uint16_t rlimit = 0xFFFF) {
    Draw_String(FTOP(string), rlimit);
  }
  inline void Draw_String(uint16_t color, FSTR_P string, uint16_t rlimit = 0xFFFF) {
    Draw_String(color, FTOP(string), rlimit);
  }

  // Draw a string
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  x/y: Upper-left coordinate of the string
  //  *string: The string
  inline void Draw_String(uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(false, font, textcolor, backcolor, x, y, string);
  }
  inline void Draw_String(uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(false, font, textcolor, backcolor, x, y, FTOP(title));
  }
  inline void Draw_String(uint16_t color, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(false, font, color, backcolor, x, y, string);
  }
  inline void Draw_String(uint16_t color, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(false, font, color, backcolor, x, y, title);
  }
  inline void Draw_String(uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, font, color, bgcolor, x, y, string);
  }
  inline void Draw_String(uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(true, font, color, bgcolor, x, y, title);
  }
  inline void Draw_String(uint8_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, string);
  }
  inline void Draw_String(uint8_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, title);
  }

  // Draw a centered string using DWIN_WIDTH
  //  bShow: true=display background color; false=don't display background color
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  y: Upper coordinate of the string
  //  *string: The string
  void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string);
  inline void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, FSTR_P string) {
    Draw_CenteredString(bShow, size, color, bColor, y, FTOP(string));
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t bcolor, uint16_t y, const char * const string) {
    Draw_CenteredString(true, font, color, bcolor, y, string);
  }
  inline void Draw_CenteredString(uint8_t size, uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, size, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint8_t size, uint16_t color, uint16_t y, FSTR_P title) {
    Draw_CenteredString(false, size, color, backcolor, y, title);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, font, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t y, FSTR_P title) {
    Draw_CenteredString(false, font, color, backcolor, y, title);
  }
  inline void Draw_CenteredString(uint16_t y, const char * const string) {
    Draw_CenteredString(false, font, textcolor, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint16_t y, FSTR_P title) {
    Draw_CenteredString(false, font, textcolor, backcolor, y, title);
  }

  // Draw a box
  //  mode: 0=frame, 1=fill, 2=XOR fill
  //  color: Rectangle color
  //  frame: Box coordinates and size
  inline void Draw_Box(uint8_t mode, uint16_t color, frame_rect_t frame) {
    DWIN_Draw_Box(mode, color, frame.x, frame.y, frame.w, frame.h);
  }

  // Draw a circle
  //  Color: circle color
  //  x: abscissa of the center of the circle
  //  y: ordinate of the center of the circle
  //  r: circle radius
  void Draw_Circle(uint16_t color, uint16_t x,uint16_t y,uint8_t r);
  inline void Draw_Circle(uint16_t color, uint8_t r) {
    Draw_Circle(color, cursor.x, cursor.y, r);
  }

  // Draw a checkbox
  //  Color: frame color
  //  bColor: Background color
  //  x/y: Upper-left point
  //  checked : 0 : unchecked, 1 : checked
  void Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool checked);
  inline void Draw_Checkbox(uint16_t x, uint16_t y, bool checked=false) {
    Draw_Checkbox(textcolor, backcolor, x, y, checked);
  }

  // Color Interpolator
  //  val : Interpolator minv..maxv
  //  minv : Minimum value
  //  maxv : Maximum value
  //  color1 : Start color
  //  color2 : End color
  uint16_t ColorInt(int16_t val, int16_t minv, int16_t maxv, uint16_t color1, uint16_t color2);

  // -------------------------- Extra -------------------------------//

  // Draw a circle filled with color
  //  bcolor: fill color
  //  x: abscissa of the center of the circle
  //  y: ordinate of the center of the circle
  //  r: circle radius
  void Draw_FillCircle(uint16_t bcolor, uint16_t x,uint16_t y,uint8_t r);
  inline void Draw_FillCircle(uint16_t bcolor, uint8_t r) {
    Draw_FillCircle(bcolor, cursor.x, cursor.y, r);
  }

  // Color Interpolator through Red->Yellow->Green->Blue
  //  val : Interpolator minv..maxv
  //  minv : Minimum value
  //  maxv : Maximum value
  uint16_t RainbowInt(int16_t val, int16_t minv, int16_t maxv);

  // Clear Menu by filling the area with background color
  // Area (0, TITLE_HEIGHT, DWIN_WIDTH, STATUS_Y - 1)
  void ClearMenuArea();

};

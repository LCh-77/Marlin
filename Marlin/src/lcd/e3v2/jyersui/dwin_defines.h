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
 * DWIN general defines and data structs
 * Author: Miguel A. Risco-Castillo
 * Version: 3.9.1
 * Date: 2021/11/21
 *
 * Modded for JYERSUI by LCH-77
 */

#define HAS_ESDIAG 1
#define HAS_LOCKSCREEN 1

#include "../../../core/types.h"
#include "../common/dwin_color.h"
#include "dwinui.h"

// Default UI Colors
#define Def_Background_Color  RGB( 1, 12,  8)
#define Def_Cursor_color      RGB(20, 49, 31)
#define Def_TitleBg_color     RGB( 0, 23, 16)
#define Def_TitleTxt_color    Color_White
#define Def_Text_Color        Color_White
#define Def_Selected_Color    Select_Color
#define Def_SplitLine_Color   RGB( 0, 23, 16)
#define Def_Highlight_Color   Color_White
#define Def_StatusBg_Color    RGB( 0, 23, 16)
#define Def_StatusTxt_Color   Color_Yellow
#define Def_PopupBg_color     Color_Bg_Window
#define Def_PopupTxt_Color    Popup_Text_Color
#define Def_AlertBg_Color     Color_Bg_Red
#define Def_AlertTxt_Color    Color_Yellow
#define Def_PercentTxt_Color  Percent_Color
#define Def_Barfill_Color     BarFill_Color
#define Def_Indicator_Color   Color_White
#define Def_Coordinate_Color  Color_White


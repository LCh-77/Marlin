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
 * DWIN general defines and data structs for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.11.2
 * Date: 2022/02/28
 *
 * Modded for JYERSUI by LCH-77
 * Version: 1.9
 * Date: Jun 16, 2022
 */

#include "../../../inc/MarlinConfigPre.h"

#if DISABLED(DWIN_CREALITY_LCD_JYERSUI)
  #error "Enhanced JYers firmware requires enabling DWIN_CREALITY_LCD_JYERSUI in the Configuration.h file" 
#endif

#define JYENHANCED 1 // Enable LCH-77 JYersUI Enhancements
#define HAS_ESDIAG 1
#define HAS_LOCKSCREEN 1
#define HAS_PIDPLOT 1
#define HAS_GCODE_PREVIEW 1
//#define DEBUG_DWIN 1
//#define NEED_HEX_PRINT 1

#if ENABLED(HOST_ACTION_COMMANDS)
  #define HAS_HOSTACTION_MENUS 1
#endif

#include "../../../core/types.h"
#include "../common/dwin_color.h"

// Default UI Colors
#define Def_Background_Color  RGB(4,4,0)
#define Def_Cursor_color      RGB(24,24,0)
#define Def_TitleBg_color     RGB(12,12,0)
#define Def_TitleTxt_color    Color_White
#define Def_Text_Color        Color_White
#define Def_Selected_Color    RGB(24,24,0)
#define Def_SplitLine_Color   RGB(24,24,0)
#define Def_Highlight_Color   RGB(31,40,0)
#define Def_StatusBg_Color    RGB(12,12,0)
#define Def_StatusTxt_Color   Color_White
#define Def_PopupBg_color     Color_Bg_Window
#define Def_PopupTxt_Color    Popup_Text_Color
#define Def_AlertBg_Color     Color_Bg_Red
#define Def_AlertTxt_Color    Color_Yellow
#define Def_PercentTxt_Color  RGB(31,48,8)
#define Def_Barfill_Color     RGB(12,12,0)
#define Def_Indicator_Color   RGB(31,48,8)
#define Def_Coordinate_Color  Color_White
#define Def_Button_Color      RGB(12,12,0)

#if ENABLED(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  #define Def_Leds_Color      0xFFFFFFFF
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  #define Def_CaseLight_Brightness 255
#endif

#if HAS_MESH
  #ifndef MESH_INSET
    #define MESH_INSET 25
  #endif
  #ifndef MESH_MIN_X
    #define MESH_MIN_X MESH_INSET
  #endif
  #ifndef MESH_MIN_Y
    #define MESH_MIN_Y MESH_INSET
  #endif
  #ifndef MESH_MAX_X
    #define MESH_MAX_X  X_BED_SIZE - (MESH_INSET)
  #endif
  #ifndef MESH_MAX_Y
    #define MESH_MAX_Y  X_BED_SIZE - (MESH_INSET)
  #endif
#endif

#if JYENHANCED
  #include "jyenhanced.h"
#endif

typedef struct {
  bool time_format_textual : 1;
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    uint8_t tilt_grid_size : 3;
  #endif
  uint16_t corner_pos : 10;
  uint8_t cursor_color : 4;
  uint8_t menu_split_line : 4;
  uint8_t menu_top_bg : 4;
  uint8_t menu_top_txt : 4;
  uint8_t highlight_box : 4;
  uint8_t progress_percent : 4;
  uint8_t progress_time : 4;
  uint8_t status_bar_text : 4;
  uint8_t status_area_text : 4;
  uint8_t coordinates_text : 4;
  uint8_t coordinates_split_line : 4;
  #if ENABLED(BAUD_RATE_GCODE)
    bool Baud115k : 1;
  #endif
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    bool ena_hotend_levtemp : 1;
    bool ena_bed_levtemp : 1;
    celsius_t hotend_levtemp = LEVELING_NOZZLE_TEMP;
    celsius_t bed_levtemp = LEVELING_BED_TEMP;
  #endif
  #if HAS_HOSTACTION_MENUS
    uint64_t host_action_label_1 : 48;
    uint64_t host_action_label_2 : 48;
    uint64_t host_action_label_3 : 48;
  #endif
  #if JYENHANCED
    bool Invert_E0 = DEF_INVERT_E0_DIR;
    int16_t x_bed_size = DEF_X_BED_SIZE;
    int16_t y_bed_size = DEF_Y_BED_SIZE;
    int16_t x_min_pos = DEF_X_MIN_POS;
    int16_t y_min_pos = DEF_Y_MIN_POS;
    int16_t x_max_pos = DEF_X_MAX_POS;
    int16_t y_max_pos = DEF_Y_MAX_POS;
    int16_t z_max_pos = DEF_Z_MAX_POS;
    #if HAS_BED_PROBE
      float probing_margin = DEF_PROBING_MARGIN;
      uint16_t zprobefeedfast = DEF_Z_PROBE_FEEDRATE_FAST;
      uint16_t zprobefeedslow = DEF_Z_PROBE_FEEDRATE_SLOW;
    #endif
    #if HAS_MESH
      float mesh_min_x = DEF_MESH_MIN_X;
      float mesh_max_x = DEF_MESH_MAX_X;
      float mesh_min_y = DEF_MESH_MIN_Y;
      float mesh_max_y = DEF_MESH_MAX_Y;
    #endif  
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
        uint8_t fil_unload_feedrate = DEF_FILAMENT_CHANGE_UNLOAD_FEEDRATE;
        uint8_t fil_fast_load_feedrate = DEF_FILAMENT_CHANGE_FAST_LOAD_FEEDRATE;
    #endif
    #if ENABLED(NOZZLE_PARK_FEATURE)
      xyz_int_t Park_point = DEF_NOZZLE_PARK_POINT;
    #endif
  #endif
} eeprom_settings_t;

static constexpr size_t eeprom_data_size = 96;
extern eeprom_settings_t eeprom_settings;

#if JYENHANCED

  #undef INVERT_E0_DIR

  #undef X_BED_SIZE
  #undef Y_BED_SIZE
  #undef X_MIN_POS
  #undef Y_MIN_POS
  #undef X_MAX_POS
  #undef Y_MAX_POS
  #undef Z_MAX_POS

  #if HAS_BED_PROBE
    #undef PROBING_MARGIN
    #undef Z_PROBE_FEEDRATE_FAST
    #undef Z_PROBE_FEEDRATE_SLOW
  #endif

  #if HAS_MESH
    #undef MESH_MIN_X
    #undef MESH_MAX_X
    #undef MESH_MIN_Y
    #undef MESH_MAX_Y
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #undef FILAMENT_CHANGE_UNLOAD_FEEDRATE
    #undef FILAMENT_CHANGE_FAST_LOAD_FEEDRATE
  #endif
  #if ENABLED(NOZZLE_PARK_FEATURE)
    #undef NOZZLE_PARK_POINT
  #endif

  #define INVERT_E0_DIR eeprom_settings.Invert_E0

  #define X_BED_SIZE (float)eeprom_settings.x_bed_size
  #define Y_BED_SIZE (float)eeprom_settings.y_bed_size
  #define X_MIN_POS  (float)eeprom_settings.x_min_pos
  #define Y_MIN_POS  (float)eeprom_settings.y_min_pos
  #define X_MAX_POS  (float)eeprom_settings.x_max_pos
  #define Y_MAX_POS  (float)eeprom_settings.y_max_pos
  #define Z_MAX_POS  (float)eeprom_settings.z_max_pos

  #if HAS_BED_PROBE
    #define PROBING_MARGIN eeprom_settings.probing_margin
    #define Z_PROBE_FEEDRATE_FAST eeprom_settings.zprobefeedfast
    #define Z_PROBE_FEEDRATE_SLOW eeprom_settings.zprobefeedslow
  #endif

  #if HAS_MESH
    #define MESH_MIN_X eeprom_settings.mesh_min_x
    #define MESH_MAX_X eeprom_settings.mesh_max_x
    #define MESH_MIN_Y eeprom_settings.mesh_min_y
    #define MESH_MAX_Y eeprom_settings.mesh_max_y
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #define FILAMENT_CHANGE_UNLOAD_FEEDRATE (float)eeprom_settings.fil_unload_feedrate
    #define FILAMENT_CHANGE_FAST_LOAD_FEEDRATE (float)eeprom_settings.fil_fast_load_feedrate
  #endif

  #if ENABLED(NOZZLE_PARK_FEATURE)
    #define NOZZLE_PARK_POINT {(float)eeprom_settings.Park_point.x, (float)eeprom_settings.Park_point.y, (float)eeprom_settings.Park_point.z}
  #endif

#endif

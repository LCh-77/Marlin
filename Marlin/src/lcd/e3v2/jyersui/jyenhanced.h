/**
 * JYERSUI Enhanced
 * Author: LCH-77
 * Version: 1.2
 * Date: 2022/02/06
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For commercial applications additional licences can be requested
 */
#pragma once

#include "../../../inc/MarlinConfigPre.h"

#ifndef LOW
  #define LOW  0x0
#endif
#ifndef HIGH
  #define HIGH 0x1
#endif

#define X_BED_MIN 150
#define Y_BED_MIN 150
constexpr int16_t DEF_X_BED_SIZE = X_BED_SIZE;
constexpr int16_t DEF_Y_BED_SIZE = Y_BED_SIZE;
constexpr int16_t DEF_X_MIN_POS = X_MIN_POS;
constexpr int16_t DEF_Y_MIN_POS = Y_MIN_POS;
constexpr int16_t DEF_X_MAX_POS = X_MAX_POS;
constexpr int16_t DEF_Y_MAX_POS = Y_MAX_POS;
constexpr int16_t DEF_Z_MAX_POS = Z_MAX_POS;

#define DEF_NOZZLE_PARK_POINT {240, 220, 20}
#define MIN_PARK_POINT_Z 10

#if HAS_MESH
  constexpr int16_t DEF_MESH_MIN_X = MESH_MIN_X;
  constexpr int16_t DEF_MESH_MAX_X = MESH_MAX_X;
  constexpr int16_t DEF_MESH_MIN_Y = MESH_MIN_Y;
  constexpr int16_t DEF_MESH_MAX_Y = MESH_MAX_Y;
  #define MIN_MESH_INSET 5
  #define MAX_MESH_INSET X_BED_SIZE
#endif

#if HAS_BED_PROBE
  constexpr int16_t DEF_PROBING_MARGIN = PROBING_MARGIN;
  #define MIN_PROBE_MARGIN 5
  #define MAX_PROBE_MARGIN 60  
  #define MIN_Z_PROBE_FEEDRATE 60
  #define MAX_Z_PROBE_FEEDRATE 1200
  constexpr int16_t DEF_Z_PROBE_FEEDRATE_FAST = Z_PROBE_FEEDRATE_FAST;
  constexpr int16_t DEF_Z_PROBE_FEEDRATE_SLOW = (Z_PROBE_FEEDRATE_FAST / 2);
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #define MIN_FIL_CHANGE_FEEDRATE 1
  #define MAX_FIL_CHANGE_FEEDRATE 60
  constexpr int8_t DEF_FILAMENT_CHANGE_UNLOAD_FEEDRATE = FILAMENT_CHANGE_UNLOAD_FEEDRATE;
  constexpr int8_t DEF_FILAMENT_CHANGE_FAST_LOAD_FEEDRATE = FILAMENT_CHANGE_FAST_LOAD_FEEDRATE;
#endif

#define DEF_INVERT_E0_DIR false

// Class
class JYEnhancedClass {
public:
  #if HAS_MESH
    static void C29();
    static void C29_report(const bool forReplay=true);
    static void C852();
    static void C852_report(const bool forReplay=true);
  #endif
  static void UpdateAxis(const AxisEnum axis);
  static void ApplyPhySet();
  static void C100();
  static void C100_report(const bool forReplay=true);
  static void C101();
  static void C101_report(const bool forReplay=true);
  static void C102();
  static void C102_report(const bool forReplay=true);
  #if ENABLED(NOZZLE_PARK_FEATURE)
    static void C125();
    static void C125_report(const bool forReplay=true);
  #endif
    static void C562();
    static void C562_report(const bool forReplay=true);
  #if HAS_BED_PROBE
    static void C851();
    static void C851_report(const bool forReplay=true);
  #endif
};

extern JYEnhancedClass JYEnhanced;

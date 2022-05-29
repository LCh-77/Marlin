/**
 * JYERSUI Enhanced
 * Author: LCH-77
 * Version: 1.2
 * Date: 2022/05/28
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
#include "../../../inc/MarlinConfigPre.h"

#if JYENHANCED && ENABLED(DWIN_CREALITY_LCD_JYERSUI)

#include "dwin.h"
#include "../../../gcode/gcode.h"

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  #include "../../../feature/bedlevel/bedlevel.h"
  #include "../../../feature/bedlevel/abl/abl.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../../feature/bedlevel/bedlevel.h"
  #include "../../../feature/bedlevel/ubl/ubl.h"
#endif

#if HAS_BED_PROBE
  float Probe::_min_x(const xy_pos_t &probe_offset_xy) {
    return _MAX((X_MIN_BED) + (eeprom_settings.mesh_min_x), (X_MIN_POS) + probe_offset_xy.x);
  }
  float Probe::_max_x(const xy_pos_t &probe_offset_xy) {
    return _MIN((eeprom_settings.mesh_max_x), (X_MAX_POS) + probe_offset_xy.x);
  }
  float Probe::_min_y(const xy_pos_t &probe_offset_xy) {
    return _MAX((Y_MIN_BED) + (eeprom_settings.mesh_min_y), (Y_MIN_POS) + probe_offset_xy.y);
  }
  float Probe::_max_y(const xy_pos_t &probe_offset_xy) {
    return _MIN((eeprom_settings.mesh_max_y), (Y_MAX_POS) + probe_offset_xy.y);
  }
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  float unified_bed_leveling::get_mesh_x(const uint8_t i) {
      return (eeprom_settings.mesh_min_x) + i * (float(eeprom_settings.mesh_max_x - (eeprom_settings.mesh_min_x)) / GRID_MAX_CELLS_X);
  }
  float unified_bed_leveling::get_mesh_y(const uint8_t i) {
      return (eeprom_settings.mesh_min_y) + i * (float(eeprom_settings.mesh_max_y - (eeprom_settings.mesh_min_y)) / GRID_MAX_CELLS_Y);
  }
#endif

void JYEnhancedClass::UpdateAxis(const AxisEnum axis) {
  const xyz_float_t _axis_min = { (float)eeprom_settings.x_min_pos, (float)eeprom_settings.y_min_pos, Z_MIN_POS };
  const xyz_float_t _axis_max = { (float)eeprom_settings.x_max_pos, (float)eeprom_settings.y_max_pos, (float)eeprom_settings.z_max_pos };
  if (axis == NO_AXIS_ENUM || axis == ALL_AXES_ENUM) {
    LOOP_L_N(i,2) {
      #if HAS_WORKSPACE_OFFSET
        workspace_offset[i] = home_offset[i] + position_shift[i];
      #endif
      soft_endstop.min[i] = _axis_min[i];
      soft_endstop.max[i] = _axis_max[i];
    }
  }
  else {
    soft_endstop.min[axis] = _axis_min[axis];
    soft_endstop.max[axis] = _axis_max[axis];
  }
}

void JYEnhancedClass::ApplyPhySet() {
  update_software_endstops(temp_val.axis);
}

#if HAS_MESH
  void JYEnhancedClass::C29() {
    if (!parser.seen("LRFB")) return C29_report();
    if (parser.seenval('L')) {
      eeprom_settings.mesh_min_x = parser.value_float();
      LIMIT( eeprom_settings.mesh_min_x, MIN_MESH_INSET, MAX_MESH_INSET);
    }
    if (parser.seenval('R')) {
      eeprom_settings.mesh_max_x = parser.value_float();
      LIMIT( eeprom_settings.mesh_max_x, MIN_MESH_INSET, MAX_MESH_INSET);
    }
    if (parser.seenval('F')) {
      eeprom_settings.mesh_min_y = parser.value_float();
      LIMIT( eeprom_settings.mesh_min_y, MIN_MESH_INSET, MAX_MESH_INSET);
    }
    if (parser.seenval('B')) {
      eeprom_settings.mesh_max_y = parser.value_float();
      LIMIT( eeprom_settings.mesh_max_y, MIN_MESH_INSET, MAX_MESH_INSET);
    }
    ApplyPhySet();
  }

  void JYEnhancedClass::C29_report(const bool forReplay/*=true*/) {
    gcode.report_heading_etc(forReplay, F("Mesh Insets L(mm) R(mm) F(mm) B(mm)"));
    SERIAL_ECHOLNPGM_P(
      PSTR("  C29 L"), eeprom_settings.mesh_min_x,
      PSTR(" R"), eeprom_settings.mesh_max_x,
      PSTR(" F"), eeprom_settings.mesh_min_y,
      PSTR(" B"), eeprom_settings.mesh_max_y
    );
  }
#endif  // HAS_MESH



void JYEnhancedClass::C100() {
  if (!parser.seen("XY")) return C100_report();
  xy_int_t xymin_value;
  if (parser.seenval('X')) {
    xymin_value.x = parser.value_int();
    if (!WITHIN(xymin_value.x, -500, 500)) return CrealityDWIN.DWIN_CError();
    else eeprom_settings.x_min_pos = xymin_value.x; 
    ApplyPhySet();
  }
  if (parser.seenval('Y')) {
    xymin_value.y = parser.value_int();
    if (!WITHIN(xymin_value.y, -500, 500)) return CrealityDWIN.DWIN_CError();
    else eeprom_settings.y_min_pos = xymin_value.y;
    ApplyPhySet();
  }
}

void JYEnhancedClass::C100_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("MIN Position X(-500 to 500 mm) Y(-500 to 500 mm)"));
  SERIAL_ECHOLNPGM_P(
    PSTR("  C100 X"), eeprom_settings.x_min_pos,
    PSTR(" Y"), eeprom_settings.y_min_pos
  );
}

void JYEnhancedClass::C101() {
  if (!parser.seen("XYZ")) return C101_report();
  xyz_int_t xyzmax_value;
  if (parser.seenval('X')) {
    xyzmax_value.x = parser.value_int();
    if (!WITHIN(xyzmax_value.x, X_BED_MIN, 999)) return CrealityDWIN.DWIN_CError();
    eeprom_settings.x_max_pos = xyzmax_value.x;
    ApplyPhySet();
  }
  if (parser.seenval('Y')) {
    xyzmax_value.y = parser.value_int();
    if (!WITHIN(xyzmax_value.y, Y_BED_MIN, 999)) return CrealityDWIN.DWIN_CError();
    eeprom_settings.y_max_pos = xyzmax_value.y;
    ApplyPhySet();
  }
  if (parser.seenval('Z')) {
    xyzmax_value.z = parser.value_int();
    if (!WITHIN(xyzmax_value.z, 100, 999)) return CrealityDWIN.DWIN_CError();
    eeprom_settings.z_max_pos = xyzmax_value.z;
    ApplyPhySet();
  }
}

void JYEnhancedClass::C101_report(const bool forReplay/*=true*/) {
    gcode.report_heading(forReplay, F("X,Y,Z MAX POS X(150 to 999 mm) Y(150 to 999 mm) Z(100 to 999 mm)"));
    SERIAL_ECHOLNPGM_P(
        PSTR("  C101 X"), eeprom_settings.x_max_pos
      , PSTR(" Y"), eeprom_settings.y_max_pos
      , PSTR(" Z"), eeprom_settings.z_max_pos
    );
}

void JYEnhancedClass::C102() {
  if (!parser.seen("XY")) return C102_report();
  xy_int_t xysize_value;
  if (parser.seenval('X')) {
    xysize_value.x = parser.value_int();
    if (!WITHIN(xysize_value.x, X_BED_MIN, X_MAX_POS)) return CrealityDWIN.DWIN_CError();
    else { eeprom_settings.x_bed_size = xysize_value.x; eeprom_settings.x_max_pos = eeprom_settings.x_bed_size; }
    ApplyPhySet();
  }
  if (parser.seenval('Y')) {
    xysize_value.y = parser.value_int();
    if (!WITHIN(xysize_value.y, Y_BED_MIN, Y_MAX_POS)) return CrealityDWIN.DWIN_CError();
    else { eeprom_settings.y_bed_size = xysize_value.y; eeprom_settings.y_max_pos = eeprom_settings.y_bed_size; }
    ApplyPhySet();
  }
}

void JYEnhancedClass::C102_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("X_BED_SIZE X(150 to 999 mm) Y(150 to 999 mm)"));
  SERIAL_ECHOLNPGM_P(
    PSTR("  C102 X"), eeprom_settings.x_bed_size,
    PSTR(" Y"), eeprom_settings.y_bed_size
  );
}

#if ENABLED(NOZZLE_PARK_FEATURE)
  void JYEnhancedClass::C125() {
    xyz_int_t park_value = eeprom_settings.Park_point;
    if (!parser.seen("XYZ")) return C125_report();
    if (parser.seenval('X')) {
      park_value.x = static_cast<int>(parser.value_linear_units());
      if (!WITHIN(park_value.y, 0, Y_MAX_POS)) return CrealityDWIN.DWIN_CError();
      else eeprom_settings.Park_point.x = park_value.x;
    }
    if (parser.seenval('Y')) {
      park_value.y = static_cast<int>(parser.value_linear_units());
      if (!WITHIN(park_value.y, 0, Y_MAX_POS)) return CrealityDWIN.DWIN_CError();
      else eeprom_settings.Park_point.y = park_value.y;
    }
    if (parser.seenval('Z')) {
      park_value.z = static_cast<int>(parser.value_linear_units());
      if (!WITHIN(park_value.z, MIN_PARK_POINT_Z, Z_MAX_POS)) return CrealityDWIN.DWIN_CError();
      else eeprom_settings.Park_point.z = park_value.z;
    }
  }

  void JYEnhancedClass::C125_report(const bool forReplay/*=true*/) {
    gcode.report_heading(forReplay, F("Park Head Position X(mm) Y(mm) Z(mm)"));
    SERIAL_ECHOLNPGM_P(
      PSTR("  C125 X"), eeprom_settings.Park_point.x,
      PSTR(" Y"), eeprom_settings.Park_point.y,
      PSTR(" Z"), eeprom_settings.Park_point.z
    );
  }
#endif  // Set park position

void JYEnhancedClass::C562() {
  if (!parser.seen("E")) return C562_report();
    if (parser.seenval('E')) {
      eeprom_settings.Invert_E0 = parser.value_bool() ? !eeprom_settings.Invert_E0 : eeprom_settings.Invert_E0;
      CrealityDWIN.DWIN_Invert_E0();
    }
  }  // Invert Extruder

void JYEnhancedClass::C562_report(const bool forReplay/*=true*/) {
  gcode.report_heading(forReplay, F("Invert extruder direction E(0->Off, 1->On)"));
  SERIAL_ECHOLNPGM_P(
    PSTR("  C562 E"), eeprom_settings.Invert_E0
  );
}

#if HAS_BED_PROBE
  void JYEnhancedClass::C851() {
    if (!parser.seen("FSM")) return C851_report();
    uint16_t z_fast_feedrate_parser = eeprom_settings.zprobefeedfast ;
    uint16_t z_slow_feedrate_parser = eeprom_settings.zprobefeedslow;
    int16_t margin_parser;
  
    if (parser.seenval('F')) {
      z_fast_feedrate_parser = static_cast<int>(parser.value_linear_units());
      if (!WITHIN(z_fast_feedrate_parser, (MIN_Z_PROBE_FEEDRATE * 2), MAX_Z_PROBE_FEEDRATE)) return CrealityDWIN.DWIN_CError();
      else eeprom_settings.zprobefeedfast = z_fast_feedrate_parser;
    }
    if (parser.seenval('S')) {
     z_slow_feedrate_parser = static_cast<int>(parser.value_linear_units());
     if  (!WITHIN(z_slow_feedrate_parser, MIN_Z_PROBE_FEEDRATE, MAX_Z_PROBE_FEEDRATE)) return CrealityDWIN.DWIN_CError();
     else eeprom_settings.zprobefeedslow = z_slow_feedrate_parser;
    }
    if (parser.seenval('M')) {
      margin_parser = parser.value_int();
      if (!WITHIN(margin_parser, MIN_PROBE_MARGIN, MAX_PROBE_MARGIN)) return CrealityDWIN.DWIN_CError();
      else eeprom_settings.probing_margin = (float)margin_parser;
    }
  }

  void JYEnhancedClass::C851_report(const bool forReplay/*=true*/) {
    gcode.report_heading(forReplay, F("Z Probe Fast/Slow Feedrate F(mm/min) S(mm/min) M(5 to 60 mm)"));
    SERIAL_ECHOLNPGM_P(
      PSTR("  C851 F"), eeprom_settings.zprobefeedfast,
      PSTR(" S"), eeprom_settings.zprobefeedslow,
      PSTR(" M"), eeprom_settings.probing_margin
    );
  }
#endif

#endif // JYENHANCED && ENABLED(DWIN_CREALITY_LCD_JYERSUI)

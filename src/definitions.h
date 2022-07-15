/*
    EngineDataLogger
    Copyright (C) 2018-2020  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cstdint>

// TODO option-ify
#define RUNNING_RPM 900
#define STOPPED_RPM 500
#define LOG_INTERVAL 50000
#define O2_PIN 26	// Default //
#define LC2_POWER_DELAY 15 // delay in seconds. Default //
#define ENGINE_DATA_ADDR 0x04

#define TIME_BUF_LEN 256
#define LOG_FILE_LEN 4096
#define FC_CMD_SIZE 8

// CmdD flags
const uint8_t		BRAKE_ON = 1 << 2,
                HORN_ON = 1 << 3,
                LEFT_ON = 1 << 4,
                RIGHT_ON = 1 << 5,
                HIGH_BEAMS_ON = 1 << 6,
                KILL_ON = 1 << 7;

// CmdC flags
const uint8_t		CLUTCH_DISENGAGED = 1 << 2,
                KICKSTAND_UP = 1 << 3,
                IN_NEUTRAL = 1 << 4;

// CmdA flags
const uint8_t		ENGINE_RUNNING = 1 << 7;

struct fc_data {
  uint8_t serialCmdD = 0,
          serialCmdB = 0,
          serialCmdC = 0,
          serialCmdA = 0,
          inputCmdD = 0,
          inputCmdC = 0;
  float	systemVoltage = 0.0;
  bool	brake_on = false;
  bool	horn_on = false;
  bool	left_on = false;
  bool	right_on = false;
  bool	high_on = false;
  bool	kill_on = false;
  bool	clutch_disengaged = false;
  bool	kickstand_up = false;
  bool	in_neutral = false;
};
struct			engine_data {
  uint16_t	rpm = 0;
  float		batteryVoltage = 0.0,
          temp_oil = 0, // divide by 100 for actual temp
          pres_oil = 0, // divide by 100 for actual pressure
          speed = 0; // divide by 100 for actual speed
  uint32_t	odometer = 0;
  uint32_t	trip = 0;
};

enum System_CMD {
  NO_CMD,
  TRPRST,
  O2MANON,
  O2MANOFF
};

/*
 * @brief Loggable data
 */
struct	bike_data {
  /// RPM. Best of available sources.
  int rpm;
  /// RPM. From Ignitech TCIP-4
  int ig_rpm;
  /// RPM. From EngineData device.
  int32_t alt_rpm;
  /// Vehicle Speed
  float speed;
  /// Odometer. Fixed point 0.01 scale (divide by 100 for real value)
  uint32_t odometer;
  /// Resettable Trip Distance. Fixed point 0.01 scale (divide by 100 for real value)
  uint32_t trip;
  /// Voltage at Front Controls.
  float systemvoltage;
  /// Voltage on EngineData board
  float batteryvoltage;
  /// Oil temperature in degrees Farenheight
  float oil_temp;
  /// Guage pressure of oil in psi
  float oil_pres;
  /// Blinker currently flashing (does not indicate actual lamp status)
  bool blink_left;
  /// Blinker currently flashing (does not indicate actual lamp status)
  bool blink_right;
  /// Lambda. Fixed point 0.01 scale (divide by 100 for real value)
  uint16_t lambda;
  /// Manifold Absolute Pressure. kilo Pascals
  int map_kpa;
  /// Throttle Position Sensor 0-100%
  int tps_percent;
  /// Engine is considered running or not
  bool engineRunning;
  /// Ignition advance in degrees
  uint8_t advance1;
  /// Ignition advance in degrees
  uint8_t advance2;
  /// Ignition advance in degrees
  uint8_t advance3;
  /// Ignition advance in degrees
  uint8_t advance4;

  /// Yaw. Rotation orthoganal to the ground.
  float yaw;
  /// Pitch. Rotation front to back.
  float pitch;
  /// Roll. Rotation side to side. (ie. "lean angle")
  float roll;

  /// Linear Acceleration. Forward and back.
  double acc_forward;
  /// Linear Acceleration. Side to Side.
  double acc_side;
  /// Linear Acceleration. Up and down.
  double acc_vert;
};

/// Enumeration of loggable data. These get pushed into a vector to represent user choice in order.
typedef enum {
  FMT_RPM,
  FMT_IG_RPM,
  FMT_ALT_RPM,
  FMT_SPEED,
  FMT_ODOMETER,
  FMT_TRIP,
  FMT_SYSTEMVOLTAGE,
  FMT_BATTERYVOLTAGE,
  FMT_OIL_TEMP,
  FMT_OIL_PRES,
  FMT_BLINK_LEFT,
  FMT_BLINK_RIGHT,
  FMT_LAMBDA,
  FMT_MAP_KPA,
  FMT_TPS_PERCENT,
  FMT_ENGINERUNNING,
  FMT_TIME,
  FMT_ADVANCE1,
  FMT_ADVANCE2,
  FMT_ADVANCE3,
  FMT_ADVANCE4,
  FMT_YAW,
  FMT_PITCH,
  FMT_ROLL,
  FMT_ACC_FORWARD,
  FMT_ACC_SIDE,
  FMT_ACC_VERT
} log_fmt_data;
#endif

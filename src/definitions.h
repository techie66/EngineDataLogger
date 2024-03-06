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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h> // C99 Bool Support
#include <time.h>

// TODO option-ify
#define RUNNING_RPM 900
#define STOPPED_RPM 500
#define LOG_INTERVAL 50000
#define GPX_INTERVAL 1000000
#define O2_PIN 26	// Default //
#define LC2_POWER_DELAY 15 // delay in seconds. Default //
#define ENGINE_DATA_ADDR 0x04

#define TIME_BUF_LEN 256
#define LOG_FILE_LEN 4096
#define FC_CMD_SIZE 8
#define WATTS_PER_HPI 745.6998715823
#define _DEFAULT_WEIGHT 300

// CmdD flags
extern const uint8_t  BRAKE_ON,
       HORN_ON,
       LEFT_ON,
       RIGHT_ON,
       HIGH_BEAMS_ON,
       KILL_ON;

// CmdC flags
extern const uint8_t  CLUTCH_DISENGAGED,
       KICKSTAND_UP,
       IN_NEUTRAL;

// CmdA flags
extern const uint8_t		ENGINE_RUNNING;

struct      engine_data {
  uint16_t  rpm;
  float     batteryVoltage,
            temp_oil, // divide by 100 for actual temp
            pres_oil, // divide by 100 for actual pressure
            speed; // divide by 100 for actual speed
  uint32_t	odometer;
  uint32_t	trip;
};

extern const struct engine_data ENGINE_DATA_DEFAULT;

enum System_CMD {
  NO_CMD,
  TRPRST,
  LOGRST,
  O2MANON,
  O2MANOFF
};

typedef enum {
  GPS_NO_FIX = 1,
  GPS_2D_FIX,
  GPS_3D_FIX
} gps_fixtype;

/*
 * @brief Loggable data
 */
struct bike_data {
  /// RPM. Best of available sources.
  uint16_t rpm;
  /// RPM. From Ignitech TCIP-4
  uint16_t ig_rpm;
  /// RPM. From EngineData device.
  uint16_t alt_rpm;
  /// Vehicle Speed
  float speed;
  /// Gear
  char gear;
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
  /// Sent to Front Controls,
  uint8_t serialCmdD,
          /// Sent to Front Controls,
          serialCmdB,
          /// Sent to Front Controls,
          serialCmdC,
          /// Sent to Front Controls, bit 7 indicates engine is running
          serialCmdA,
          /// Raw data byte when reading front controls over USB/Serial
          inputCmdD,
          /// Raw data byte when reading front controls over USB/Serial
          inputCmdC;
  /// From Front Controls
  bool  brake_on;
  /// From Front Controls
  bool  horn_on;
  /// From Front Controls
  bool  high_on;
  /// From Front Controls
  bool  kill_on;
  /// From Front Controls
  bool  clutch_disengaged;
  /// From Front Controls
  bool  kickstand_up;
  /// From Front Controls
  bool  in_neutral;
  /// Blinker currently flashing (does not indicate actual lamp status)
  bool blink_left;
  /// Blinker currently flashing (does not indicate actual lamp status)
  bool blink_right;
  /// Lambda. Fixed point 0.001 scale (divide by 1000 for real value)
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
  /// Mounting Offset. Used to make sensor read 0 when vehicle level.
  float pitch_offset;
  /// Mounting Offset. Used to make sensor read 0 when vehicle level.
  float roll_offset;
  /// If sensor mounted so that roll and pitch are swapped.
  bool roll_pitch_swap;

  /// Linear Acceleration. Forward and back.
  double acc_forward;
  /// Linear Acceleration. Side to Side.
  double acc_side;
  /// Linear Acceleration. Up and down.
  double acc_vert;

  /// Calculated power (HP-Imperial)
  int power;

  /// Weight of vehicle (total/loaded) in kg's.
  int weight;

  /// Latitude in Decimal Degrees
  double lat;
  /// Longitude in Decimal Degrees
  double lon;
  /// GPS Elevation
  int altitude;
  /// GPS Speed
  float gps_speed;
  /// GPS Heading
  float gps_heading;
  /// GPS Fix Type
  gps_fixtype gpsfix;
  /// PDOP
  float pdop;
  /// HDOP
  float hdop;
  /// VDOP
  float vdop;
  /// Satellites in View
  int satV;
  /// Satellites in Use
  int satU;
  /// GPS Time (UTC)
  time_t gpstime;
};
extern const struct bike_data BIKE_DATA_DEFAULT;

/// Enumeration of loggable data. These get pushed into a vector to represent user choice in order.
typedef enum {
  FMT_RPM,
  FMT_IG_RPM,
  FMT_ALT_RPM,
  FMT_SPEED,
  FMT_GEAR,
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
  FMT_ACC_VERT,
  FMT_POWER,
  FMT_LAT,
  FMT_LON,
  FMT_ALTITUDE,
  FMT_GPS_SPEED,
  FMT_GPS_HEADING,
  FMT_GPS_FIX,
  FMT_GPS_PDOP,
  FMT_GPS_HDOP,
  FMT_GPS_VDOP,
  FMT_SAT_INVIEW,
  FMT_SAT_INUSE,
  FMT_GPS_TIME,
} log_fmt_data;

#ifdef __cplusplus
}
#endif

#endif

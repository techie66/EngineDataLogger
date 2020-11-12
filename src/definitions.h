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
#define O2_PIN 26	// Default //
#define LC2_POWER_DELAY 15 // delay in seconds. Default //
#define FC_PORT "/dev/front_controls"
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

struct			fc_data {
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

struct	bike_data {
	int ig_rpm;
	int32_t alt_rpm;
	float speed;
	uint32_t odometer;
	uint32_t trip;
	float systemvoltage;
	float batteryvoltage;
	float oil_temp;
	float oil_pres;
	bool blink_left;
	bool blink_right;
	uint16_t lambda;
	int map_kpa;
	bool engineRunning;
};
#endif

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cstdint>

const uint16_t		RUNNING_RPM=500;
const uint8_t		engine_data_addr = 0x04,
      			FC_CMD_SIZE = 8;


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
						speed = 0; // divide by 100 for actual speed
				uint32_t	odometer = 0;
				uint32_t	trip = 0;
			};

#endif

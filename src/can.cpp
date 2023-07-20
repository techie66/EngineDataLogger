/*
    EngineDataLogger
    Copyright (C) 2018-2022  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "can.h"

bool can_sock_connect( int can_s, char const *can_arg )
{
  struct sockaddr_can addr;
  struct ifreq ifr;
  strcpy(ifr.ifr_name, can_arg);
  if ( ioctl(can_s, SIOCGIFINDEX, &ifr) < 0 ) {
    error_message(WARN, "Warning:CAN: ioctl failed : %s, Retrying...", strerror(errno));
    return false;
  }

  else {
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if ( bind(can_s, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
      error_message(WARN, "Warning:CAN: bind failed : %s, Retrying...", strerror(errno));
      return false;
    }

    else {
      return true;
    }
  }
}

void can_parse(const can_frame &frame, bike_data &log_data, const int can_s)
{
  int _status = EXIT_FAILURE;
  switch (frame.can_id & 0x7FFFFFFF) {
    case IMU_CAN_BODY_POSITION_FRAME_ID: {
      struct imu_can_body_position_t st_body_pos;
      int status_unpack = imu_can_body_position_unpack(&st_body_pos, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        // Physical mounting may require changing up pitch and roll.
        log_data.yaw = imu_can_body_position_yaw_angle_decode(st_body_pos.yaw_angle);
        log_data.pitch = log_data.pitch_offset + imu_can_body_position_pitch_angle_decode(st_body_pos.pitch_angle);
        log_data.roll = log_data.roll_offset + imu_can_body_position_roll_angle_decode(st_body_pos.roll_angle);
	if (log_data.roll_pitch_swap) {
          float swap_temp = log_data.pitch;
	  log_data.pitch = log_data.roll;
	  log_data.roll = swap_temp;
	}
      }
      error_message(INFO, "CAN:Body Roll: %f", log_data.roll);
    }
    break;

    case IMU_CAN_BODY_ACCEL_FRAME_ID: {
      struct imu_can_body_accel_t st_body_acc;
      int status_unpack = imu_can_body_accel_unpack(&st_body_acc, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        // Physical mounting may require changing up x and y
        log_data.acc_forward = imu_can_body_accel_acc_x_decode(st_body_acc.acc_x);
        log_data.acc_side = imu_can_body_accel_acc_y_decode(st_body_acc.acc_y);
        log_data.acc_vert = imu_can_body_accel_acc_z_decode(st_body_acc.acc_z);
      }
      error_message(INFO, "CAN:IMU X Accel: %f", log_data.acc_forward);
    }
    break;

    case IGNITECH_CAN_IGNITECH_WB_2_FRAME_ID: {
      struct ignitech_can_ignitech_wb_2_t st_wb2;
      int status_unpack = ignitech_can_ignitech_wb_2_unpack(&st_wb2, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        log_data.lambda = st_wb2.lambda * 10 ; // Convert scaled by 0.01 to 0.001
      }
      error_message(DEBUG, "CAN:ignitech lambda: %f", ignitech_can_ignitech_wb_2_lambda_decode(st_wb2.lambda));
    }
    break;

    case OBD2_OBD2_REQUEST_FRAME_ID: {
      _status = obd2_process(frame, log_data, can_s);
      error_message(DEBUG, "CAN:OBD2 Request");
    }
    break;

    case GPS_GPS_LOC_FRAME_ID: {
      error_message(DEBUG, "CAN:GPS Location Frame");
      struct gps_gps_loc_t st_gps_loc;
      int status_unpack = gps_gps_loc_unpack(&st_gps_loc, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        log_data.lat = gps_gps_loc_lat_decimal_degrees_decode(st_gps_loc.lat_decimal_degrees);
        log_data.lon = gps_gps_loc_long_decimal_degrees_decode(st_gps_loc.long_decimal_degrees);
      }
    }
    break;

    case GPS_GPS_NAV_FRAME_ID: {
      error_message(DEBUG, "CAN:GPS Navigation Frame");
      struct gps_gps_nav_t st_gps_nav;
      int status_unpack = gps_gps_nav_unpack(&st_gps_nav, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        log_data.altitude = gps_gps_nav_altitude_decode(st_gps_nav.altitude);
        log_data.gps_speed = gps_gps_nav_speed_decode(st_gps_nav.speed);
        log_data.gps_heading = gps_gps_nav_heading_decode(st_gps_nav.heading);
      }
    }
    break;

    case GPS_GPS_STAT_FRAME_ID: {
      error_message(DEBUG, "CAN:GPS Stats Frame");
      struct gps_gps_stat_t st_gps_stat;
      int status_unpack = gps_gps_stat_unpack(&st_gps_stat, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        log_data.satV = st_gps_stat.visible_satellites;
        log_data.satU = st_gps_stat.active_satellites;
        log_data.gpsfix = (gps_fixtype)st_gps_stat.type;
        log_data.pdop = gps_gps_stat_pdop_decode(st_gps_stat.pdop);
        log_data.hdop = gps_gps_stat_hdop_decode(st_gps_stat.hdop);
        log_data.vdop = gps_gps_stat_vdop_decode(st_gps_stat.vdop);
      }
    }
    break;

    case GPS_GPS_TIME_FRAME_ID: {
      error_message(DEBUG, "CAN:GPS Time Frame");
      struct gps_gps_time_t st_gps_time;
      int status_unpack = gps_gps_time_unpack(&st_gps_time, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        struct tm tm_gps_time;
        tm_gps_time.tm_year = st_gps_time.year - 1900;
        tm_gps_time.tm_mon = st_gps_time.month - 1;
        tm_gps_time.tm_mday = st_gps_time.day;
        tm_gps_time.tm_hour = st_gps_time.hour;
        tm_gps_time.tm_min = st_gps_time.minute;
        tm_gps_time.tm_sec = st_gps_time.second;
        tm_gps_time.tm_isdst = 0;
        log_data.gpstime = timegm(&tm_gps_time);
      }
    }
    break;

    case FC_FRONT_CONTROLS_FRAME_ID: {
      error_message(DEBUG, "CAN:Front Controls Frame");
      struct fc_front_controls_t st_front_controls;
      int status_unpack = fc_front_controls_unpack(&st_front_controls, frame.data, sizeof(frame.data));
      if ( status_unpack == 0 )
      {
        _status = EXIT_SUCCESS;
        log_data.brake_on = st_front_controls.brake;
        log_data.horn_on = st_front_controls.horn;
        log_data.blink_left = st_front_controls.left;
        log_data.blink_right = st_front_controls.right;
        log_data.high_on = st_front_controls.high;
        log_data.kill_on = st_front_controls.kill;
        log_data.clutch_disengaged = st_front_controls.clutch;
        log_data.kickstand_up = st_front_controls.kickstand;
        log_data.in_neutral = st_front_controls.neutral;
        log_data.systemvoltage = fc_front_controls_voltage_decode(st_front_controls.voltage);
      }
    }
    break;

    default: /// Default if can_id is not in the switch print out message details
      error_message(WARN, "WARN:Unknown CAN frame%: %X", frame.can_id);
  }
  if (_status == EXIT_FAILURE) {
    // TODO do something if it wasn't decoded?
    //printf("%8X %d\n", frame.can_id, frame.can_dlc);
  }
}

int obd2_process(const can_frame &frame, bike_data &log_data, const int can_s)
{
  int _status = EXIT_FAILURE;
  struct obd2_obd2_request_t obd2Request;
  obd2_obd2_request_unpack(&obd2Request, frame.data, frame.can_dlc);
  // Check Valid Mode and PIDs
  if ( !obd2_obd2_request_mode_is_in_range(obd2Request.mode) )
    return EXIT_FAILURE;
  if ( !obd2_obd2_request_pid_is_in_range(obd2Request.pid) )
    return EXIT_FAILURE;

  // Send responses for each supported mode/pid
  if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE ) {
    switch (obd2Request.pid) {
      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_00_PI_DS_SUPPORTED_01_20_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_00_PI_DS_SUPPORTED_01_20_CHOICE;
        _obd2_response.s1_pid_00_pi_ds_supported_01_20 = 0x003C8011;
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_B_INTAKE_MANI_ABS_PRESS_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: MAP kpa");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_0_b_intake_mani_abs_press = obd2_obd2_s1_pid_0_b_intake_mani_abs_press_encode(log_data.map_kpa);
        _obd2_response.length = 4u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_C_ENGINE_RPM_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: RPM");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_0_c_engine_rpm = obd2_obd2_s1_pid_0_c_engine_rpm_encode(log_data.rpm);
        _obd2_response.length = 4u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_C_ENGINE_RPM_CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_D_VEHICLE_SPEED_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Vehicle Speed");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_D_VEHICLE_SPEED_CHOICE;
        _obd2_response.s1_pid_0_d_vehicle_speed = obd2_obd2_s1_pid_0_d_vehicle_speed_encode( (log_data.speed * 1.609344) );
        _obd2_response.length = 3u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_E_TIMING_ADVANCE_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Timing Advance");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_E_TIMING_ADVANCE_CHOICE;
        _obd2_response.s1_pid_0_e_timing_advance = obd2_obd2_s1_pid_0_e_timing_advance_encode(log_data.advance1 * 1.0);
        _obd2_response.length = 3u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_11_THROTTLE_POSITION_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Throttle Position");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_11_THROTTLE_POSITION_CHOICE;
        _obd2_response.s1_pid_11_throttle_position = obd2_obd2_s1_pid_11_throttle_position_encode(log_data.tps_percent);
        _obd2_response.length = 3u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_1_C_OBD_STANDARD_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: OBD Standard");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_1_C_OBD_STANDARD_CHOICE;
        _obd2_response.s1_pid_1_c_obd_standard = OBD2_OBD2_S1_PID_1_C_OBD_STANDARD_NOT_OBD_COMPLIANT_CHOICE;
        _obd2_response.length = 3u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_20_PI_DS_SUPPORTED_21_40_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        _obd2_response.s1_pid_20_pi_ds_supported_21_40 = 0x10000001;
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_24_OXY_SENSOR1_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: lambda");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_24_oxy_sensor1_faer = obd2_obd2_s1_pid_24_oxy_sensor1_faer_encode(log_data.lambda);
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_40_PI_DS_SUPPORTED_41_60_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        _obd2_response.s1_pid_40_pi_ds_supported_41_60 = 0x40000010;
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_42_CONTROL_MODULE_VOLT_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: module voltage");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_42_control_module_volt = obd2_obd2_s1_pid_42_control_module_volt_encode(log_data.batteryvoltage);
        _obd2_response.length = 4u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_5_C_ENGINE_OIL_TEMP_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Oil Temp");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_5_c_engine_oil_temp = obd2_obd2_s1_pid_5_c_engine_oil_temp_encode( (log_data.oil_temp-32)*5.0/9.0 );
        _obd2_response.length = 3u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_60_PI_DS_SUPPORTED_61_80_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        _obd2_response.s1_pid_60_pi_ds_supported_61_80 = 0x00000001;
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_80_PI_DS_SUPPORTED_81_A0_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        _obd2_response.s1_pid_60_pi_ds_supported_61_80 = 0x00000001;
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_A0_PI_DS_SUPPORTED_A1_C0_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        _obd2_response.s1_pid_60_pi_ds_supported_61_80 = 0x00000000;
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_A6_ODOMETER_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Odometer");
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_a6_odometer = obd2_obd2_s1_pid_a6_odometer_encode( log_data.odometer * 1.609344 );
        _obd2_response.length = 6u; // MODE(1) + PID(1) + Data Bytes
        _obd2_response.response = 1u;  // Every response packet is 1
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = obd2Request.pid;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_FREEZE_FRAME_DATA__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_STORED_DT_CS__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_CLEAR_DT_CS_AND_STORED_VALUES_CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_OXYGEN_SENSOR_MONITORING__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_OTHER_SYSTEM_MONITORING__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_PENDING_DT_CS__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_CONTROL_ON_BOARD_SYSTEM__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_REQUEST_VEHICLE_INFORMATION__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_PERMANENT_DT_CS___CLEARED_DT_CS___CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == 53u ) { // Custom Service
    switch (obd2Request.pid) {
      case 1u: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Custom Service: Blinkers");
        struct can_frame _response;
        _response.data[0]= 4u;
        _response.data[1]= 0x75; // Custon Service
        _response.data[2]= 1u; // Custom PID
        _response.data[3]= 0x00;
        _response.data[4]= log_data.blink_left | (log_data.blink_right << 1);
        _response.data[5]= 0x00;
        _response.data[6]= 0x00;
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      case 2u: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Custom Service: Gear");
        struct can_frame _response;
        _response.data[0]= 4u;
        _response.data[1]= 0x75; // Custom Service
        _response.data[2]= 2u; // Custom PID
        _response.data[3]= 0x00;
        if ( log_data.gear == 'N' )
          _response.data[4]= 0;
        else if ( log_data.gear == '?' )
          _response.data[4]= -1;
        else
          _response.data[4]= log_data.gear - 48; // Convert from 'char' to actual number
        _response.data[5]= 0x00;
        _response.data[6]= 0x00;
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }
  return _status;
}

